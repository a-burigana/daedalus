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

#include "search_tester.h"
#include <iostream>
#include <string>
#include "../include/del/language/language.h"
#include "../include/del/semantics/kripke/states/state.h"
#include "../include/del/semantics/kripke/update/updater.h"
#include "../tests/builder/state_builder.h"
#include "../tests/formula_tester.h"
#include "../tests/update_tester.h"
#include "../tests/printer.h"
#include "bisimulation/bisimulation_tester.h"
#include "../tests/action_tester.h"
#include "../include/search/planner.h"
#include "../tests/builder/domains/coin_in_the_box.h"
#include "../tests/builder/domains/consecutive_numbers.h"
#include "../tests/builder/domains/selective_communication.h"
#include "../tests/builder/domains/collaboration_communication.h"
#include "builder/domains/switches.h"

#define OUT_PATH std::string{"../tests/out/"}

using namespace daedalus::tester;
using namespace del;
using namespace search;


void search_tester::run_formulas_tests() {
    formula_tester::test_CB_1();
    formula_tester::test_CB_2();
    formula_tester::test_CB_3();
}

void search_tester::run_actions_tests() {
    std::string cb_dir = coin_in_the_box::get_name() + "/";
    action_tester::test_coin_in_the_box_actions(OUT_PATH + "actions/" + cb_dir);

    std::string cn_dir = consecutive_numbers::get_name() + "/";
    action_tester::test_consecutive_numbers_actions(2, OUT_PATH + "actions/" + cn_dir);
    action_tester::test_consecutive_numbers_actions(5, OUT_PATH + "actions/" + cn_dir);
    action_tester::test_consecutive_numbers_actions(10, OUT_PATH + "actions/" + cn_dir);

    std::string sc_dir = selective_communication::get_name() + "/";
    action_tester::test_selective_communication_actions(2, 3, OUT_PATH + "actions/" + sc_dir);
    action_tester::test_selective_communication_actions(2, 4, OUT_PATH + "actions/" + sc_dir);
    action_tester::test_selective_communication_actions(3, 4, OUT_PATH + "actions/" + sc_dir);

    std::string cc_dir = collaboration_communication::get_name() + "/";
    action_tester::test_collaboration_communication_actions(2, 3, 2, OUT_PATH + "actions/" + cc_dir);
    action_tester::test_collaboration_communication_actions(3, 3, 2, OUT_PATH + "actions/" + cc_dir);
    action_tester::test_collaboration_communication_actions(3, 4, 3, OUT_PATH + "actions/" + cc_dir);
}

void search_tester::run_product_update_tests() {
    const state s_cb = coin_in_the_box::build_initial_state();
    std::string cb_dir = "Coin_in_the_Box/";
    printer::print_state(s_cb, OUT_PATH + "product_update/" + cb_dir, "coin_in_the_box");

    update_tester::test_CB_1(OUT_PATH + "product_update/");
    update_tester::test_CB_2(OUT_PATH + "product_update/");
    update_tester::test_CB_3(OUT_PATH + "product_update/");
}

void search_tester::run_contractions_tests() {
    bisimulation_tester::test_bisim_1                (OUT_PATH + "contractions/", 3);

    bisimulation_tester::test_bisim_singleton        (OUT_PATH + "contractions/", 1);
    bisimulation_tester::test_bisim_singleton_no_loop(OUT_PATH + "contractions/", 1);

    bisimulation_tester::test_bisim_chain            (OUT_PATH + "contractions/", 3, 3);
    bisimulation_tester::test_bisim_chain_no_loop    (OUT_PATH + "contractions/", 3, 3);

    bisimulation_tester::test_bisim_chain            (OUT_PATH + "contractions/", 3, 3, true);
    bisimulation_tester::test_bisim_chain_no_loop    (OUT_PATH + "contractions/", 3, 3, true);

    bisimulation_tester::test_bisim_k_tree           (OUT_PATH + "contractions/", 3);
    bisimulation_tester::test_bisim_k_tree           (OUT_PATH + "contractions/", 4);
    bisimulation_tester::test_bisim_k_tree           (OUT_PATH + "contractions/", 5);
    bisimulation_tester::test_bisim_k_tree           (OUT_PATH + "contractions/", 10);

    bisimulation_tester::test_bisim_cn               (OUT_PATH + "contractions/", 5, 2);
    bisimulation_tester::test_bisim_cn               (OUT_PATH + "contractions/", 5, 3);
    bisimulation_tester::test_bisim_cn               (OUT_PATH + "contractions/", 5, 4);
    bisimulation_tester::test_bisim_cn               (OUT_PATH + "contractions/", 5, 5);
}

