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

#ifndef DAEDALUS_SELECTIVE_COMMUNICATION_H
#define DAEDALUS_SELECTIVE_COMMUNICATION_H

#include "../../../include/del/semantics/kripke/actions/action.h"
#include "../../../include/del/del_types.h"
#include "../../../include/del/formulas/formula.h"
#include "../../../include/del/language/language.h"
#include "../../../include/search/planning_task.h"

namespace daedalus::tester {
    class selective_communication {
    public:
        static std::string get_name();

        static del::language_ptr build_language(unsigned long agents_no, unsigned long rooms_no);
        static kripke::state build_initial_state(unsigned long agents_no, unsigned long rooms_no, const del::label_storage_ptr &l_storage);

        static kripke::action_deque build_actions(unsigned long agents_no, unsigned long rooms_no);
        static search::planning_task build_task(unsigned long agents_no, unsigned long rooms_no, unsigned long goal_id, const del::label_storage_ptr &l_storage);
        static std::vector<search::planning_task> build_tasks(const del::label_storage_ptr &l_storage);

    private:
        static kripke::action build_left(unsigned long agents_no, unsigned long rooms_no, del::agent ag, const del::agent_set &fo_ags);
        static kripke::action build_right(unsigned long agents_no, unsigned long rooms_no, del::agent ag, const del::agent_set &fo_ags);

        static kripke::action build_sense(unsigned long agents_no, unsigned long rooms_no, del::agent ag, const del::agent_set &po_ags);
        static kripke::action build_tell(unsigned long agents_no, unsigned long rooms_no, unsigned long r, del::agent ag, const del::agent_set &fo_ags);

        static del::formula_ptr build_goal(const del::language_ptr &language, unsigned long goal_id);

        static del::formula_deque in_room_agents(const del::language_ptr &language, unsigned long r, const del::agent_set &ags);
    };
}

#endif //DAEDALUS_SELECTIVE_COMMUNICATION_H
