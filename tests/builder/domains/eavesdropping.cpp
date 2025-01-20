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

#include "eavesdropping.h"
#include "domain_utils.h"
#include "../action_builder.h"
#include "../../../include/del/formulas/propositional/atom_formula.h"
#include "../../../include/del/formulas/modal/box_formula.h"
#include "../../../include/del/formulas/propositional/not_formula.h"
#include "../../../include/del/formulas/propositional/or_formula.h"
#include "../../../include/del/formulas/propositional/true_formula.h"
#include "../../../include/del/formulas/propositional/false_formula.h"
#include "../../../include/del/formulas/propositional/and_formula.h"
#include <memory>
#include <string>

using namespace daedalus::tester;
using namespace del;

std::string eavesdropping::get_name() {
    return "eavesdropping";
}

std::string eavesdropping::get_id(unsigned long agents_no, unsigned long steps_no) {
    return std::to_string(agents_no) + "_" + std::to_string(steps_no);
}

del::language_ptr eavesdropping::build_language(unsigned long agents_no, unsigned long steps_no) {
    name_vector atom_names(agents_no * steps_no), agent_names(agents_no);
    unsigned long count = 0;

    for (unsigned long i = 0; i < agents_no; ++i) {

        if (i != agents_no-1)
            for (unsigned long l = 0; l < steps_no; ++l)
                atom_names[count++] = "at_" + std::to_string(i) + "_" + std::to_string(l);

        agent_names[i] = "ag_" + std::to_string(i);
    }

    return std::make_shared<language>(std::move(language{atom_names, agent_names}));
}

kripke::state eavesdropping::build_initial_state(unsigned long agents_no, unsigned long steps_no, const del::label_storage_ptr &l_storage) {
    language_ptr language = eavesdropping::build_language(agents_no, steps_no);
    const auto worlds_number = 1;

    relations r(language->get_agents_number());

    for (agent ag = 0; ag < language->get_agents_number(); ++ag) {
        r[ag]    = agent_relation(worlds_number);
        r[ag][0] = world_bitset(worlds_number, world_set{0});
    }

    label_vector ls = label_vector(worlds_number);
    auto bs = boost::dynamic_bitset<>(language->get_atoms_number());

    for (unsigned long i = 0; i < agents_no-1; ++i)
        bs[language->get_atom_id("at_" + std::to_string(i) + "_0")] = true;

    ls[0] = l_storage->emplace(label{std::move(bs)});

    world_bitset designated_worlds = world_bitset{worlds_number, world_set{0}};

    return state{language, worlds_number, std::move(r), std::move(ls), std::move(designated_worlds)};
}

kripke::action_deque eavesdropping::build_actions(unsigned long agents_no, unsigned long steps_no) {
    language_ptr language = eavesdropping::build_language(agents_no, steps_no);
    action_deque actions;

    for (agent i = 0; i < agents_no-1; ++i) {
        actions.push_back(std::make_shared<action>(build_move(agents_no, steps_no, i)));
        actions.push_back(std::make_shared<action>(build_shout(agents_no, steps_no, i)));
    }

    return actions;
}

search::planning_task eavesdropping::build_task(unsigned long agents_no, unsigned long steps_no, const del::label_storage_ptr &l_storage) {
    std::string name = eavesdropping::get_name();
    std::string id = std::to_string(agents_no) + "_" + std::to_string(steps_no);

    language_ptr language = eavesdropping::build_language(agents_no, steps_no);
    state s0 = eavesdropping::build_initial_state(agents_no, steps_no, l_storage);

    action_deque actions = eavesdropping::build_actions(agents_no, steps_no);

    formula_deque fs;

    for (agent i = 0; i < agents_no-1; ++i)
        fs.push_back(std::make_shared<atom_formula>(language->get_atom_id("at_" + std::to_string(i) + "_" + std::to_string(steps_no-1))));

    formula_ptr goal = std::make_shared<and_formula>(std::move(fs));
//    formula_ptr goal = std::make_shared<box_formula>(agents_no-1, std::make_shared<and_formula>(std::move(fs)));

    return search::planning_task{std::move(name), std::move(id), language, std::move(s0), std::move(actions), std::move(goal)};
}

std::vector<search::planning_task> eavesdropping::build_tasks(const del::label_storage_ptr &l_storage) {
    const unsigned long N_MIN_AGS = 2, N_MAX_AGS = 4, N_MIN_STEPS = 2, N_MAX_STEPS = 5;
    std::vector<search::planning_task> tasks;

    for (unsigned long agents_no = N_MIN_AGS; agents_no <= N_MAX_AGS; ++agents_no)
        for (unsigned long rooms_no = N_MIN_STEPS; rooms_no <= N_MAX_STEPS; ++rooms_no)
            tasks.emplace_back(eavesdropping::build_task(agents_no, rooms_no, l_storage));

    return tasks;
}

kripke::action eavesdropping::build_move(unsigned long agents_no, unsigned long steps_no, del::agent ag) {
    language_ptr language = eavesdropping::build_language(agents_no, steps_no);
    const event_id events_number = agents_no-1;
    action_relations q(language->get_agents_number());

    for (agent i = 0; i < language->get_agents_number(); ++i) {
        q[i] = action_agent_relations(events_number);

        for (event_id e = 0; e < events_number; ++e)
            q[i][e] = event_bitset(events_number);
    }

    for (agent i = 0; i < language->get_agents_number(); ++i)
        for (event_id e = 0; e < events_number; ++e)
            for (event_id f = 0; f < events_number; ++f)
                if (i == agents_no-1 or e == f)
                    q[i][e].push_back(f);

    preconditions pre(events_number);
    postconditions post = postconditions(events_number);
    boost::dynamic_bitset<> is_ontic(events_number);

    for (agent i = 0; i < agents_no-1; ++i) {
        formula_ptr at_i_l = std::make_shared<atom_formula>(language->get_atom_id("at_" + std::to_string(i) + "_" + std::to_string(steps_no-1)));
        pre[i] = std::make_shared<not_formula>(std::move(at_i_l));

        event_post e_post;
        std::string i_str = std::to_string(i);

        atom at_room_1_ag = language->get_atom_id("at_" + i_str + "_1");
        e_post[at_room_1_ag] = std::make_shared<false_formula>();

        for (unsigned long step = 0; step < steps_no-1; ++step) {
            atom at_i_to   = language->get_atom_id("at_" + i_str + "_" + std::to_string(step + 1));
            atom at_i_from = language->get_atom_id("at_" + i_str + "_" + std::to_string(step));
            formula_ptr f_post = std::make_shared<atom_formula>(at_i_from);
            e_post[at_i_to] = std::move(f_post);
        }

        post[i] = std::move(e_post);
        is_ontic[i] = true;
    }

    event_set designated_events = event_set{ag};
    std::string name = "move_" + std::to_string(ag);

    return action{language, action_type::public_announcement, std::move(name), events_number, q, pre, post, is_ontic, designated_events};
}

kripke::action eavesdropping::build_shout(unsigned long agents_no, unsigned long steps_no, del::agent ag) {
    language_ptr language = eavesdropping::build_language(agents_no, steps_no);
    formula_ptr f = std::make_shared<atom_formula>(language->get_atom_id("at_" + std::to_string(ag) + "_" + std::to_string(steps_no-1)));

    return action_builder::build_public_announcement("shout_" + std::to_string(ag), language, f);
}
