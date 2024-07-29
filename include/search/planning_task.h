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

#ifndef DAEDALUS_PLANNING_TASK_H
#define DAEDALUS_PLANNING_TASK_H

#include <set>
#include "../del/semantics/kripke/states/state.h"
#include "../del/semantics/kripke/actions/action.h"
#include "../del/formulas/formula.h"
#include "../del/language/language.h"

namespace search {
    class planning_task;
    using planning_task_ptr = std::unique_ptr<planning_task>;

    class planning_task {
    public:
        planning_task(std::string domain_name, std::string problem_id, del::language_ptr language,
                      kripke::state initial_state, kripke::action_deque actions, del::formula_ptr goal);

        planning_task(const planning_task&) = delete;
        planning_task& operator=(const planning_task&) = delete;

        planning_task(planning_task&&) = default;
        planning_task& operator=(planning_task&&) = default;

        ~planning_task() = default;

        [[nodiscard]] std::string get_domain_name() const;
        [[nodiscard]] std::string get_problem_id() const;
        [[nodiscard]] unsigned long get_maximum_depth() const;

        [[nodiscard]] del::language_ptr get_language() const;
        [[nodiscard]] kripke::state_ptr get_initial_state() const;
        [[nodiscard]] const kripke::action_deque &get_actions() const;
        [[nodiscard]] del::formula_ptr get_goal() const;

        [[nodiscard]] const kripke::action_ptr &get_action(const std::string &name) const;
        [[nodiscard]] kripke::action_deque get_actions(const std::deque<std::string> &names) const;

    private:
        std::string m_domain_name, m_problem_id;
        unsigned long m_maximum_depth;

        del::language_ptr m_language;
        kripke::state_ptr m_initial_state;
        kripke::action_deque m_actions;
        del::formula_ptr m_goal;

        std::map<std::string, kripke::action_ptr> m_actions_map;

        void init_maximum_depth();
        void init_actions_map();
    };
}

#endif //DAEDALUS_PLANNING_TASK_H
