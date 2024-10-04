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

#include "collaboration_communication.h"
#include "domain_utils.h"
#include "../action_builder.h"
#include "../../../include/del/formulas/propositional/atom_formula.h"
#include "../../../include/del/formulas/propositional/not_formula.h"
#include "../../../include/del/formulas/propositional/and_formula.h"
#include "../../../include/del/formulas/modal/box_formula.h"
#include "../../../include/del/formulas/modal/diamond_formula.h"
#include "../../../include/del/formulas/propositional/false_formula.h"
#include <memory>
#include <string>

using namespace daedalus::tester;
using namespace del;

std::string collaboration_communication::get_name() {
    return "collaboration_through_communication";
}

del::language_ptr
collaboration_communication::build_language(unsigned long agents_no, unsigned long rooms_no, unsigned long boxes_no) {
    assert(rooms_no = boxes_no + 1);

    name_vector atom_names, agent_names(agents_no);

    for (agent ag = 0; ag < agents_no; ++ag) {
        std::string ag_name{static_cast<char>('a' + ag)};
        agent_names[ag] = ag_name;

        for (unsigned long r = 1; r <= rooms_no; ++r) {
            atom_names.emplace_back("in_room_" + std::to_string(r) + "_" + ag_name);

            for (unsigned long b = 1; b <= boxes_no; ++b)
                atom_names.emplace_back("in_room_" + std::to_string(r) + "_box_" + std::to_string(b));
        }
    }

    return std::make_shared<language>(std::move(language{atom_names, agent_names}));
}

kripke::state collaboration_communication::build_initial_state(unsigned long agents_no, unsigned long rooms_no,
                                                               unsigned long boxes_no) {
    language_ptr language = collaboration_communication::build_language(agents_no, rooms_no, boxes_no);

    ulong_vector all_rooms_but_2(rooms_no - 1);
    all_rooms_but_2[0] = 1;

    for (unsigned long room = 2; room < rooms_no; ++room)
        all_rooms_but_2[room-1] = room+1;

    auto room_combinations = domain_utils::all_permutations_with_repetitions(all_rooms_but_2, boxes_no);

    const world_id worlds_number = room_combinations.size();    // (rooms_no-1) * boxes_no;
    world_set designated_worlds = world_set{worlds_number};

    relations r(language->get_agents_number());

    for (agent ag = 0; ag < language->get_agents_number(); ++ag) {
        r[ag] = agent_relation(worlds_number);

        for (world_id w = 0; w < worlds_number; ++w)
            r[ag][w] = world_set(worlds_number);
    }

    for (agent ag = 0; ag < language->get_agents_number(); ++ag)
        for (world_id w = 0; w < worlds_number; ++w)
            for (world_id v = 0; v < worlds_number; ++v)
                r[ag][w].push_back(v);

    label_vector ls = label_vector(worlds_number);
    world_id w = 0;

    for (const auto &rooms: room_combinations) {
        boost::dynamic_bitset<> bs(language->get_atoms_number());

        for (agent ag = 0; ag < language->get_agents_number(); ++ag) {
            std::string ag_name{static_cast<char>('a' + ag)};
            atom in_room_2_ag = language->get_atom_id("in_room_2_" + ag_name);
            bs.set(in_room_2_ag);
        }

        bool is_sorted = true;

        for (unsigned long box = 0; box < boxes_no; ++box) {
            unsigned long room = rooms[box];
            atom in_room_box = language->get_atom_id(
                    "in_room_" + std::to_string(room) + "_box_" + std::to_string(box + 1));
            bs.set(in_room_box);

            if (box > 0 and room <= rooms[box - 1])
                is_sorted = false;
        }

        if (is_sorted)
            designated_worlds.push_back(w);

        ls[w++] = label{std::move(bs)};
    }
    return state{language, worlds_number, std::move(r), std::move(ls), std::move(designated_worlds)};
}

