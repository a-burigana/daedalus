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

#include "../tests/search_tester.h"
#include "../tests/action_tester.h"
#include "../tests/builder/domains/consecutive_numbers.h"
#include "../tests/printer.h"
#include "../tests/builder/domains/collaboration_communication.h"
#include "../tests/builder/domains/coin_in_the_box.h"
#include "../include/search/planner.h"
#include "../tests/bisimulation/bisimulation_tester.h"
#include "../tests/builder/state_builder.h"
#include "../include/del/semantics/kripke/bisimulation/bounded_identification.h"
#include "utils/storage.cpp"
#include "../tests/builder/domains/tiger.h"
#include "../tests/builder/domains/gossip.h"
#include "../include/utils/printer/formula_printer.h"
#include "../tests/builder/domains/grapevine.h"
#include "../tests/builder/domains/active_muddy_children.h"
#include "../include/search/delphic/delphic_planning_task.h"
#include "../include/del/semantics/delphic/delphic_utils.h"
#include "../include/del/semantics/delphic/update/union_updater.h"
#include "../include/search/delphic/delphic_planner.h"
#include "../tests/builder/domains/selective_communication.h"
#include <string>

#define OUT_PATH std::string{"../tests/out/"}

using namespace daedalus::tester;

#include <iostream>

void storage_test() {
    storage<int> int_storage;
    int x = 5, y = 5;

    int_storage.emplace(std::move(x));
    int_storage.emplace(std::move(y));

    state s1 = state_builder::build_test_state1();
    unsigned long k = 3;
    storage<delphic::possibility> storage;
    auto worlds_signatures = signature_matrix(1);

    for (auto &h_signatures: worlds_signatures)
        h_signatures = signature_vector(s1.get_worlds_number());

    bounded_identification::calculate_world_signature(s1, 3, 3, 0, storage, worlds_signatures);
    bounded_identification::calculate_world_signature(s1, 3, 5, 0, storage, worlds_signatures);

    signature_ptr sign_w3_0 = worlds_signatures[0][3];
    signature_ptr sign_w5_0 = worlds_signatures[0][5];

    assert(sign_w3_0->get_label() == sign_w5_0->get_label());
    assert(sign_w3_0->get_information_state(0) == sign_w5_0->get_information_state(0));
    assert(*sign_w3_0 == *sign_w5_0);
}

