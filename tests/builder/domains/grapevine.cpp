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

#include "grapevine.h"
#include "domain_utils.h"
#include "../action_builder.h"
#include "../../../include/del/formulas/propositional/atom_formula.h"
#include "../../../include/del/formulas/modal/box_formula.h"
#include "../../../include/del/formulas/propositional/not_formula.h"
#include "../../../include/del/formulas/propositional/and_formula.h"
#include "../../../include/del/formulas/propositional/or_formula.h"
#include "../../../include/del/formulas/propositional/true_formula.h"
#include "../../../include/del/formulas/propositional/false_formula.h"
#include "../../../include/del/formulas/propositional/imply_formula.h"
#include <memory>

using namespace daedalus::tester;
using namespace del;

std::string grapevine::get_name() {
    return "grapevine";
}

del::language_ptr grapevine::build_language(unsigned long agents_no, unsigned long secrets_no) {
    name_vector atom_names(secrets_no + agents_no), agent_names(agents_no);

    for (unsigned long i = 0; i < secrets_no; ++i)
        atom_names[i] = "s_" + std::to_string(i);

    for (unsigned long i = 0; i < agents_no; ++i) {
        atom_names[secrets_no + i] = "in_room_1_ag_" + std::to_string(i);

        agent_names[i] = "ag_" + std::to_string(i);
    }

    return std::make_shared<language>(std::move(language{atom_names, agent_names}));
}

kripke::state grapevine::build_initial_state(unsigned long agents_no, unsigned long secrets_no) {
    language_ptr language = grapevine::build_language(agents_no, secrets_no);

    const auto worlds_number = static_cast<const world_id>(std::exp2(secrets_no));
    world_id count = 0;

    label_vector ls = label_vector(worlds_number);
    auto secrets_combinations = domain_utils::all_combinations(secrets_no);

    for (auto &combination: secrets_combinations) {
        for (unsigned long bit = 0; bit < agents_no; ++bit)
            combination.push_back(true);

        ls[count++] = label{combination};
    }
    boost::dynamic_bitset<> all_worlds(worlds_number);
    all_worlds.set();

    relations r(language->get_agents_number());

    for (agent ag = 0; ag < language->get_agents_number(); ++ag) {
        r[ag] = agent_relation(worlds_number);

        for (world_id w = 0; w < worlds_number; ++w)
            r[ag][w] = world_set(all_worlds);
    }

    for (agent ag = 0; ag < secrets_no; ++ag)
        for (world_id w = 0; w < worlds_number; ++w)
            for (world_id v = 0; v < worlds_number; ++v)
                if (ls[w][ag] != ls[v][ag])
                    r[ag][w].remove(v);

    world_id designated;

    for (world_id w = 0; w < worlds_number; ++w)
        if ((*ls[w]).all()) designated = w;

    world_set designated_worlds = world_set{worlds_number, world_list{designated}};

    return state{language, worlds_number, std::move(r), std::move(ls), std::move(designated_worlds)};
}

kripke::action_deque grapevine::build_actions(unsigned long agents_no, unsigned long secrets_no) {
    action_deque actions;

    for (unsigned long ag_1 = 0; ag_1 < secrets_no; ++ag_1) {
        boost::dynamic_bitset<> filter(agents_no);
        filter[ag_1] = true;

        auto ag_combinations = domain_utils::all_combinations_without(agents_no, filter);

        for (const auto &fo_ags : ag_combinations)
            actions.push_back(std::make_shared<action>(build_tell(agents_no, secrets_no, ag_1, fo_ags)));
    }

    for (unsigned long ag = 0; ag < agents_no; ++ag) {
        actions.push_back(std::make_shared<action>(build_left(agents_no, secrets_no, ag)));
        actions.push_back(std::make_shared<action>(build_right(agents_no, secrets_no, ag)));
    }

    return actions;
}

search::planning_task grapevine::build_task(unsigned long agents_no, unsigned long secrets_no, unsigned long learning_ags_no) {
    std::string name = grapevine::get_name();
    std::string id   = std::to_string(agents_no) + "_" + std::to_string(secrets_no) + "_" + std::to_string(learning_ags_no);
    language_ptr language = grapevine::build_language(agents_no, secrets_no);

    state s0 = grapevine::build_initial_state(agents_no, secrets_no);
    action_deque actions = grapevine::build_actions(agents_no, secrets_no);
    formula_ptr goal = grapevine::build_goal(agents_no, secrets_no, learning_ags_no);

    return search::planning_task{std::move(name), std::move(id), language, std::move(s0), std::move(actions), std::move(goal)};
}

std::vector<search::planning_task> grapevine::build_tasks() {
    const unsigned long N_MIN_AGS = 3, N_MAX_AGS = 7, N_MIN_SECRETS = 1, N_MAX_SECRETS = 4;

    std::vector<search::planning_task> tasks;

    for (unsigned long agents_no = N_MIN_AGS; agents_no <= N_MAX_AGS; ++agents_no)
        for (unsigned long secrets_no = N_MIN_SECRETS; secrets_no <= N_MAX_SECRETS; ++secrets_no)
            for (unsigned long learning_ags_no = 1; learning_ags_no <= agents_no; ++learning_ags_no)
                tasks.push_back(build_task(agents_no, secrets_no, learning_ags_no));

    return tasks;
}

