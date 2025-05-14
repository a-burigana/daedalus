//
// DAEDALUS - DynAmic Epistemic and DoxAstic Logic Universal Solver (MIT License)
//
// Copyright (c) 2023-2024 Alessandro Burigana
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "printer.h"
#include "../include/del/formulas/propositional/true_formula.h"
#include "../include/del/formulas/propositional/false_formula.h"
#include "../include/del/formulas/propositional/atom_formula.h"
#include "../include/del/formulas/propositional/and_formula.h"
#include "../include/del/formulas/propositional/not_formula.h"
#include "../include/del/semantics/kripke/update/updater.h"
#include "../include/del/semantics/kripke/bisimulation/bisimulator.h"
#include "../include/search/planner.h"
#include "../include/search/delphic/delphic_planner.h"
#include "../include/del/semantics/delphic/delphic_utils.h"
#include "../include/del/semantics/delphic/update/union_updater.h"
#include "../include/utils/time_utils.h"
#include "../include/utils/timer.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <chrono>
#include <string>
#include <thread>

using namespace daedalus::tester;
using namespace std::chrono_literals;

printer::printer(bool out_to_file, const std::string &path, const std::string &name) :
        m_out_to_file{out_to_file} {
    if (not out_to_file)
        return;

    if (not std::filesystem::exists(path))
        std::filesystem::create_directories(path);

    m_output_stream = std::ofstream{path + name};
}

std::ostream &printer::out() {
    return m_out_to_file ? m_output_stream : std::cout;
}

void printer::set_out_to_file(bool out_to_file) {
    m_out_to_file = out_to_file;
}

