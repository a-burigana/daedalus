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

void printer::print_states(const kripke::state &s, const kripke::action_deque &as, const std::string &path,
                           const std::string &name, bool apply_contraction, kripke::bisimulation_type type,
                           const unsigned long k) {
    kripke::state s_ = kripke::updater::product_update(s, *as.front());
    std::string new_name = name + "_" + as.front()->get_name();

    if (apply_contraction)
        s_ = kripke::bisimulator::contract(type, s_, k).second;

    print_state(s_, path, new_name);
    kripke::action_deque as_ = as;
    as_.pop_front();

    if (as_.empty())
        return;

    print_states(s_, as_, path, new_name, apply_contraction, type, k);
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

void printer::print_time_results(const search::planning_task &task, std::ofstream &table) {
    unsigned long atoms_no = task.get_language()->get_atoms_number();
    unsigned long agents_no = task.get_language()->get_agents_number();
    unsigned long actions_no = task.get_actions().size();
    unsigned long goal_depth = task.get_goal()->get_modal_depth();

    table
        << task.get_problem_id() << ";"
        << atoms_no << ";" << agents_no << ";"
        << task.get_initial_state()->get_worlds_number() << ";" << actions_no << ";" << goal_depth << ";";

    std::chrono::seconds pause(5);

    auto start = std::chrono::steady_clock::now();
    search::node_deque path_IBDS = search::planner::search(task, search::strategy::iterative_bounded_search);
    auto end = std::chrono::steady_clock::now();
    auto delta_IBDS = since(start).count();

    unsigned long plan_length_IBDS = path_IBDS.size() - 1;
    double time_IBDS = static_cast<double>(delta_IBDS) / 1000;

    table
        << std::to_string(plan_length_IBDS) << ";"
        << path_IBDS.back()->get_id() << ";"
        << std::to_string(time_IBDS) << ";";

    std::this_thread::sleep_for(pause);

    start = std::chrono::steady_clock::now();
    search::node_deque path_US = search::planner::search(task, search::strategy::unbounded_search);
    end = std::chrono::steady_clock::now();
    auto delta_US = since(start).count();

    unsigned long plan_length_US = path_US.size() - 1;
    double time_US = static_cast<double>(delta_US) / 1000;

    std::this_thread::sleep_for(pause);

    table
        << std::to_string(plan_length_US) << ";"
        << path_US.back()->get_id() << ";"
        << std::to_string(time_US) << std::endl;
}