void search_tester::run_search_tests(const std::vector<planning_task> &tasks) {
    for (const planning_task &task : tasks) {
        planner::search(task, search::strategy::iterative_bounded_search);
        planner::search(task, search::strategy::unbounded_search);
    }
}

void search_tester::run_coin_in_the_box_search_tests() {
    auto tasks = coin_in_the_box::build_tasks();
    run_search_tests(tasks);
}

void search_tester::run_consecutive_numbers_search_tests() {
    auto tasks = consecutive_numbers::build_tasks();
    run_search_tests(tasks);
}

void search_tester::run_selective_communication_search_tests() {
    auto tasks = selective_communication::build_tasks();
    run_search_tests(tasks);
}

void search_tester::run_collaboration_communication_search_tests() {
    auto tasks = collaboration_communication::build_tasks();
    run_search_tests(tasks);
}

void search_tester::print_search_results(const std::vector<planning_task> &tasks) {
    for (const planning_task &task : tasks) {
        printer::print_results(task, search::strategy::iterative_bounded_search, OUT_PATH);
        printer::print_results(task, search::strategy::unbounded_search, OUT_PATH);
    }
}

void search_tester::print_coin_in_the_box_search_tests() {
    auto tasks = coin_in_the_box::build_tasks();
    print_search_results(tasks);
}

void search_tester::print_consecutive_numbers_search_tests() {
    auto tasks = consecutive_numbers::build_tasks();
    print_search_results(tasks);
}

void search_tester::print_selective_communication_search_tests() {
    auto tasks = selective_communication::build_tasks();
    print_search_results(tasks);
}

void search_tester::print_collaboration_communication_search_tests() {
    auto tasks = collaboration_communication::build_tasks();
    print_search_results(tasks);
}

void search_tester::print_time_results(const std::vector<planning_task> &tasks, const std::string &table_path) {
    if (not std::filesystem::exists(table_path))
        std::filesystem::create_directories(table_path);

    std::string table_name = table_path + "results.csv";
    std::ofstream table = std::ofstream{table_name};

    table << "Problem ID;#Atoms;#Agents;|W|;#Actions;Goal depth;Plan length (IBDS);#Nodes (IBDS);Time (IBDS);Plan length (US);#Nodes (US);Time (US)" << std::endl;

    for (const planning_task &task : tasks)
        printer::print_time_results(task, table);

    table.close();
}

void search_tester::print_coin_in_the_box_time_results() {
    std::string table_path = OUT_PATH + "time_results/" + coin_in_the_box::get_name() + "/";
    auto tasks = coin_in_the_box::build_tasks();
    print_time_results(tasks, table_path);
}

void search_tester::print_consecutive_numbers_time_results() {
    std::string table_path = OUT_PATH + "time_results/" + consecutive_numbers::get_name() + "/";
    auto tasks = consecutive_numbers::build_tasks();
    print_time_results(tasks, table_path);
}

void search_tester::print_switches_time_results() {
    std::string table_path = OUT_PATH + "time_results/" + switches::get_name() + "/";
    auto tasks = switches::build_tasks();
    print_time_results(tasks, table_path);
}

void search_tester::print_selective_communication_time_results() {
    std::string table_path = OUT_PATH + "time_results/" + selective_communication::get_name() + "/";
    auto tasks = selective_communication::build_tasks();
    print_time_results(tasks, table_path);
}


void search_tester::print_collaboration_communication_time_results() {
    std::string table_path = OUT_PATH + "time_results/" + collaboration_communication::get_name() + "/";
    auto tasks = collaboration_communication::build_tasks();
    print_time_results(tasks, table_path);
}
