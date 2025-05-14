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

#include "coin_in_the_box.h"
#include "../action_builder.h"
#include "../../../include/del/formulas/propositional/atom_formula.h"
#include "../../../include/del/formulas/propositional/true_formula.h"
#include "../../../include/del/formulas/propositional/false_formula.h"
#include "../../../include/del/formulas/propositional/not_formula.h"
#include "../../../include/del/formulas/propositional/and_formula.h"
#include "../../../include/del/formulas/modal/box_formula.h"
#include "../../../include/del/formulas/modal/diamond_formula.h"
#include "../../../include/del/formulas/propositional/or_formula.h"
#include "domain_utils.h"
#include <memory>
#include <string>

using namespace daedalus::tester;
using namespace del;

std::string coin_in_the_box::get_name() {
    return "coin_in_the_box";
}

del::language_ptr coin_in_the_box::build_language() {
    name_vector
            atom_names  = {"heads", "opened", "looking_a", "looking_b", "looking_c"},
            agent_names = {"a", "b", "c"};

    return std::make_shared<language>(std::move(language{atom_names, agent_names}));
}

kripke::state coin_in_the_box::build_initial_state(label_storage &l_storage) {
    language_ptr language = coin_in_the_box::build_language();

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

    b0.set(language->get_atom_id("heads"));
    b0.set(language->get_atom_id("looking_a"));
    b1.set(language->get_atom_id("looking_a"));

    label l0 = label{std::move(b0)}, l1 = label{std::move(b1)};
    ls[0] = l_storage.emplace(std::move(l0));
    ls[1] = l_storage.emplace(std::move(l1));

    return state{language, worlds_number, std::move(r), std::move(ls), std::move(designated_worlds)};
}


kripke::action_deque coin_in_the_box::build_actions() {
    language_ptr language = coin_in_the_box::build_language();
    action_deque actions;

    for (agent i = 0; i < language->get_agents_number(); ++i) {
        agent_set i_ags = agent_set(language->get_agents_number());
        i_ags[i] = true;

        bitset_deque i_combinations = domain_utils::all_combinations_with(language->get_agents_number(), i_ags);

        for (const agent_set &fo_ags : i_combinations) {
            if (i == 0)
                actions.push_back(std::make_shared<action>(std::move(coin_in_the_box::build_open(i, fo_ags))));
            if (fo_ags.count() > 1)
                actions.push_back(std::make_shared<action>(std::move(coin_in_the_box::build_shout(i, fo_ags))));
        }

        i_combinations = domain_utils::all_combinations_without(language->get_agents_number(), i_ags);

        for (const agent_set &po_ags : i_combinations)
            actions.push_back(std::make_shared<action>(coin_in_the_box::build_peek(i, po_ags)));

        agent_set all_ags = agent_set(language->get_agents_number());
        all_ags.set();

        actions.push_back(std::make_shared<action>(std::move(coin_in_the_box::build_walk_in(i, all_ags))));
        actions.push_back(std::make_shared<action>(std::move(coin_in_the_box::build_walk_out(i, all_ags))));
    }
    return actions;
}

search::planning_task coin_in_the_box::build_task(unsigned long id, label_storage &l_storage) {
    if (id == 1) return build_task_1(l_storage);
    else if (id == 2) return build_task_2(l_storage);
    else if (id == 3) return build_task_3(l_storage);
    else if (id == 4) return build_task_4(l_storage);
    else if (id == 5) return build_task_5(l_storage);
    else if (id == 6) return build_task_6(l_storage);
    return build_task_1(l_storage);
}

std::vector<search::planning_task> coin_in_the_box::build_tasks(label_storage &l_storage) {
    const unsigned long N_TASKS = 5;
    std::vector<search::planning_task> tasks;
    tasks.reserve(N_TASKS);

    for (unsigned long id = 1; id <= N_TASKS; ++id)
        tasks.emplace_back(coin_in_the_box::build_task(id, l_storage));

    return tasks;
}

search::planning_task coin_in_the_box::build_task_1(label_storage &l_storage) {
    std::string domain_name = coin_in_the_box::get_name();
    unsigned long problem_id = 1;

    language_ptr language = coin_in_the_box::build_language();
    state s0 = coin_in_the_box::build_initial_state(l_storage);
    action_deque actions = coin_in_the_box::build_actions();

    formula_ptr heads = std::make_shared<atom_formula>(language->get_atom_id("heads"));
    formula_ptr B_a_heads = std::make_shared<box_formula>(language->get_agent_id("a"), heads);

    return search::planning_task{std::move(domain_name), "cb_" + std::to_string(problem_id), language, std::move(s0), std::move(actions), std::move(B_a_heads)};
}

