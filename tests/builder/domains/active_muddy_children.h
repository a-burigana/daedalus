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

#ifndef DAEDALUS_ACTIVE_MUDDY_CHILDREN_H
#define DAEDALUS_ACTIVE_MUDDY_CHILDREN_H

#include "../../../include/del/language/language.h"
#include "../../../include/del/semantics/kripke/states/state.h"
#include "../../../include/search/planning_task.h"

namespace daedalus::tester {
    class active_muddy_children {
    public:
        static std::string get_name();

        static del::language_ptr build_language(unsigned long children_no, unsigned long muddy_no);
        static kripke::state build_initial_state(unsigned long children_no, unsigned long muddy_no, bool is_0_muddy, const del::label_storage_ptr &l_storage);

        static kripke::action_deque build_actions(unsigned long children_no, unsigned long muddy_no);
        static search::planning_task build_task(unsigned long children_no, unsigned long muddy_no, bool is_0_muddy, const del::label_storage_ptr &l_storage);
        static std::vector<search::planning_task> build_tasks(const del::label_storage_ptr &l_storage);

    private:
        static kripke::action build_ask(unsigned long children_no, unsigned long muddy_no, del::agent ag);
    };
}

#endif //DAEDALUS_ACTIVE_MUDDY_CHILDREN_H
