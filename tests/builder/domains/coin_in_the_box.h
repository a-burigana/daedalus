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

#ifndef DAEDALUS_COIN_IN_THE_BOX_H
#define DAEDALUS_COIN_IN_THE_BOX_H

#include "../../../include/del/semantics/kripke/actions/action.h"
#include "../../../include/del/del_types.h"
#include "../../../include/del/formulas/formula.h"
#include "../../../include/del/language/language.h"
#include "../../../include/search/planning_task.h"

namespace daedalus::tester {
    class coin_in_the_box {
    public:
        static std::string get_name();

        static del::language_ptr build_language();
        static kripke::state build_initial_state(const del::label_storage_ptr &l_storage);

        static kripke::action_deque build_actions();
        static search::planning_task build_task(unsigned long id, const del::label_storage_ptr &l_storage);
        static std::vector<search::planning_task> build_tasks(const del::label_storage_ptr &l_storage);

        static kripke::action build_open(del::agent ag, const del::agent_set &fo_ags);
        static kripke::action build_peek(del::agent ag, const del::agent_set &po_ags);
        static kripke::action build_shout(del::agent ag, const del::agent_set &fo_ags);
        static kripke::action build_walk_in(del::agent ag, const del::agent_set &fo_ags);
        static kripke::action build_walk_out(del::agent ag, const del::agent_set &fo_ags);

    private:
        static search::planning_task build_task_1(const del::label_storage_ptr &l_storage);
        static search::planning_task build_task_2(const del::label_storage_ptr &l_storage);
        static search::planning_task build_task_3(const del::label_storage_ptr &l_storage);
        static search::planning_task build_task_4(const del::label_storage_ptr &l_storage);
        static search::planning_task build_task_5(const del::label_storage_ptr &l_storage);
        static search::planning_task build_task_6(const del::label_storage_ptr &l_storage);

        static del::formula_deque looking_agents(const del::language_ptr &language, const del::agent_set &ags);
        static del::formula_deque B_looking_agents(const del::language_ptr &language, const del::agent_set &ags,
                                                   del::formula_deque &fs);
    };
}

#endif //DAEDALUS_COIN_IN_THE_BOX_H