int main() {
//    const auto cc_task = collaboration_communication::build_task(2, 3, 2, 1);
//    const auto cc_task_ = delphic::delphic_utils::convert(cc_task);
//
//    daedalus::tester::printer::print_results(cc_task, search::strategy::iterative_bounded_search, OUT_PATH);
//    daedalus::tester::printer::print_results(cc_task, search::strategy::unbounded_search, OUT_PATH);
//    daedalus::tester::printer::print_delphic_results(cc_task_, search::strategy::unbounded_search, OUT_PATH);

//    const auto as = cc_task.get_actions({"left_a_11", "sense_a_in_room_1_box_1_10", "left_b_11", "sense_b_in_room_1_box_1_11"});
//
//    daedalus::tester::printer::print_state(*cc_task.get_initial_state(), OUT_PATH + "collaboration_communication/problem_2_3_2_g1/product_update/", "s0");
//    daedalus::tester::printer::print_states(*cc_task.get_initial_state(), as, OUT_PATH + "collaboration_communication/problem_2_3_2_g1/product_update/", "s0");
//
//    const auto as_ = cc_task_.get_actions({"left_a_11", "sense_a_in_room_1_box_1_10", "left_b_11", "sense_b_in_room_1_box_1_11"});
//    daedalus::tester::printer::print_states(cc_task_.get_initial_state(), as_, OUT_PATH + "delphic/collaboration_communication/problem_2_3_2_g1/union_update/", "s0");

//    auto W1 = delphic::union_updater::update(cc_task_.get_initial_state(), as_[0]);
//    auto W2 = delphic::union_updater::update(cc_task_.get_initial_state(), as_[0]);

//    search::planner::search(cb_task, search::strategy::iterative_bounded_search);
//    search::planner::search(cb_task, search::strategy::unbounded_search);

    for (const auto &task : coin_in_the_box::build_tasks()) {
        const auto &task_ = delphic_utils::convert(task);

        daedalus::tester::printer::print_results(task, search::strategy::iterative_bounded_search, OUT_PATH);
        daedalus::tester::printer::print_results(task, search::strategy::unbounded_search, OUT_PATH);
        search::delphic_planner::search(task_, search::strategy::unbounded_search);
    }

//    daedalus::tester::printer::print_delphic_results(cb_task_, search::strategy::unbounded_search, OUT_PATH);

//    daedalus::tester::printer::print_task(cb_task, OUT_PATH);

//    search_tester::print_coin_in_the_box_time_results();

//    search::planning_task sc_task = selective_communication::build_task(3, 5, 2);
//    search::delphic_planning_task sc_task_ = delphic::delphic_utils::convert(sc_task);

//    search::planner::search(sc_task, search::strategy::iterative_bounded_search);
//    search::delphic_planner::search(sc_task_, search::strategy::unbounded_search);

//    search::planning_task g_task = gossip::build_task(5, 2, 9);
//    search::delphic_planning_task g_task_ = delphic::delphic_utils::convert(g_task);

//    search::planner::search(g_task, search::strategy::iterative_bounded_search);
//    search::delphic_planner::search(g_task_, search::strategy::unbounded_search);

//    auto s0 = g_task.get_initial_state();
//    auto a0 = g_task.get_action("tell_0_1");
//    auto s1 = kripke::updater::product_update(*s0, *a0);
//
//    daedalus::tester::printer::print_state(*s0, OUT_PATH + "gossip/2_1_1/states/", "s0");
//    daedalus::tester::printer::print_state( s1, OUT_PATH + "gossip/2_1_1/states/", "s0_tell_0_1");
//
//    auto W0 = g_task_.get_initial_state();
//    auto E0 = g_task_.get_action("tell_0_1");
//    auto W1 = delphic::union_updater::update(W0, E0);
//
//    auto s0_ = delphic::delphic_utils::convert(*W0);
//    auto s1_ = delphic::delphic_utils::convert(*W1);
//
//    daedalus::tester::printer::print_state(s0_, OUT_PATH + "delphic/gossip/2_1_1/states/", "s0");
//    daedalus::tester::printer::print_state(s1_, OUT_PATH + "delphic/gossip/2_1_1/states/", "s0_tell_0_1");

//    daedalus::tester::printer::print_results(amc_task, search::strategy::iterative_bounded_search, OUT_PATH);
//    daedalus::tester::printer::print_results(amc_task, search::strategy::unbounded_search, OUT_PATH);
//    daedalus::tester::printer::print_task(amc_task, OUT_PATH);




//    search::planning_task gr_task = grapevine::build_task(6, 3, 3);
//    daedalus::tester::printer::print_task(gr_task, OUT_PATH);

//    search::planner::search(gr_task, search::strategy::iterative_bounded_search);
//    daedalus::tester::printer::print_results(gr_task, search::strategy::iterative_bounded_search, OUT_PATH);

//    search::planner::search(gr_task, search::strategy::unbounded_search);


//
//    auto actions = gossip::build_actions(3);
//
//    for (const action_ptr &a : actions)
//        printer::print_action(*a, OUT_PATH + "gossip/3/actions/");

//    search::planning_task tiger_task = tiger::build_task(5, 3);
//    printer::print_task(tiger_task, OUT_PATH);

//    printer::print_results(tiger_task, search::strategy::unbounded_search, OUT_PATH);
//    printer::print_results(tiger_task, search::strategy::iterative_bounded_search, OUT_PATH);



//    action_deque as = {tiger_task.get_action("listen_1"), tiger_task.get_action("right"), tiger_task.get_action("open_2"), tiger_task.get_action("look_2"), tiger_task.get_action("save_princess_2")};
//    printer::print_states(tiger_task, as, OUT_PATH, true, bisimulation_type::full);

//    printer::print_results(tiger_task, search::strategy::iterative_bounded_search, OUT_PATH);

//    state s = tiger::build_initial_state(4, 2);
//    printer::print_state(s, OUT_PATH + "states/tiger/4_2/", "s0");
//
//    for (const action_ptr &a : tiger_task.get_actions())
//        printer::print_action(*a, OUT_PATH + tiger_task.get_domain_name() + "/" + tiger_task.get_problem_id() + "/");


//    int n = 6;
//    auto cn_task = consecutive_numbers::build_task(n);

//    state_ptr s = cn_task.get_initial_state();
//    auto actions = cn_task.get_actions();
//
//    for (const auto &a : actions)
//        printer::print_action(*a, OUT_PATH + "actions/consecutive_numbers/" + std::to_string(n) + "/");

//    for (int n = 2; n <= 4; ++n) {
//        auto cn_task = consecutive_numbers::build_task(n);
//
//        printer::print_results(cn_task, search::strategy::unbounded_search, OUT_PATH);
//        printer::print_results(cn_task, search::strategy::iterative_bounded_search, OUT_PATH);
//    }

//    unsigned long b = 4;
//    const auto &[is_bisim, s_contr] = bisimulator::contract(bisimulation_type::rooted, *s, b);
//    assert(not is_bisim);
//
//    auto a = cn_task.get_actions({"a_not_knows_b_0"}); // "a_not_knows_b_3", "b_not_knows_a_4"
//    printer::print_state( s_contr,    OUT_PATH + "product_update/consecutive_numbers/" + std::to_string(n) + "/", "s0");
//    printer::print_states(s_contr, a, OUT_PATH + "product_update/consecutive_numbers/" + std::to_string(n) + "/", "s0" , true, bisimulation_type::rooted, b);



//    auto a = cn_task.get_actions({"b_not_knows_a_1", "a_not_knows_b_2", "b_not_knows_a_3"});
//    printer::print_state( *s,    OUT_PATH + "product_update/consecutive_numbers/" + std::to_string(n) + "/", "s0");
//    printer::print_states(*s, a, OUT_PATH + "product_update/consecutive_numbers/" + std::to_string(n) + "/", "s0", true, bisimulation_type::full);

//    bisimulation_tester::test_bisim_1(OUT_PATH, k, &storage);








    /*auto tasks = coin_in_the_box::build_tasks();
//    std::map<std::string, unsigned long> b_map;

    for (const auto &task : tasks) {
        search::planner::search(task, search::strategy::iterative_bounded_search);
        search::planner::search(task, search::strategy::unbounded_search);
//        b_map[task.get_problem_id()] = path.back()->get_bound();
    }*/


//
////    for (const auto &[id, b] : b_map)
////        std::cout << id << " & " << b << std::endl;

//    action_tester::test_switches_actions(5, OUT_PATH);

//
    return 0;
}