kripke::action_deque collaboration_communication::build_actions(unsigned long agents_no, unsigned long rooms_no,
                                                             unsigned long boxes_no) {
    language_ptr language = collaboration_communication::build_language(agents_no, rooms_no, boxes_no);
    action_deque actions;

    for (agent i = 0; i < language->get_agents_number(); ++i) {
        agent_set i_ags = agent_set(language->get_agents_number());
        i_ags[i] = true;

        bitset_deque i_combinations = domain_utils::all_combinations_with(language->get_agents_number(), i_ags);

        for (const agent_set &fo_ags: i_combinations)
            if (fo_ags.count() > 1)
                for (unsigned long r = 1; r <= rooms_no; ++r)
                    if (r != 2)
                        for (unsigned long b = 1; b <= boxes_no; ++b)
                            actions.push_back(std::make_shared<action>(
                                std::move(collaboration_communication::build_tell(agents_no, rooms_no, boxes_no, i, r, b, fo_ags))));

        i_combinations = domain_utils::all_combinations_without(language->get_agents_number(), i_ags);

        for (const agent_set &po_ags: i_combinations)
            for (unsigned long r = 1; r <= rooms_no; ++r)
                if (r != 2)
                    for (unsigned long b = 1; b <= boxes_no; ++b)
                        actions.push_back(std::make_shared<action>(
                            collaboration_communication::build_sense(agents_no, rooms_no, boxes_no, i, r, b, po_ags)));

        agent_set all_ags = agent_set(language->get_agents_number());
        all_ags.set();

        actions.push_back(std::make_shared<action>(
                std::move(collaboration_communication::build_left(agents_no, rooms_no, boxes_no, i, all_ags))));
        actions.push_back(std::make_shared<action>(
                std::move(collaboration_communication::build_right(agents_no, rooms_no, boxes_no, i, all_ags))));
    }

    return actions;
}

del::formula_ptr collaboration_communication::build_goal(const del::language_ptr &language, unsigned long boxes_no,
                                                         unsigned long goal_id) {
    assert(1 <= goal_id and goal_id <= 4);

    formula_ptr goal;

    if (goal_id == 1) {
        agent a = 0, b = language->get_agents_number() / 2;
        formula_ptr in_room_1_box_1 = std::make_shared<atom_formula>(language->get_atom_id("in_room_1_box_1"));

        formula_ptr B_a_in_room_1_box_1     = std::make_shared<box_formula>(a, in_room_1_box_1);
        formula_ptr B_b_B_a_in_room_1_box_1 = std::make_shared<box_formula>(b, std::move(B_a_in_room_1_box_1));

        goal = std::move(B_b_B_a_in_room_1_box_1);
    } else if (goal_id == 2) {
        formula_ptr in_room_1_box_1 = std::make_shared<atom_formula>(language->get_atom_id("in_room_1_box_1"));
        formula_deque fs;

        for (agent ag = 0; ag < language->get_agents_number(); ++ag)
            fs.push_back(std::make_shared<box_formula>(ag, in_room_1_box_1));

        goal = std::make_shared<and_formula>(std::move(fs));
    } else if (goal_id == 3) {
        formula_deque fs;

        for (unsigned long b = 1; b <= boxes_no; ++b) {
            if (b < language->get_agents_number()) {
                agent ag = b - 1;
                unsigned long r = b == 1 ? 1 : b + 1;
                formula_ptr in_room_box = std::make_shared<atom_formula>(
                        language->get_atom_id("in_room_" + std::to_string(r) + "_box_" + std::to_string(b)));
                fs.push_back(std::make_shared<box_formula>(ag, std::move(in_room_box)));
            }
        }

        goal = std::make_shared<and_formula>(std::move(fs));
    } else if (goal_id == 4) {
        agent a = 0;
        formula_deque fs;

        for (unsigned long b = 1; b <= boxes_no; ++b) {
            unsigned long r = b == 1 ? 1 : b + 1;
            formula_ptr in_room_box = std::make_shared<atom_formula>(
                    language->get_atom_id("in_room_" + std::to_string(r) + "_box_" + std::to_string(b)));
            fs.push_back(std::make_shared<box_formula>(a, std::move(in_room_box)));
        }

        goal = std::make_shared<and_formula>(std::move(fs));
    }

    return goal;
}

search::planning_task collaboration_communication::build_task(unsigned long agents_no, unsigned long rooms_no,
                                                              unsigned long boxes_no, unsigned long goal_id) {
    std::string name = collaboration_communication::get_name();
    std::string id = std::to_string(agents_no) + "_" + std::to_string(rooms_no) + "_" + std::to_string(boxes_no) + "_g" + std::to_string(goal_id);

    language_ptr language = collaboration_communication::build_language(agents_no, rooms_no, boxes_no);
    state s0 = collaboration_communication::build_initial_state(agents_no, rooms_no, boxes_no);

    action_deque actions = collaboration_communication::build_actions(agents_no, rooms_no, boxes_no);
    formula_ptr goal = collaboration_communication::build_goal(language, boxes_no, goal_id);

    return search::planning_task{std::move(name), std::move(id), language, std::move(s0), std::move(actions), std::move(goal)};
}

