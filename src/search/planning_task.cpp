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

#include "../../include/search/planning_task.h"
#include <memory>
#include <utility>

using namespace search;

planning_task::planning_task(std::string domain_name, std::string problem_id, del::language_ptr language,
                             del::state initial_state, del::action_deque actions, del::formula_ptr goal) :
         m_domain_name{std::move(domain_name)},
         m_problem_id{std::move(problem_id)},
         m_language{std::move(language)},
         m_initial_state{std::make_shared<del::state>(std::move(initial_state))},
         m_actions{std::move(actions)},
         m_goal{std::move(goal)} {
    init_actions_map();
    init_maximum_depth();
}

void planning_task::init_maximum_depth() {
    m_maximum_depth = 0;
    
    for (const del::action_ptr &a : m_actions)
        if (a->get_maximum_depth() > m_maximum_depth)
            m_maximum_depth = a->get_maximum_depth();
}

void planning_task::init_actions_map() {
    for (const del::action_ptr &a : m_actions)
        m_actions_map[a->get_name()] = a;
}

const del::action_ptr &planning_task::get_action(const std::string &name) const {
    return m_actions_map.at(name);
}

del::action_deque planning_task::get_actions(const std::deque<std::string> &names) const {
    del::action_deque as;

    for (const std::string &name : names)
        as.push_back(get_action(name));

    return as;
}

std::string planning_task::get_domain_name() const {
    return m_domain_name;
}

std::string planning_task::get_problem_id() const {
    return m_problem_id;
}

unsigned long planning_task::get_maximum_depth() const {
    return m_maximum_depth;
}

del::language_ptr planning_task::get_language() const {
    return m_language;
}

del::state_ptr planning_task::get_initial_state() const {
    return m_initial_state;
}

const del::action_deque &planning_task::get_actions() const {
    return m_actions;
}

del::formula_ptr planning_task::get_goal() const {
    return m_goal;
}