search::planning_task coin_in_the_box::build_task_2(label_storage &l_storage) {
    std::string domain_name = coin_in_the_box::get_name();
    unsigned long problem_id = 2;

    language_ptr language = coin_in_the_box::build_language();
    state s0 = coin_in_the_box::build_initial_state(l_storage);
    action_deque actions = coin_in_the_box::build_actions();

    formula_ptr heads = std::make_shared<atom_formula>(language->get_atom_id("heads"));
    formula_ptr B_b_heads = std::make_shared<box_formula>(language->get_agent_id("b"), heads);

    return search::planning_task{std::move(domain_name), "cb_" + std::to_string(problem_id), language, std::move(s0), std::move(actions), std::move(B_b_heads)};
}

search::planning_task coin_in_the_box::build_task_3(label_storage &l_storage) {
    std::string domain_name = coin_in_the_box::get_name();
    unsigned long problem_id = 3;

    language_ptr language = coin_in_the_box::build_language();
    state s0 = coin_in_the_box::build_initial_state(l_storage);
    action_deque actions = coin_in_the_box::build_actions();

    formula_ptr heads = std::make_shared<atom_formula>(language->get_atom_id("heads"));
    formula_deque fs;

    for (agent ag = 0; ag < language->get_agents_number(); ++ag)
        fs.push_back(std::make_shared<box_formula>(ag, heads));

    formula_ptr goal = std::make_shared<and_formula>(std::move(fs));

    return search::planning_task{std::move(domain_name), "cb_" + std::to_string(problem_id), language, std::move(s0), std::move(actions), std::move(goal)};
}

search::planning_task coin_in_the_box::build_task_4(label_storage &l_storage) {
    std::string domain_name = coin_in_the_box::get_name();
    unsigned long problem_id = 4;

    language_ptr language = coin_in_the_box::build_language();
    state s0 = coin_in_the_box::build_initial_state(l_storage);
    action_deque actions = coin_in_the_box::build_actions();

    agent a = language->get_agent_id("a");
    agent b = language->get_agent_id("b");
    agent c = language->get_agent_id("c");

    formula_ptr heads          = std::make_shared<atom_formula>(language->get_atom_id("heads"));
    formula_ptr not_heads      = std::make_shared<not_formula>(heads);
    formula_ptr D_a_heads      = std::make_shared<diamond_formula>(a, heads);
    formula_ptr D_a_not_heads  = std::make_shared<diamond_formula>(a, not_heads);
    formula_ptr not_Bw_a_heads = std::make_shared<and_formula>(formula_deque{D_a_heads, D_a_not_heads});
    formula_ptr goal_1         = std::make_shared<box_formula>(b, not_Bw_a_heads);

    formula_ptr B_c_heads      = std::make_shared<box_formula>(c, heads);
    formula_ptr B_c_not_heads  = std::make_shared<box_formula>(c, not_heads);
    formula_ptr Bw_c_heads     = std::make_shared<or_formula>(formula_deque{B_c_heads, B_c_not_heads});
    formula_ptr B_a_Bw_c_heads = std::make_shared<box_formula>(a, Bw_c_heads);
    formula_ptr goal_2         = std::make_shared<not_formula>(B_a_Bw_c_heads);

    formula_ptr B_b_heads      = std::make_shared<box_formula>(b, heads);

    formula_deque fs = {goal_1, goal_2, B_b_heads, B_c_heads};
    formula_ptr goal = std::make_shared<and_formula>(std::move(fs));

    return search::planning_task{std::move(domain_name), "cb_" + std::to_string(problem_id), language, std::move(s0), std::move(actions), std::move(goal)};
}

