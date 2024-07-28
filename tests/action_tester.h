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

#ifndef BEP_ACTION_TESTER_H
#define BEP_ACTION_TESTER_H

#include <string>

namespace bep::tester {
    class action_tester {
    public:
        static void test_coin_in_the_box_actions(const std::string &out_path);
        static void test_consecutive_numbers_actions(unsigned long n, const std::string &out_path);
        static void test_switches_actions(unsigned long n, const std::string &out_path);
        static void test_selective_communication_actions(unsigned long agents_no, unsigned long rooms_no, const std::string &out_path);
        static void test_collaboration_communication_actions(unsigned long agents_no, unsigned long rooms_no, unsigned long boxes_no, const std::string &out_path);
    };
}

#endif //BEP_ACTION_TESTER_H
