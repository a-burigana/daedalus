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

#include "switches.h"
#include "../../../include/del/formulas/propositional/atom_formula.h"
#include "../../../include/del/formulas/propositional/not_formula.h"
#include "../../../include/del/del_types.h"
#include "../action_builder.h"
#include "../../../include/del/formulas/propositional/true_formula.h"
#include "../../../include/del/formulas/propositional/and_formula.h"
#include <memory>
#include <string>

using namespace daedalus::tester;
using namespace del;

std::string switches::get_name() {
    return "switches";
}

del::language_ptr switches::build_language(unsigned long n) {
    name_vector atom_names, agent_names(n + 1);

    for (unsigned long i = 1; i <= n; ++i) {
//        atom_names.emplace_back("in_room_a" + std::to_string(i));
        atom_names.emplace_back("is_on_s_" + std::to_string(i));
    }

    for (unsigned long i = 0; i <= n; ++i)
        agent_names[i] = "a" + std::to_string(i);

    return std::make_shared<language>(std::move(language{atom_names, agent_names}));
}

kripke::state switches::build_initial_state(unsigned long n, label_storage &l_storage) {
    language_ptr language = switches::build_language(n);

    world_id worlds_number = 1;
    world_bitset designated_worlds = world_bitset{worlds_number, world_set{0}};

    relations r(language->get_agents_number());

    for (agent ag = 0; ag < language->get_agents_number(); ++ag) {
        r[ag] = agent_relation(worlds_number);

        r[ag][0] = world_bitset(worlds_number, world_set{0});
    }

    label_vector ls = label_vector(worlds_number);
    boost::dynamic_bitset<> b0(language->get_atoms_number());
    label l0 = label{std::move(b0)};
    ls[0] = l_storage.emplace(std::move(l0));

    return state{language, worlds_number, std::move(r), std::move(ls), std::move(designated_worlds)};
}

kripke::action_deque switches::build_actions(unsigned long n) {
    language_ptr language = switches::build_language(n);
    action_deque actions;

    for (unsigned long i = 1; i <= n; ++i) {
        atom is_on_s_i = language->get_atom_id("is_on_s_" + std::to_string(i));
        formula_ptr f_is_on_s_i  = std::make_shared<atom_formula>(is_on_s_i);
        formula_ptr f_is_off_s_i = std::make_shared<not_formula>(f_is_on_s_i);

        event_post e_post;
        e_post[is_on_s_i] = std::make_shared<true_formula>();

        agent_set fo_ags(language->get_agents_number());
        fo_ags[0] = true;
        fo_ags[i] = true;

        action a = action_builder::build_private_ontic("switch_on_" + std::to_string(i), language, f_is_off_s_i, e_post, fo_ags);
        actions.emplace_back(std::make_shared<action>(std::move(a)));
    }

    return actions;
}

search::planning_task switches::build_task(unsigned long n, label_storage &l_storage) {
    language_ptr language = switches::build_language(n);

    state s0 = switches::build_initial_state(n, l_storage);
    action_deque actions = switches::build_actions(n);

    formula_deque fs;

    for (unsigned long i = 1; i <= n; ++i) {
        atom is_on_s_i = language->get_atom_id("is_on_s_" + std::to_string(i));
        fs.push_back(std::make_shared<atom_formula>(is_on_s_i));
    }

    formula_ptr goal = std::make_shared<and_formula>(std::move(fs));

    return search::planning_task{switches::get_name(), std::to_string(n), language, std::move(s0), std::move(actions), std::move(goal)};
}

std::vector<search::planning_task> switches::build_tasks(label_storage &l_storage) {
    const unsigned long N_MIN_TASKS = 2, N_MAX_TASK = 10;
    std::vector<search::planning_task> tasks;

    for (unsigned long n = N_MIN_TASKS; n <= N_MAX_TASK; ++n)
        tasks.emplace_back(switches::build_task(n, l_storage));

    return tasks;
}
