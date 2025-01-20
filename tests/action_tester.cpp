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

#include "action_tester.h"
#include "builder/domains/coin_in_the_box.h"
#include "builder/domains/collaboration_communication.h"
#include "builder/domains/consecutive_numbers.h"
#include "builder/domains/selective_communication.h"
#include "builder/domains/switches.h"
#include "builder/language_builder.h"
#include <string>

using namespace daedalus::tester;

void action_tester::test_coin_in_the_box_actions(const std::string &out_path) {
    auto actions = coin_in_the_box::build_actions();

    for (const kripke::action_ptr &a: actions)
        printer::print_action(*a, out_path + "actions/" + coin_in_the_box::get_name() + "/");
}

void action_tester::test_consecutive_numbers_actions(unsigned long n, const std::string &out_path) {
    auto actions = consecutive_numbers::build_actions(n);

    for (const kripke::action_ptr &a: actions)
        printer::print_action(*a, out_path + "actions/" + consecutive_numbers::get_name() + "/" + std::to_string(n) + "/");
}

void action_tester::test_switches_actions(unsigned long n, const std::string &out_path) {
    auto actions = switches::build_actions(n);

    for (const kripke::action_ptr &a: actions)
        printer::print_action(*a, out_path + "actions/" + switches::get_name() + "/" + std::to_string(n) + "/");
}

void action_tester::test_selective_communication_actions(unsigned long agents_no, unsigned long rooms_no, const std::string &out_path) {
    auto actions = selective_communication::build_actions(agents_no, rooms_no);

    for (const kripke::action_ptr &a: actions)
        printer::print_action(*a, out_path + "actions/" + selective_communication::get_name() + "/" + std::to_string(agents_no) + "_" + std::to_string(rooms_no) + "/");
}

void action_tester::test_collaboration_communication_actions(unsigned long agents_no, unsigned long rooms_no,
                                                             unsigned long boxes_no, const std::string &out_path) {
    auto actions = collaboration_communication::build_actions(agents_no, rooms_no, boxes_no);

    for (const kripke::action_ptr &a: actions)
        printer::print_action(*a, out_path + "actions/" + collaboration_communication::get_name() + "/" + std::to_string(agents_no) + "_" + std::to_string(rooms_no) + "_" + std::to_string(boxes_no) + "/");
}
