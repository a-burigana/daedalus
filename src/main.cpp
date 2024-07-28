//
// BEP - Bounded Epistemic Planner (MIT License)
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

#define OUT_PATH std::string{"../tests/out/"}

using namespace bep::tester;

#include <iostream>

int main() {
    auto tasks = collaboration_communication::build_tasks();
    std::map<std::string, unsigned long> b_map;

    for (const auto &task : tasks) {
        auto path = search::planner::search(task, search::strategy::iterative_bounded_search);
        b_map[task.get_problem_id()] = path.back()->get_bound();
    }

    for (const auto &[id, b] : b_map)
        std::cout << id << " & " << b << std::endl;

//    action_tester::test_switches_actions(5, OUT_PATH);

//    auto cn_task = consecutive_numbers::build_task(2);
//    auto actions = cn_task.get_actions({"a_tells_minus_0"});
//    auto actions = cn_task.get_actions({"b_not_knows_a_1", "a_not_knows_b_2", "b_not_knows_a_3", "a_tells_minus_3"});
//    printer::print_state(*cn_task.get_initial_state(), OUT_PATH + "product_update/" + consecutive_numbers::get_name() + "/" + cn_task.get_problem_id() + "/", "s0");
//    printer::print_states(*cn_task.get_initial_state(), actions, OUT_PATH + "product_update/" + consecutive_numbers::get_name() + "/" + cn_task.get_problem_id() + "/", "s0");

//    assert(cn_task.get_initial_state()->satisfies(actions.front()->get_precondition(0)));

//    search_tester::run_coin_in_the_box_search_tests();
//    search_tester::print_consecutive_numbers_search_tests();

//    search_tester::print_coin_in_the_box_time_results();
//    search_tester::print_consecutive_numbers_time_results();
//    search_tester::print_selective_communication_time_results();
//    search_tester::print_collaboration_communication_time_results();

    return 0;
}
