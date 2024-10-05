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

void printer::print_state(const kripke::state &s, const std::string &path, const std::string &name) {
    if (not std::filesystem::exists(path))
        std::filesystem::create_directories(path);

    std::string dot_file_name = path + name + ".dot";
    std::string pdf_file_name = path + name + ".pdf";
    std::ofstream dot_file = std::ofstream{dot_file_name};

    dot_file << s;
    dot_file.close();

    std::string command = "dot -Tpdf " + dot_file_name + " > " + pdf_file_name;
    system(command.c_str());
    command = "rm " + dot_file_name;
    system(command.c_str());

    dot_file.close();
}

void printer::print_state(const delphic::possibility_spectrum &W, const std::string &path, const std::string &name) {
    const state s = delphic_utils::convert(W);
    print_state(s, path, name);
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

void printer::print_states(const kripke::state &s, const kripke::action_deque &as, const del::formula_ptr &goal,
                           const std::string &path, const std::string &name, bool apply_contraction, kripke::bisimulation_type type) {
    if (not kripke::updater::is_applicable(s, *as.front())) {
        std::cout << "Action '" << as.front()->get_name() << "' is not applicable" << std::endl;
        return;
    }

    kripke::state s_ = kripke::updater::product_update(s, *as.front());
    std::string new_name = name + "_" + as.front()->get_name();

    if (apply_contraction)
        s_ = kripke::bisimulator::contract(type, s_, goal->get_modal_depth()).second;

    print_state(s_, path, new_name);
    kripke::action_deque as_ = as;
    as_.pop_front();

    if (s_.satisfies(goal))
        std::cout << "Goal found after applying '" << as.front()->get_name() << "'" << std::endl;

    if (as_.empty())
        return;

    print_states(s_, as_, goal, path, new_name, apply_contraction, type);
}

void
printer::print_states(const delphic::possibility_spectrum_ptr &W, const delphic::action_deque &as, const del::formula_ptr &goal,
                      const std::string &path, const std::string &name) {
    if (not delphic::union_updater::is_applicable(W, as.front())) {
        std::cout << "Action '" << as.front()->get_name() << "' is not applicable" << std::endl;
        return;
    }

    auto W_ = delphic::union_updater::update(W, as.front());
    std::string new_name = name + "_" + as.front()->get_name();

    print_state(delphic::delphic_utils::convert(*W_), path, new_name);
    delphic::action_deque as_ = as;
    as_.pop_front();

    if (W_->satisfies(goal))
        std::cout << "Goal found after applying '" << as.front()->get_name() << "'" << std::endl;

    if (as_.empty())
        return;

    print_states(W_, as_, goal, path, new_name);
}

void printer::print_states(const search::planning_task &task, const kripke::action_deque &as, const std::string &path,
                           bool apply_contraction, kripke::bisimulation_type type, unsigned long k) {
//    print_states(*task.get_initial_state(), as, path + task.get_domain_name() +  "/" + task.get_problem_id() + "/product_update/",
//                 "s0", apply_contraction, type, k);
}

void printer::print_states(const search::delphic_planning_task &task, const delphic::action_deque &as,
                           const std::string &path) {
//    print_states(task.get_initial_state(), as, path, "W0");
}

void printer::print_task(const search::planning_task &task, const std::string &path) {
    std::string task_path = path + task.get_domain_name() + "/" + task.get_problem_id() + "/";

    print_state(*task.get_initial_state(), task_path + "initial_state/", "s0");

    for (const action_ptr &a : task.get_actions())
        printer::print_action(*a,task_path + "actions/");
}

void printer::print_results(const search::planning_task &task, search::strategy strategy, const std::string &out_path) {
    std::string out_task_path =
            out_path + "search/" + task.get_domain_name() + "/problem_" + task.get_problem_id() + "/";
    std::string strategy_str = strategy == search::strategy::unbounded_search
                               ? "unbounded/"
                               : "iter_bounded/";

    printer search_printer(true, out_task_path + strategy_str, "search_tree.txt");
    search::node_deque path = search::planner::search(task, strategy,
                                                      std::make_unique<printer>(std::move(search_printer)));
    std::string state_name = "s0";

    for (const auto &node: path) {
        if (node->get_action())
            state_name += "_" + node->get_action()->get_name();

        if (not out_path.empty())
            daedalus::tester::printer::print_state(*node->get_state(), out_task_path + strategy_str, state_name);
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
    search::delphic_node_deque path = search::delphic_planner::search(task, strategy, std::make_unique<printer>(std::move(search_printer)));
    std::string state_name = "s0";

    for (const auto &node: path) {
        if (node->get_action())
            state_name += "_" + node->get_action()->get_name();

        if (not out_path.empty())
            daedalus::tester::printer::print_state(delphic::delphic_utils::convert(*node->get_state()), out_task_path + strategy_str, state_name);
    }
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
        << atoms_no << ";" << agents_no << ";"
        << task.get_initial_state()->get_worlds_number() << ";" << actions_no << ";" << goal_depth << ";";
}

void printer::print_time_results(const search::planning_task &task, search::strategy strategy, std::ofstream &table) {
    auto start = std::chrono::steady_clock::now();
    search::node_deque path = search::planner::search(task, strategy);
    auto end = std::chrono::steady_clock::now();
    auto delta = since(start).count();

    unsigned long plan_length = path.size() - 1;
    double time = static_cast<double>(delta) / 1000;

    if (strategy == search::strategy::iterative_bounded_search)
        table << std::to_string(path.back()->get_bound()) << ";";

    table
        << std::to_string(plan_length) << ";"
        << path.back()->get_id() << ";"
        << std::to_string(time);

    std::chrono::seconds pause(10);
    std::this_thread::sleep_for(pause);
}

void printer::print_delphic_time_results(const search::delphic_planning_task &task, search::strategy strategy, std::ofstream &table) {
    auto start = std::chrono::steady_clock::now();
    search::delphic_node_deque path = search::delphic_planner::search(task, strategy);
    auto end = std::chrono::steady_clock::now();
    auto delta = since(start).count();

    unsigned long plan_length = path.size() - 1;
    double time = static_cast<double>(delta) / 1000;

    table
        << std::to_string(plan_length) << ";"
        << path.back()->get_id() << ";"
        << std::to_string(time);

    std::chrono::seconds pause(10);
    std::this_thread::sleep_for(pause);
}