search::planning_task coin_in_the_box::build_task_5(label_storage &l_storage) {
    std::string domain_name = coin_in_the_box::get_name();
    unsigned long problem_id = 5;

    language_ptr language = coin_in_the_box::build_language();
    state s0 = coin_in_the_box::build_initial_state(l_storage);
    action_deque actions = coin_in_the_box::build_actions();

    agent a = language->get_agent_id("a");
    agent b = language->get_agent_id("b");
    agent c = language->get_agent_id("c");

    formula_ptr heads          = std::make_shared<atom_formula>(language->get_atom_id("heads"));
    formula_ptr not_heads      = std::make_shared<not_formula>(heads);
    formula_ptr D_a_heads      = std::make_shared<diamond_formula>(a, heads);
    formula_ptr D_a_not_heads  = std::make_shared<diamond_formula>(a, not_heads);
    formula_ptr not_Bw_a_heads = std::make_shared<and_formula>(formula_deque{D_a_heads, D_a_not_heads});
    formula_ptr goal_1         = std::make_shared<box_formula>(b, not_Bw_a_heads);

    formula_ptr B_a_heads      = std::make_shared<box_formula>(a, heads);
    formula_ptr goal_2         = std::make_shared<box_formula>(c, B_a_heads);

    formula_ptr B_c_heads      = std::make_shared<box_formula>(c, heads);
    formula_ptr B_b_heads      = std::make_shared<box_formula>(b, heads);

    formula_deque fs = {goal_1, goal_2, B_b_heads, B_c_heads};
    formula_ptr goal = std::make_shared<and_formula>(std::move(fs));

    return search::planning_task{std::move(domain_name), "cb_" + std::to_string(problem_id), language, std::move(s0), std::move(actions), std::move(goal)};
}

search::planning_task coin_in_the_box::build_task_6(label_storage &l_storage) {
    std::string domain_name = coin_in_the_box::get_name();
    unsigned long problem_id = 6;

    language_ptr language = coin_in_the_box::build_language();
    state s0 = coin_in_the_box::build_initial_state(l_storage);
    action_deque actions = coin_in_the_box::build_actions();

    formula_ptr heads          = std::make_shared<atom_formula>(language->get_atom_id("heads"));
    formula_ptr not_heads      = std::make_shared<not_formula>(heads);
    formula_deque fs;

    for (agent ag = 0; ag < language->get_agents_number(); ++ag) {
        formula_ptr B_ag_heads      = std::make_shared<box_formula>(ag, heads);
        fs.push_back(B_ag_heads);

        formula_ptr D_ag_heads      = std::make_shared<diamond_formula>(ag, heads);
        formula_ptr D_ag_not_heads  = std::make_shared<diamond_formula>(ag, not_heads);
        formula_ptr not_Bw_ag_heads = std::make_shared<and_formula>(formula_deque{D_ag_heads, D_ag_not_heads});
        fs.push_back(std::make_shared<box_formula>((ag + 1) % language->get_agents_number(), not_Bw_ag_heads));
    }

//    formula_ptr D_a_heads      = std::make_shared<diamond_formula>(a, heads);
//    formula_ptr D_a_not_heads  = std::make_shared<diamond_formula>(a, not_heads);
//    formula_ptr not_Bw_a_heads = std::make_shared<and_formula>(formula_deque{D_a_heads, D_a_not_heads});
//    formula_ptr goal_1         = std::make_shared<box_formula>(b, not_Bw_a_heads);
//
//    formula_ptr B_a_heads      = std::make_shared<box_formula>(a, heads);
//    formula_ptr goal_2         = std::make_shared<box_formula>(c, B_a_heads);
//
//    formula_ptr B_c_heads      = std::make_shared<box_formula>(c, heads);
//    formula_ptr B_b_heads      = std::make_shared<box_formula>(b, heads);

//    formula_deque fs = {goal_1, goal_2, B_b_heads, B_c_heads};
    formula_ptr goal = std::make_shared<and_formula>(std::move(fs));

    return search::planning_task{std::move(domain_name), "cb_" + std::to_string(problem_id), language, std::move(s0), std::move(actions), std::move(goal)};
}


action coin_in_the_box::build_open(const del::agent ag, const agent_set &fo_ags) {
    language_ptr language = coin_in_the_box::build_language();

    formula_ptr opened     = std::make_shared<atom_formula>(language->get_atom_id("opened"));
    formula_ptr not_opened = std::make_shared<not_formula>(opened);

    formula_deque fs = looking_agents(language, fo_ags);
    fs.push_back(not_opened);

    formula_ptr f_pre = std::make_shared<and_formula>(std::move(fs));
    event_post e_post;
    e_post[language->get_atom_id("opened")] = std::make_shared<true_formula>();

    std::string fo_str;
    to_string(fo_ags, fo_str);
    std::reverse(fo_str.begin(), fo_str.end());
    std::string name = "open_" + language->get_agent_name(ag) + "_" + fo_str;

    return action_builder::build_private_ontic(std::move(name), language, f_pre, e_post, fo_ags);
}

