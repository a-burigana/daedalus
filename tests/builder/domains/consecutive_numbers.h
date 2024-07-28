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

#ifndef BEP_CONSECUTIVE_NUMBERS_H
#define BEP_CONSECUTIVE_NUMBERS_H

#include "../../../include/del/language/language.h"
#include "../../../include/del/states/state.h"
#include "../../../include/search/planning_task.h"

namespace daedalus::tester {
    class consecutive_numbers {
    public:
        static std::string get_name();

        static del::language_ptr build_language(unsigned long n);
        static del::state build_initial_state(unsigned long n);

        static del::action_deque build_actions(unsigned long n);
        static search::planning_task build_task(unsigned long n);
        static std::vector<search::planning_task> build_tasks();
    };
}

#endif //BEP_CONSECUTIVE_NUMBERS_H
