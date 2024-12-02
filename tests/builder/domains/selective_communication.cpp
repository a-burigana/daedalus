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

#include "selective_communication.h"
#include "domain_utils.h"
#include "../action_builder.h"
#include "../../../include/del/formulas/propositional/not_formula.h"
#include "../../../include/del/formulas/propositional/atom_formula.h"
#include "../../../include/del/formulas/modal/box_formula.h"
#include "../../../include/del/formulas/modal/diamond_formula.h"
#include "../../../include/del/formulas/propositional/and_formula.h"
#include "../../../include/del/formulas/propositional/or_formula.h"
#include "../../../include/del/formulas/propositional/false_formula.h"
#include <memory>
#include <string>

using namespace daedalus::tester;
using namespace del;

std::string selective_communication::get_name() {
    return "selective_communication";
}

del::language_ptr selective_communication::build_language(const unsigned long agents_no, const unsigned long rooms_no) {
    assert(agents_no < rooms_no);

    name_vector atom_names, agent_names(agents_no);
    atom_names.emplace_back("q");

    for (agent ag = 0; ag < agents_no; ++ag) {
        std::string ag_name{static_cast<char>('a' + ag)};
        agent_names[ag] = ag_name;

        for (unsigned long r = 1; r <= rooms_no; ++r)
            atom_names.emplace_back("in_room_" + std::to_string(r) + "_" + ag_name);
    }

    return std::make_shared<language>(std::move(language{atom_names, agent_names}));
}

kripke::state selective_communication::build_initial_state(unsigned long agents_no, unsigned long rooms_no, const label_storage_ptr &l_storage) {
    language_ptr language = selective_communication::build_language(agents_no, rooms_no);

    const world_id worlds_number = 2;
    world_bitset designated_worlds = world_bitset{worlds_number, world_set{0}};

    relations r(language->get_agents_number());

    for (agent ag = 0; ag < language->get_agents_number(); ++ag) {
        r[ag] = agent_relation(worlds_number);

        for (world_id w = 0; w < worlds_number; ++w)
            r[ag][w] = world_bitset(worlds_number);
    }

    for (agent ag = 0; ag < language->get_agents_number(); ++ag)
        for (world_id w = 0; w < worlds_number; ++w)
            for (world_id v = 0; v < worlds_number; ++v)
                r[ag][w].push_back(v);

    label_vector ls = label_vector(worlds_number);

    boost::dynamic_bitset<> b0(language->get_atoms_number()), b1(language->get_atoms_number());

    b0.set(language->get_atom_id("q"));

    for (agent ag = 0; ag < agents_no; ++ag) {
        std::string ag_name{static_cast<char>('a' + ag)};
        atom in_room_ag = language->get_atom_id("in_room_" + std::to_string(ag + 1) + "_" + ag_name);
        b0.set(in_room_ag);
        b1.set(in_room_ag);
    }

    label l0 = label{std::move(b0)}, l1 = label{std::move(b1)};
    ls[0] = l_storage->emplace(std::move(l0));
    ls[1] = l_storage->emplace(std::move(l1));

    return state{language, worlds_number, std::move(r), std::move(ls), std::move(designated_worlds)};
}

kripke::action_deque selective_communication::build_actions(unsigned long agents_no, unsigned long rooms_no) {
    language_ptr language = selective_communication::build_language(agents_no, rooms_no);
    action_deque actions;

    for (agent i = 0; i < language->get_agents_number(); ++i) {
        agent_set i_ags = agent_set(language->get_agents_number());
        i_ags[i] = true;

        bitset_deque i_combinations = domain_utils::all_combinations_with(language->get_agents_number(), i_ags);

        for (const agent_set &fo_ags: i_combinations)
            if (fo_ags.count() > 1)
                for (unsigned long r = 1; r <= rooms_no; ++r)
                    actions.push_back(std::make_shared<action>(
                            std::move(selective_communication::build_tell(agents_no, rooms_no, i, r, fo_ags))));

        i_combinations = domain_utils::all_combinations_without(language->get_agents_number(), i_ags);

        for (const agent_set &po_ags: i_combinations)
            actions.push_back(
                    std::make_shared<action>(selective_communication::build_sense(agents_no, rooms_no, i, po_ags)));

        agent_set all_ags = agent_set(language->get_agents_number());
        all_ags.set();

        actions.push_back(std::make_shared<action>(
                std::move(selective_communication::build_left(agents_no, rooms_no, i, all_ags))));
        actions.push_back(std::make_shared<action>(
                std::move(selective_communication::build_right(agents_no, rooms_no, i, all_ags))));
    }

    return actions;
}