std::vector<search::planning_task> collaboration_communication::build_tasks() {
    const unsigned long N_MIN_AGS = 2, N_MAX_AGS = 3, N_MIN_ROOMS = 3, N_MAX_ROOMS = 4, MIN_GOAL_ID = 1, MAX_GOAL_ID = 3;
    std::vector<search::planning_task> tasks;

    for (unsigned long agents_no = N_MIN_AGS; agents_no <= N_MAX_AGS; ++agents_no)
        for (unsigned long rooms_no = N_MIN_ROOMS; rooms_no <= N_MAX_ROOMS; ++rooms_no)
            for (unsigned long goal_id = MIN_GOAL_ID; goal_id <= MAX_GOAL_ID; ++goal_id)
                tasks.emplace_back(collaboration_communication::build_task(agents_no, rooms_no, rooms_no-1, goal_id));

    return tasks;
}

kripke::action collaboration_communication::build_left(unsigned long agents_no, unsigned long rooms_no, unsigned long boxes_no,
                                                    del::agent ag, const del::agent_set &fo_ags) {
    language_ptr language = collaboration_communication::build_language(agents_no, rooms_no, boxes_no);
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

kripke::action collaboration_communication::build_right(unsigned long agents_no, unsigned long rooms_no, unsigned long boxes_no,
                                                     del::agent ag, const del::agent_set &fo_ags) {
    language_ptr language = collaboration_communication::build_language(agents_no, rooms_no, boxes_no);
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

kripke::action collaboration_communication::build_sense(unsigned long agents_no, unsigned long rooms_no,
                                                     unsigned long boxes_no, del::agent ag, unsigned long r,
                                                     unsigned long b, const del::agent_set &po_ags) {
    language_ptr language = collaboration_communication::build_language(agents_no, rooms_no, boxes_no);
    std::string ag_name = language->get_agent_name(ag);

    agent_set fo_ags(language->get_agents_number());
    fo_ags[ag] = true;

    agent_set looking_ags = po_ags;
    looking_ags[ag] = true;

    formula_ptr sensed = std::make_shared<atom_formula>(language->get_atom_id("in_room_" + std::to_string(r) + "_box_" + std::to_string(b)));

    formula_deque fs = in_room_agents(language, r, looking_ags);

    formula_ptr f_pre = std::make_shared<and_formula>(std::move(fs));

    std::string looking_str;
    to_string(looking_ags, looking_str);
    std::reverse(looking_str.begin(), looking_str.end());
    std::string name = "sense_" + ag_name + "_in_room_" + std::to_string(r) + "_box_" + std::to_string(b) + "_" + looking_str;

    return action_builder::build_semi_private_sensing(std::move(name), language, f_pre, sensed, fo_ags, po_ags);
}

kripke::action collaboration_communication::build_tell(unsigned long agents_no, unsigned long rooms_no,
                                                    unsigned long boxes_no, del::agent ag, unsigned long r,
                                                    unsigned long b, const del::agent_set &fo_ags) {
    language_ptr language = collaboration_communication::build_language(agents_no, rooms_no, boxes_no);
    agent_set ag_set(language->get_agents_number()), fo_ags_2 = fo_ags;
    ag_set[ag] = true;
    fo_ags_2[ag] = false;

    std::string ag_name = language->get_agent_name(ag);

    formula_ptr in_room_box  = std::make_shared<atom_formula>(language->get_atom_id("in_room_" + std::to_string(r) + "_box_" + std::to_string(b)));

    formula_ptr D_a_in_room_box = std::make_shared<diamond_formula>(ag, in_room_box);

    formula_deque fs = in_room_agents(language, r, fo_ags);
    fs.push_back(D_a_in_room_box);

    formula_ptr f_pre = std::make_shared<and_formula>(std::move(fs));

    std::string fo_str;
    to_string(fo_ags, fo_str);
    std::reverse(fo_str.begin(), fo_str.end());
    std::string name = "tell_" + ag_name + "_in_room_" + std::to_string(r) + "_box_" + std::to_string(b) + "_" + fo_str;

    return action_builder::build_private_announcement(std::move(name), language, f_pre, fo_ags);
}

del::formula_deque collaboration_communication::in_room_agents(const del::language_ptr &language, unsigned long r,
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