void printer::to_string(std::ofstream &os, const kripke::state &s, const del::label_storage &l_storage) {
    using edges_map = std::map<std::pair<world_id, world_id>, std::vector<del::agent>>;
    const std::string font = std::string{"\"Helvetica,Arial,sans-serif\""};

    os
        << "digraph {" << std::endl
        << "\tfontname=" << font << ";" << std::endl
        << "\tnode [fontname=" << font << "];" << std::endl
        << "\tedge [fontname=" << font << "];" << std::endl
        << "\tlabeljust=l;" << std::endl
        << "\trankdir=BT;" << std::endl
        << "\tranksep=0.5;" << std::endl
        << "\tnewrank=true;" << std::endl
        << "\tcompound=true;" << std::endl;

    os << std::endl << "\t{ node[shape=doublecircle] ";

    for (const world_id wd : s.get_designated_worlds())
        os << "w" << wd << "; ";

    os << "}" << std::endl << std::endl;

    // Map: <accessible_worlds_hash> -> <list_of_worlds>
    try {
        std::map<unsigned long, std::deque<world_id>> ranks;

        for (world_id w = 0; w < s.get_worlds_number(); ++w) {
            boost::dynamic_bitset<> out(s.get_worlds_number());

            for (del::agent ag = 0; ag < s.get_language()->get_agents_number(); ++ag)
                out |= *s.get_agent_possible_worlds(ag, w);

            ranks[out.to_ulong()].emplace_back(w);
        }

        for (const auto &[hash, ws]: ranks) {
            os << "\t{ rank = same; ";

            for (const world_id w_id: ws)
                os << "w" << w_id << "; ";

            os << "}" << std::endl;
        }

        os << std::endl;
    } catch (boost::wrapexcept<std::overflow_error> &e) {}

    edges_map edges;

    for (del::agent ag = 0; ag < s.get_language()->get_agents_number(); ++ag)
        for (world_id w = 0; w < s.get_worlds_number(); ++w)
            for (const world_id v : s.get_agent_possible_worlds(ag, w))
//            for (world_id v = 0; v < state.get_worlds_number(); ++v)
                if (edges.find({w, v}) == edges.end())      // state.has_edge(ag, w, v) and
                    edges[{w, v}] = {ag};
                else
                    edges[{w, v}].emplace_back(ag);

    auto print_ags = [&s](const std::vector<del::agent> &ags) {
        std::string ags_string;
        for (const del::agent &ag: ags)
            ags_string += std::string{s.get_language()->get_agent_name(ag)} + ", ";
        return ags_string.substr(0, ags_string.size() - 2);
    };

    for (auto &[t, ags]: edges) {
        const world_id from = t.first, to = t.second;
        auto it = edges.find({to, from});
        if (it == edges.end())
            os << "\tw" << from << " -> w" << to << " [label=\"" << print_ags(ags) << "\"];" << std::endl;
        else {
            std::vector<del::agent> ags2 = it->second;
            auto size = std::max(ags.size(), ags2.size());
            std::vector<del::agent> ags_intersection = std::vector<del::agent>(size), ags_difference = std::vector<del::agent>(size);

            std::sort(ags.begin(), ags.end());
            std::sort(ags2.begin(), ags2.end());

            auto it1 = std::set_intersection(ags.begin(), ags.end(), ags2.begin(), ags2.end(), ags_intersection.begin());
            auto it2 = std::set_difference(ags.begin(), ags.end(), ags2.begin(), ags2.end(), ags_difference.begin());

            ags_intersection.resize(it1 - ags_intersection.begin());
            ags_difference.resize(it2 - ags_difference.begin());

            if (not ags_intersection.empty()) {
                if (from < to)
                    os << "\tw" << from << " -> w" << to << " [label=\"" << print_ags(ags_intersection) << "\" dir=both];" << std::endl;
                else if (from == to)
                    os << "\tw" << from << " -> w" << to << " [label=\"" << print_ags(ags_intersection) << "\"];" << std::endl;
            }

            if (not ags_difference.empty())
                os << "\tw" << from << " -> w" << to << " [label=\"" << print_ags(ags_difference) << "\"];" << std::endl;
        }
    }

    os
        << "\tnode [] val_table [shape=none label=<"                                       << std::endl
        << "\t\t<TABLE border=\"0\" cellspacing=\"0\" cellborder=\"1\" cellpadding=\"2\">" << std::endl;

    os
        << "\t\t\t<TR>"             << std::endl
        << "\t\t\t\t<TD>World</TD>" << std::endl
        << "\t\t\t\t<TD>Depth</TD>" << std::endl
        << "\t\t\t\t<TD>Label</TD>" << std::endl
        << "\t\t\t</TR>"            << std::endl;

    for (world_id w = 0; w < s.get_worlds_number(); ++w) {
        os
            << "\t\t\t<TR>"                          << std::endl
            << "\t\t\t\t<TD>" << "w" << w << "</TD>" << std::endl;

        os
            << "\t\t\t\t<TD>" << s.get_depth(w) << "</TD>" << std::endl;

        os << "\t\t\t\t<TD>" << std::endl;

        for (del::atom p = 0; p < s.get_language()->get_atoms_number(); ++p) {
            if ((*l_storage.get(s.get_label_id(w)))[p]) {
                std::string_view color = "blue";        // s.get_label(w)[p] ? "blue" : "red";
                std::string_view sep   = " ";           // p < s.get_language()->get_atoms_number() - 1 ? ", " : "";

                os << "\t\t\t\t\t<font color=\"" << color << "\">" << s.get_language()->get_atom_name(p) << "</font>"
                   << sep << std::endl;
            }
        }

        os
            << "\t\t\t\t</TD>" << std::endl
            << "\t\t\t</TR>"   << std::endl;
    }

    os
        << "\t\t</TABLE>" << std::endl
        << "\t>];"        << std::endl
        << "}";
}

void printer::print_state(const kripke::state &s, const del::label_storage &l_storage, const std::string &path, const std::string &name) {
    if (not std::filesystem::exists(path))
        std::filesystem::create_directories(path);

    std::string dot_file_name = path + name + ".dot";
    std::string pdf_file_name = path + name + ".pdf";
    std::ofstream dot_file = std::ofstream{dot_file_name};

    to_string(dot_file, s, l_storage);
    dot_file.close();

    std::string command = "dot -Tpdf " + dot_file_name + " > " + pdf_file_name;
    system(command.c_str());
    command = "rm " + dot_file_name;
    system(command.c_str());

    dot_file.close();
}