del::formula_ptr selective_communication::build_goal(const language_ptr &language, unsigned long goal_id) {
    assert(1 <= goal_id and goal_id <= 4);

    formula_ptr goal;
    formula_ptr q     = std::make_shared<atom_formula>(language->get_atom_id("q"));
    formula_ptr not_q = std::make_shared<not_formula>(q);

    if (goal_id == 1) {
        agent a = 0;
        formula_ptr B_a_q = std::make_shared<box_formula>(a, q);

        goal = std::move(B_a_q);
    } else if (goal_id == 2) {
        formula_deque fs;

        for (agent ag = 0; ag < language->get_agents_number(); ++ag) {
            formula_ptr B_ag_q         = std::make_shared<box_formula>(ag, q);
            formula_ptr not_B_ag_q     = std::make_shared<not_formula>(B_ag_q);

            formula_ptr B_ag_not_q     = std::make_shared<box_formula>(ag, not_q);
            formula_ptr not_B_ag_not_q = std::make_shared<not_formula>(B_ag_not_q);

            formula_ptr not_Bw_ag_q    = std::make_shared<and_formula>(formula_deque{not_B_ag_q, not_B_ag_not_q});

            formula_ptr f_ag = ag <= language->get_agents_number() / 2
                               ? B_ag_q
                               : not_Bw_ag_q;

            fs.push_back(std::move(f_ag));
        }

        goal = std::make_shared<and_formula>(std::move(fs));
    } else if (goal_id == 3) {
        agent a = 0, b = language->get_agents_number() / 2;

        formula_ptr B_a_q      = std::make_shared<box_formula>(a, q);
        formula_ptr B_ag_not_q = std::make_shared<box_formula>(a, not_q);
        formula_ptr Bw_a_q     = std::make_shared<or_formula>(formula_deque{std::move(B_a_q), std::move(B_ag_not_q)});
        formula_ptr B_b_Bw_a_q = std::make_shared<box_formula>(b, std::move(Bw_a_q));

        goal = std::move(B_b_Bw_a_q);
    } else if (goal_id == 4) {
        agent a = 0, b = language->get_agents_number() / 2;

        formula_ptr B_a_q      = std::make_shared<box_formula>(a, q);
        formula_ptr B_b_B_a_q = std::make_shared<box_formula>(b, std::move(B_a_q));

        goal = std::move(B_b_B_a_q);
    }

    return goal;
}

search::planning_task selective_communication::build_task(unsigned long agents_no, unsigned long rooms_no, unsigned long goal_id, const del::label_storage_ptr &l_storage) {
    std::string name = selective_communication::get_name();
    std::string id = std::to_string(agents_no) + "_" + std::to_string(rooms_no) + "_g" + std::to_string(goal_id);

    language_ptr language = selective_communication::build_language(agents_no, rooms_no);
    state s0 = selective_communication::build_initial_state(agents_no, rooms_no, l_storage);

    action_deque actions = selective_communication::build_actions(agents_no, rooms_no);
    formula_ptr goal = selective_communication::build_goal(language, goal_id);

    return search::planning_task{std::move(name), std::move(id), language, std::move(s0), std::move(actions), std::move(goal)};
}

std::vector<search::planning_task> selective_communication::build_tasks(const del::label_storage_ptr &l_storage) {
    const unsigned long N_MIN_AGS = 2, N_MAX_AGS = 3, N_MIN_ROOMS = 5, N_MAX_ROOMS = 5, MIN_GOAL_ID = 3, MAX_GOAL_ID = 4;
    std::vector<search::planning_task> tasks;

    for (unsigned long goal_id = MIN_GOAL_ID; goal_id <= MAX_GOAL_ID; ++goal_id)
        for (unsigned long agents_no = N_MIN_AGS; agents_no <= N_MAX_AGS; ++agents_no)
            for (unsigned long rooms_no = N_MIN_ROOMS; rooms_no <= N_MAX_ROOMS; ++rooms_no)
                if (agents_no < rooms_no)
                    tasks.emplace_back(selective_communication::build_task(agents_no, rooms_no, goal_id, l_storage));

    return tasks;
}

kripke::action selective_communication::build_left(unsigned long agents_no, unsigned long rooms_no, del::agent ag, const del::agent_set &fo_ags) {
    language_ptr language = selective_communication::build_language(agents_no, rooms_no);
    std::string ag_name = language->get_agent_name(ag);

    formula_ptr in_room_1_ag = std::make_shared<atom_formula>(language->get_atom_id("in_room_1_" + ag_name));
    formula_ptr f_pre = std::make_shared<not_formula>(in_room_1_ag);

    event_post e_post;

    atom in_room_n_ag = language->get_atom_id("in_room_" + std::to_string(rooms_no) + "_" + ag_name);
    e_post[in_room_n_ag] = std::make_shared<false_formula>();

    for (unsigned long r = 1; r < rooms_no; ++r) {
        atom in_room_r_ag_to   = language->get_atom_id("in_room_" + std::to_string(r)   + "_" + ag_name);
        atom in_room_r_ag_from = language->get_atom_id("in_room_" + std::to_string(r+1) + "_" + ag_name);
        formula_ptr f_post = std::make_shared<atom_formula>(in_room_r_ag_from);
        e_post[in_room_r_ag_to] = std::move(f_post);
    }

    std::string fo_str;
    to_string(fo_ags, fo_str);
    std::reverse(fo_str.begin(), fo_str.end());
    std::string name = "left_" + ag_name + "_" + fo_str;

    return action_builder::build_public_ontic(std::move(name), language, f_pre, e_post);
}