kripke::action coin_in_the_box::build_peek(const del::agent ag, const agent_set &po_ags) {
    language_ptr language = coin_in_the_box::build_language();

    agent_set fo_ags(language->get_agents_number());
    fo_ags[ag] = true;

    agent_set looking_ags = po_ags;
    looking_ags[ag] = true;

    formula_ptr opened = std::make_shared<atom_formula>(language->get_atom_id("opened"));
    formula_ptr sensed = std::make_shared<atom_formula>(language->get_atom_id("heads"));

    formula_deque fs = looking_agents(language, looking_ags);
    fs.push_back(opened);

    for (agent i = 0; i < language->get_agents_number(); ++i)
        if (looking_ags[i])
            fs.push_back(std::make_shared<box_formula>(i, opened));

    formula_ptr f_pre = std::make_shared<and_formula>(std::move(fs));

    std::string looking_str;
    to_string(looking_ags, looking_str);
    std::reverse(looking_str.begin(), looking_str.end());
    std::string name = "peek_" + language->get_agent_name(ag) + "_" + looking_str;

    return action_builder::build_semi_private_sensing(std::move(name), language, f_pre, sensed, fo_ags, po_ags);
}

kripke::action coin_in_the_box::build_shout(const agent ag, const agent_set &fo_ags) {
    language_ptr language = coin_in_the_box::build_language();

    formula_ptr announced         = std::make_shared<atom_formula>(language->get_atom_id("heads"));
    formula_ptr D_a_announced     = std::make_shared<box_formula>(ag, announced);

    formula_deque fs = looking_agents(language, fo_ags);
    fs.push_back(D_a_announced);
    formula_ptr f_pre = std::make_shared<and_formula>(fs);

    std::string fo_str;
    to_string(fo_ags, fo_str);
    std::reverse(fo_str.begin(), fo_str.end());
    std::string name = "shout_" + language->get_agent_name(ag) + "_" + fo_str;

    return action_builder::build_private_announcement(std::move(name), language, f_pre, fo_ags);
}

action coin_in_the_box::build_walk_in(del::agent ag, const agent_set &fo_ags) {
    language_ptr language = coin_in_the_box::build_language();
    std::string ag_name = language->get_agent_name(ag);

    atom        looking_ag  = language->get_atom_id("looking_" + ag_name);
    formula_ptr looking     = std::make_shared<atom_formula>(looking_ag);
    formula_ptr not_looking = std::make_shared<not_formula>(looking);
//    formula_ptr B_ag_not_looking = std::make_shared<box_formula>(ag, not_looking);

    formula_ptr f_pre  = std::move(not_looking);
    event_post e_post;
    e_post[looking_ag] = std::make_shared<true_formula>();

    std::string fo_str;
    to_string(fo_ags, fo_str);
    std::reverse(fo_str.begin(), fo_str.end());
    std::string name = "walk_in_" + language->get_agent_name(ag) + "_" + fo_str;

    return action_builder::build_private_ontic(std::move(name), language, f_pre, e_post, fo_ags);
}

action coin_in_the_box::build_walk_out(del::agent ag, const agent_set &fo_ags) {
    language_ptr language = coin_in_the_box::build_language();
    std::string ag_name = language->get_agent_name(ag);

    atom looking_ag = language->get_atom_id("looking_" + ag_name);
    formula_ptr looking = std::make_shared<atom_formula>(looking_ag);
//    formula_ptr B_ag_looking = std::make_shared<box_formula>(ag, looking);

    formula_ptr f_pre  = std::move(looking);
    event_post e_post;
    e_post[looking_ag] = std::make_shared<false_formula>();

    std::string fo_str;
    to_string(fo_ags, fo_str);
    std::reverse(fo_str.begin(), fo_str.end());
    std::string name = "walk_out_" + language->get_agent_name(ag) + "_" + fo_str;

    return action_builder::build_private_ontic(std::move(name), language, f_pre, e_post, fo_ags);
}

del::formula_deque coin_in_the_box::looking_agents(const language_ptr &language, const del::agent_set &ags) {
    formula_deque fs;

    for (agent ag = 0; ag < language->get_agents_number(); ++ag) {
        formula_ptr looking_ag = std::make_shared<atom_formula>(language->get_atom_id("looking_" + language->get_agent_name(ag)));

        if (ags[ag])
            fs.push_back(std::move(looking_ag));
        else
            fs.push_back(std::make_shared<not_formula>(std::move(looking_ag)));
    }

    return fs;
}

del::formula_deque coin_in_the_box::B_looking_agents(const language_ptr &language, const del::agent_set &ags, formula_deque &fs) {
    formula_deque B_fs;
    formula_ptr looking_ags = std::make_shared<and_formula>(std::move(fs));

    for (agent i = 0; i < language->get_agents_number(); ++i)
        if (ags[i])
            B_fs.push_back(std::make_shared<box_formula>(i, looking_ags));

    B_fs.push_back(std::move(looking_ags));
    return B_fs;
}