kripke::action grapevine::build_tell(unsigned long agents_no, unsigned long secrets_no, del::agent ag_1, const agent_set &fo_ags) {
    language_ptr language = grapevine::build_language(agents_no, secrets_no);

    agent_set all_fo = fo_ags;
    all_fo[ag_1] = true;

    formula_ptr s_1 = std::make_shared<atom_formula>(ag_1);     // index of ag_1 is the same as index of s_1

    formula_deque fs;

    for (agent ag = 0; ag < agents_no; ++ag)
        fs.push_back(std::make_shared<not_formula>(std::make_shared<box_formula>(ag, s_1)));

    formula_ptr some_agent_not_K_s_1 = std::make_shared<or_formula>(std::move(fs));

//    formula_ptr K_ag_2_s_1 = std::make_shared<box_formula>(ag_2, s_1);
//    formula_ptr not_K_ag_2_s_1 = std::make_shared<not_formula>(K_ag_2_s_1);

    formula_deque ag_room_1, ag_room_2;

    for (agent ag = 0; ag < language->get_agents_number(); ++ag) {
        formula_ptr in_room_1_ag = std::make_shared<atom_formula>(language->get_atom_id("in_room_1_ag_" + std::to_string(ag)));
        formula_ptr not_in_room_1_ag = std::make_shared<not_formula>(in_room_1_ag);

        if (all_fo[ag]) {
            ag_room_1.push_back(in_room_1_ag);
            ag_room_2.push_back(not_in_room_1_ag);
        } else {
            ag_room_1.push_back(not_in_room_1_ag);
            ag_room_2.push_back(in_room_1_ag);
        }
    }

    formula_ptr all_fo_ag_room_1 = std::make_shared<and_formula>(formula_deque{ag_room_1});
    formula_ptr all_fo_ag_room_2 = std::make_shared<and_formula>(formula_deque{ag_room_2});

    formula_ptr in_room_1_ag_1 = std::make_shared<atom_formula>(language->get_atom_id("in_room_1_ag_" + std::to_string(ag_1)));
    formula_ptr not_in_room_1_ag_1 = std::make_shared<not_formula>(in_room_1_ag_1);

    formula_ptr if_ag_room_1 = std::make_shared<imply_formula>(in_room_1_ag_1, all_fo_ag_room_1);
    formula_ptr if_ag_room_2 = std::make_shared<imply_formula>(not_in_room_1_ag_1, all_fo_ag_room_2);

    formula_ptr same_room_fo_ag = std::make_shared<and_formula>(formula_deque{if_ag_room_1, if_ag_room_2});

    formula_deque fs_pre = {s_1, some_agent_not_K_s_1, same_room_fo_ag};
    formula_ptr f_pre = std::make_shared<and_formula>(fs_pre);

    std::string fo_str;
    to_string(all_fo, fo_str);
    std::reverse(fo_str.begin(), fo_str.end());
    std::string name = "tell_" + std::to_string(ag_1) + "_" + fo_str;

    return action_builder::build_private_sensing(std::move(name), language, f_pre, all_fo);
}

kripke::action grapevine::build_left(unsigned long agents_no, unsigned long secrets_no, del::agent ag) {
    language_ptr language = grapevine::build_language(agents_no, secrets_no);
    std::string ag_name = language->get_agent_name(ag);

    formula_ptr in_room_1_ag = std::make_shared<atom_formula>(language->get_atom_id("in_room_1_" + ag_name));
    formula_ptr not_in_room_1_ag = std::make_shared<not_formula>(in_room_1_ag);
    formula_ptr f_pre = std::make_shared<box_formula>(ag, not_in_room_1_ag);

    event_post e_post;
    e_post[language->get_atom_id("in_room_1_" + ag_name)] = std::make_shared<true_formula>();

    std::string name = "left_" + std::to_string(ag);

    return action_builder::build_public_ontic(std::move(name), language, f_pre, e_post);
}

kripke::action grapevine::build_right(unsigned long agents_no, unsigned long secrets_no, del::agent ag) {
    language_ptr language = grapevine::build_language(agents_no, secrets_no);
    std::string ag_name = language->get_agent_name(ag);

    formula_ptr in_room_1_ag = std::make_shared<atom_formula>(language->get_atom_id("in_room_1_" + ag_name));
    formula_ptr f_pre = std::make_shared<box_formula>(ag, in_room_1_ag);

    event_post e_post;
    e_post[language->get_atom_id("in_room_1_" + ag_name)] = std::make_shared<false_formula>();

    std::string name = "right_" + std::to_string(ag);

    return action_builder::build_public_ontic(std::move(name), language, f_pre, e_post);
}

del::formula_ptr grapevine::build_goal(unsigned long agents_no, unsigned long secrets_no, unsigned long learning_ags_no) {
    language_ptr language = grapevine::build_language(agents_no, secrets_no);
    formula_ptr goal;

    formula_deque fs;

    for (agent ag = 0; ag < agents_no; ++ag) {
        if (ag < learning_ags_no) {
            formula_deque fs_secrets;

            for (unsigned long secret = 0; secret < secrets_no; ++secret)
                if (secret != ag)
                    fs_secrets.push_back(std::make_shared<atom_formula>(secret));

            fs.push_back(std::make_shared<box_formula>(ag, std::make_shared<and_formula>(std::move(fs_secrets))));
        } else {
            for (agent ag_1 = 0; ag_1 < agents_no; ++ag_1)
                if (ag_1 < learning_ags_no) {
                    formula_deque fs_secrets;

                    for (unsigned long secret = 0; secret < secrets_no; ++secret)
                        if (secret != ag_1 and secret != ag)
                            fs_secrets.push_back(std::make_shared<atom_formula>(secret));

                    if (not fs_secrets.empty()) {
                        formula_ptr f = std::make_shared<box_formula>(ag_1, std::make_shared<and_formula>(std::move(fs_secrets)));
                        fs.push_back(std::make_shared<not_formula>(std::make_shared<box_formula>(ag, std::move(f))));
                    }
                }
        }
    }

    return std::make_shared<and_formula>(std::move(fs));
}