kripke::action selective_communication::build_right(unsigned long agents_no, unsigned long rooms_no, del::agent ag, const del::agent_set &fo_ags) {
    language_ptr language = selective_communication::build_language(agents_no, rooms_no);
    std::string ag_name = language->get_agent_name(ag);

    formula_ptr in_room_n_ag = std::make_shared<atom_formula>(language->get_atom_id("in_room_" + std::to_string(rooms_no) + "_" + ag_name));
    formula_ptr f_pre = std::make_shared<not_formula>(in_room_n_ag);

    event_post e_post;

    atom in_room_1_ag = language->get_atom_id("in_room_1_" + ag_name);
    e_post[in_room_1_ag] = std::make_shared<false_formula>();

    for (unsigned long r = 1; r < rooms_no; ++r) {
        atom in_room_r_ag_to   = language->get_atom_id("in_room_" + std::to_string(r+1) + "_" + ag_name);
        atom in_room_r_ag_from = language->get_atom_id("in_room_" + std::to_string(r)   + "_" + ag_name);
        formula_ptr f_post = std::make_shared<atom_formula>(in_room_r_ag_from);
        e_post[in_room_r_ag_to] = std::move(f_post);
    }

    std::string fo_str;
    to_string(fo_ags, fo_str);
    std::reverse(fo_str.begin(), fo_str.end());
    std::string name = "right_" + ag_name + "_" + fo_str;

    return action_builder::build_public_ontic(std::move(name), language, f_pre, e_post);
}

kripke::action selective_communication::build_sense(unsigned long agents_no, unsigned long rooms_no, del::agent ag,
                                                 const del::agent_set &po_ags) {
    language_ptr language = selective_communication::build_language(agents_no, rooms_no);
    std::string ag_name = language->get_agent_name(ag);

    agent_set fo_ags(language->get_agents_number());
    fo_ags[ag] = true;

    agent_set looking_ags = po_ags;
    looking_ags[ag] = true;

    formula_ptr sensed = std::make_shared<atom_formula>(language->get_atom_id("q"));

    formula_deque fs = in_room_agents(language, rooms_no, looking_ags);
    fs.push_back(std::make_shared<atom_formula>(language->get_atom_id("in_room_" + std::to_string(rooms_no) + "_" + ag_name)));

    formula_ptr f_pre = std::make_shared<and_formula>(std::move(fs));

    std::string looking_str;
    to_string(looking_ags, looking_str);
    std::reverse(looking_str.begin(), looking_str.end());
    std::string name = "sense_" + ag_name + "_" + looking_str;

    return action_builder::build_semi_private_sensing(std::move(name), language, f_pre, sensed, fo_ags, po_ags);
}

kripke::action selective_communication::build_tell(unsigned long agents_no, unsigned long rooms_no, del::agent ag,
                                                unsigned long r, const del::agent_set &fo_ags) {
    language_ptr language = selective_communication::build_language(agents_no, rooms_no);
    std::string ag_name = language->get_agent_name(ag);

    formula_ptr q            = std::make_shared<atom_formula>(language->get_atom_id("q"));
    formula_ptr D_a_q = std::make_shared<diamond_formula>(ag, q);

    formula_deque fs = in_room_agents(language, r, fo_ags);
    fs.push_back(D_a_q);

    formula_ptr f_pre = std::make_shared<and_formula>(std::move(fs));

    std::string fo_str;
    to_string(fo_ags, fo_str);
    std::reverse(fo_str.begin(), fo_str.end());
    std::string name = "tell_" + ag_name + "_" + std::to_string(r) + "_" + fo_str;

    return action_builder::build_private_announcement(std::move(name), language, f_pre, fo_ags);
}

del::formula_deque selective_communication::in_room_agents(const del::language_ptr &language, unsigned long r,
                                                           const del::agent_set &ags) {
    formula_deque fs;

    for (agent ag = 0; ag < language->get_agents_number(); ++ag) {
        std::string ag_name = language->get_agent_name(ag);
        formula_ptr in_room_r_ag = std::make_shared<atom_formula>(language->get_atom_id("in_room_" + std::to_string(r) + "_" + language->get_agent_name(ag)));

        if (ags[ag])
            fs.push_back(std::move(in_room_r_ag));
        else
            fs.push_back(std::make_shared<not_formula>(std::move(in_room_r_ag)));
    }

    return fs;
}