void printer::print_state(const delphic::possibility_spectrum &W, const std::string &path, const std::string &name) {
    const state s = delphic_utils::convert(W);
    print_state(s, del::label_storage{}, path, name);
}

void printer::print_action(const kripke::action &a, const std::string &path) {
    if (not std::filesystem::exists(path))
        std::filesystem::create_directories(path);

    std::string dot_file_name = path + a.get_name() + ".dot";
    std::string pdf_file_name = path + a.get_name() + ".pdf";
    std::ofstream dot_file = std::ofstream{dot_file_name};

    dot_file << a;
    dot_file.close();

    std::string command = "dot -Tpdf " + dot_file_name + " > " + pdf_file_name;
    system(command.c_str());
    command = "rm " + dot_file_name;
    system(command.c_str());

    dot_file.close();
}

state_deque printer::print_states(kripke::state_deque &ss, kripke::action_deque &as, del::storages_handler_ptr handler,
                                  const del::formula_ptr &goal, const std::string &path, const std::string &name,
                                  bool apply_contraction, kripke::contraction_type type, const unsigned long b) {
    kripke::state &s = *ss.back();

    if (apply_contraction)
        s = std::get<1>(kripke::bisimulator::contract(type, s, b, handler));

    print_state(s, handler->get_label_storage(), path, name);

    if (as.empty())
        return ss;

    kripke::action &a = *as.front();

    if (type != contraction_type::full and b < a.get_maximum_depth() + goal->get_modal_depth()) {
        std::cout << "Bound too low for action '" << a.get_name() << "'" << std::endl;
        return ss;
    }

    if (not kripke::updater::is_applicable(s, a, handler->get_label_storage())) {
        std::cout << "Action '" << a.get_name() << "' is not applicable" << std::endl;
        return ss;
    }

    kripke::state s_ = kripke::updater::product_update(s, a, handler->get_label_storage());

    if (s_.satisfies(goal, handler->get_label_storage()))
        std::cout << "Goal found after applying '" << a.get_name() << "'" << std::endl;

    std::string new_name = name + "_" + a.get_name();
    ss.emplace_back(std::make_shared<kripke::state>(std::move(s_)));
    as.pop_front();

    return print_states(ss, as, handler, goal, path, new_name, apply_contraction, type, b - a.get_maximum_depth());
}

void
printer::print_states(const delphic::possibility_spectrum_ptr &W, const delphic::action_deque &as, const del::formula_ptr &goal,
                      const std::string &path, const std::string &name) {
    // todo: UNCOMMENT AND FIX
//    if (not delphic::union_updater::is_applicable(W, as.front())) {
//        std::cout << "Action '" << as.front()->get_name() << "' is not applicable" << std::endl;
//        return;
//    }

    auto W_ = delphic::union_updater::update(W, as.front());
    std::string new_name = name + "_" + as.front()->get_name();

    print_state(delphic::delphic_utils::convert(*W_), del::label_storage{}, path, new_name);
    delphic::action_deque as_ = as;
    as_.pop_front();

    // todo: UNCOMMENT AND FIX
//    if (W_->satisfies(goal))
//        std::cout << "Goal found after applying '" << as.front()->get_name() << "'" << std::endl;

    if (as_.empty())
        return;

    print_states(W_, as_, goal, path, new_name);
}

void printer::print_states(const search::planning_task &task, const kripke::action_deque &as, const std::string &path,
                           bool apply_contraction, kripke::contraction_type type, unsigned long k) {
//    print_states(*task.get_initial_state(), as, path + task.get_domain_name() +  "/" + task.get_problem_id() + "/product_update/",
//                 "s0", apply_contraction, type, k);
}

void printer::print_states(const search::delphic_planning_task &task, const delphic::action_deque &as,
                           const std::string &path) {
//    print_states(task.get_initial_state(), as, path, "W0");
}

void printer::print_task(const search::planning_task &task, del::storages_handler_ptr handler, const std::string &path) {
    std::string task_path = path + task.get_domain_name() + "/" + task.get_problem_id() + "/";

    print_state(*task.get_initial_state(), handler->get_label_storage(), task_path + "initial_state/", "s0");

    for (const action_ptr &a : task.get_actions())
        printer::print_action(*a,task_path + "actions/");
}

