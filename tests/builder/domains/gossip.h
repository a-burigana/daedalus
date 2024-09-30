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

#ifndef DAEDALUS_GOSSIP_H
#define DAEDALUS_GOSSIP_H

#include "../../../include/del/language/language.h"
#include "../../../include/del/semantics/kripke/states/state.h"
#include "../../../include/search/planning_task.h"

namespace daedalus::tester {
    class gossip {
    public:
        static std::string get_name();

        static del::language_ptr build_language(unsigned long agents_no, unsigned long secrets_no);
        static kripke::state build_initial_state(unsigned long agents_no, unsigned long secrets_no);

        static kripke::action_deque build_actions(unsigned long agents_no, unsigned long secrets_no);
        static search::planning_task build_task(unsigned long agents_no, unsigned long secrets_no, unsigned long goal_id);
        static std::vector<search::planning_task> build_tasks();

        static del::formula_ptr build_goal(unsigned long agents_no, unsigned long secrets_no, unsigned long goal_id);
    private:
        static kripke::action build_tell(unsigned long agents_no, unsigned long secrets_no, del::agent ag_1, del::agent ag_2);

    };
}

#endif //DAEDALUS_GOSSIP_H