void printer::print_results(const search::planning_task &task, search::strategy strategy, contraction_type contraction_type, del::storages_handler_ptr handler, const std::string &out_path) {
    std::string out_task_path =
            out_path + "search/" + task.get_domain_name() + "/problem_" + task.get_problem_id() + "/";
    std::string strategy_str = strategy == search::strategy::unbounded_search
                               ? "unbounded/"
                               : "iter_bounded/";

    printer search_printer(true, out_task_path + strategy_str, "search_tree.txt");
    search::node_deque path = search::planner::search(task, strategy, contraction_type, handler,
                                                      std::make_unique<printer>(std::move(search_printer))).first;
    std::string state_name = "s0";

    for (const auto &node: path) {
        if (node->get_action())
            state_name += "_" + node->get_action()->get_name();

        if (not out_path.empty())
            daedalus::tester::printer::print_state(*node->get_state(), handler->get_label_storage(), out_task_path + strategy_str, state_name);
    }
}

void printer::print_delphic_results(const search::delphic_planning_task &task, search::strategy strategy,
                                    const std::string &out_path) {
    std::string out_task_path =
            out_path + "search/delphic/" + task.get_domain_name() + "/problem_" + task.get_problem_id() + "/";
    std::string strategy_str = strategy == search::strategy::unbounded_search
                               ? "unbounded/"
                               : "iter_bounded/";

    printer search_printer(true, out_task_path + strategy_str, "search_tree.txt");
    // todo: UNCOMMENT AND FIX
//    search::delphic_node_deque path = search::delphic_planner::search(task, strategy, std::make_unique<printer>(std::move(search_printer)));
//    std::string state_name = "s0";
//
//    for (const auto &node: path) {
//        if (node->get_action())
//            state_name += "_" + node->get_action()->get_name();
//
//        if (not out_path.empty())
//            daedalus::tester::printer::print_state(delphic::delphic_utils::convert(*node->get_state()), out_task_path + strategy_str, state_name);
//    }
}

void printer::print_domain_info(const search::planning_task &task, std::ofstream &table) {
    unsigned long atoms_no = task.get_language()->get_atoms_number();
    unsigned long agents_no = task.get_language()->get_agents_number();
    unsigned long actions_no = task.get_actions().size();
    unsigned long goal_depth = task.get_goal()->get_modal_depth();

    table
        << std::endl
        << task.get_domain_name() << ";"
        << task.get_problem_id() << ";"
        << atoms_no << ";"
        << agents_no << ";"
        << task.get_initial_state()->get_worlds_number() << ";"
        << actions_no << ";"
        << goal_depth << ";";
}

void printer::print_time_results(const search::planning_task &task, search::strategy strategy, contraction_type contraction_type, del::storages_handler_ptr handler, std::ofstream &table) {
    const auto &[_, stats] = search::planner::search(task, strategy, contraction_type, handler);

    if (strategy == search::strategy::iterative_bounded_search)
        table << std::to_string(stats.m_plan_bound) << ";";

    table
        << stats.m_plan_length << ";"
        << stats.m_visited_states_no << ";"
        << stats.m_visited_worlds_no << ";"
        << stats.m_computation_time;

    std::chrono::seconds pause(5);
    std::this_thread::sleep_for(pause);
}

void printer::print_delphic_time_results(const search::delphic_planning_task &task, search::strategy strategy, std::ofstream &table) {
    // todo: UNCOMMENT AND FIX
//    auto start = std::chrono::steady_clock::now();
//    search::delphic_node_deque path = search::delphic_planner::search(task, strategy);
//    auto end = std::chrono::steady_clock::now();
//    auto delta = since(start).count();
//
//    unsigned long plan_length = path.size() - 1;
//    double time = static_cast<double>(delta) / 1000;
//
//    table
//        << std::to_string(plan_length) << ";"
//        << path.back()->get_id() << ";"
//        << std::to_string(time);
//
//    std::chrono::seconds pause(10);
//    std::this_thread::sleep_for(pause);
}