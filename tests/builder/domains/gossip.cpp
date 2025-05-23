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

#include "gossip.h"
#include "domain_utils.h"
#include "../action_builder.h"
#include "../../../include/del/formulas/propositional/atom_formula.h"
#include "../../../include/del/formulas/modal/box_formula.h"
#include "../../../include/del/formulas/propositional/not_formula.h"
#include "../../../include/del/formulas/propositional/and_formula.h"
#include "ma_star_utils.h"
#include <filesystem>
#include <fstream>
#include <memory>
#include <ostream>
#include <string>
#include <utility>

using namespace daedalus::tester;
using namespace del;

std::string gossip::get_name() {
    return "gossip";
}

del::language_ptr gossip::build_language(unsigned long agents_no, unsigned long secrets_no) {
    name_vector atom_names(secrets_no), agent_names(agents_no);

    for (unsigned long i = 0; i < secrets_no; ++i)
        atom_names[i] = "s_" + std::to_string(i);

    for (unsigned long i = 0; i < agents_no; ++i)
        agent_names[i] = "ag_" + std::to_string(i);

    return std::make_shared<language>(std::move(language{atom_names, agent_names}));
}

kripke::state gossip::build_initial_state(unsigned long agents_no, unsigned long secrets_no, label_storage &l_storage) {
    language_ptr language = gossip::build_language(agents_no, secrets_no);

    const auto worlds_number = static_cast<const world_id>(std::exp2(secrets_no));
    world_id count = 0;

    label_vector ls = label_vector(worlds_number);
    auto secrets_combinations = domain_utils::all_combinations(secrets_no);

    for (const auto &combination: secrets_combinations) {
        label l = label{combination};
        ls[count++] = l_storage.emplace(std::move(l));
    }
    boost::dynamic_bitset<> all_worlds(worlds_number);
    all_worlds.set();

    relations r(language->get_agents_number());

    for (agent ag = 0; ag < language->get_agents_number(); ++ag) {
        r[ag] = agent_relation(worlds_number);

        for (world_id w = 0; w < worlds_number; ++w)
            r[ag][w] = world_bitset(all_worlds);
    }

    for (agent ag = 0; ag < secrets_no; ++ag)
        for (world_id w = 0; w < worlds_number; ++w)
            for (world_id v = 0; v < worlds_number; ++v) {
                label &lw = *l_storage.get(ls[w]), &lv = *l_storage.get(ls[v]);

                if (lw[ag] != lv[ag])
                    r[ag][w].remove(v);
            }

    world_id designated;

    for (world_id w = 0; w < worlds_number; ++w) {
        label &lw = *l_storage.get(ls[w]);
        if (lw.get_bitset().all()) designated = w;
    }

    world_bitset designated_worlds = world_bitset{worlds_number, world_set{designated}};

    return state{language, worlds_number, std::move(r), std::move(ls), std::move(designated_worlds)};
}

kripke::action_deque gossip::build_actions(unsigned long agents_no, unsigned long secrets_no) {
    action_deque actions;

    for (unsigned long ag_1 = 0; ag_1 < secrets_no; ++ag_1)
        for (unsigned long ag_2 = 0; ag_2 < agents_no; ++ag_2)
            if (ag_1 != ag_2)
                actions.push_back(std::make_shared<action>(build_tell(agents_no, secrets_no, ag_1, ag_2)));

    return actions;
}

search::planning_task gossip::build_task(unsigned long agents_no, unsigned long secrets_no, unsigned long goal_id, label_storage &l_storage) {
    std::string name = gossip::get_name();
    std::string id   = std::to_string(agents_no) + "_" + std::to_string(secrets_no) + "_" + std::to_string(goal_id);
    language_ptr language = gossip::build_language(agents_no, secrets_no);

    state s0 = gossip::build_initial_state(agents_no, secrets_no, l_storage);
    action_deque actions = gossip::build_actions(agents_no, secrets_no);
    formula_ptr goal = gossip::build_goal(agents_no, secrets_no, goal_id);

    return search::planning_task{std::move(name), std::move(id), language, std::move(s0), std::move(actions), std::move(goal)};
}

std::vector<search::planning_task> gossip::build_tasks(label_storage &l_storage) {
    const unsigned long N_MIN_AGS = 3, N_MAX_AGS = 7, N_MIN_SECRETS = 2, MIN_GOAL_ID = 1, MAX_GOAL_ID = 9;
    std::vector<search::planning_task> tasks;

    for (unsigned long agents_no = N_MIN_AGS; agents_no <= N_MAX_AGS; ++agents_no)
        for (unsigned long secrets_no = N_MIN_SECRETS; secrets_no <= agents_no; ++secrets_no)
            for (unsigned long goal_id = MIN_GOAL_ID; goal_id <= MAX_GOAL_ID; ++goal_id)
                tasks.push_back(build_task(agents_no, secrets_no, goal_id, l_storage));

    return tasks;
}

kripke::action gossip::build_tell(unsigned long agents_no, unsigned long secrets_no, del::agent ag_1, del::agent ag_2) {
    language_ptr language = gossip::build_language(agents_no, secrets_no);

    agent_set fo_ags = agent_set(language->get_agents_number());
    fo_ags[ag_1] = true;
    fo_ags[ag_2] = true;

    formula_ptr s_1 = std::make_shared<atom_formula>(ag_1);     // index of ag_1 is the same as index of s_1
    formula_ptr K_ag_2_s_1 = std::make_shared<box_formula>(ag_2, s_1);
    formula_ptr not_K_ag_2_s_1 = std::make_shared<not_formula>(K_ag_2_s_1);

    formula_deque fs = {s_1, not_K_ag_2_s_1};
    formula_ptr f_pre = std::make_shared<and_formula>(fs);

    std::string name = "tell_" + std::to_string(ag_1) + "_" + std::to_string(ag_2);

    return action_builder::build_private_announcement(std::move(name), language, f_pre, fo_ags);
}

del::formula_ptr gossip::build_goal(unsigned long agents_no, unsigned long secrets_no, unsigned long goal_id) {
    assert(1 <= goal_id and goal_id <= 9);

    language_ptr language = gossip::build_language(agents_no, secrets_no);
    formula_ptr goal;

    formula_deque fs_two, fs_some, fs_all;

    fs_two.push_back(std::make_shared<atom_formula>(0));
    fs_two.push_back(std::make_shared<atom_formula>(1));

    for (del::agent ag = 0; ag < secrets_no; ++ag) {
        if (ag < secrets_no / 2)
            fs_some.push_back(std::make_shared<atom_formula>(ag));

        fs_all.push_back(std::make_shared<atom_formula>(ag));
    }

    formula_ptr two_secrets  = std::make_shared<and_formula>(std::move(fs_two));
    formula_ptr some_secrets = std::make_shared<and_formula>(std::move(fs_some));
    formula_ptr all_secrets  = std::make_shared<and_formula>(std::move(fs_all));

    formula_deque fs;

    if (goal_id <= 3) {
        for (del::agent ag = 0; ag <= 1; ++ag)
            fs.push_back(
                    std::make_shared<box_formula>(ag, (goal_id == 1 ? two_secrets  :    // 2 agents know 2 secrets
                                                      (goal_id == 2 ? some_secrets :    // 2 agents know some secrets
                                                      all_secrets))));                  // 2 agents know all secrets
    } else if (goal_id <= 6) {
        for (del::agent ag = 0; ag < language->get_agents_number() / 3; ++ag)
            fs.push_back(
                    std::make_shared<box_formula>(ag, (goal_id == 4 ? two_secrets  :    // Some agents know 2 secrets
                                                      (goal_id == 5 ? some_secrets :    // Some agents know some secrets
                                                      all_secrets))));                  // Some agents know all secrets
    } else if (goal_id <= 9) {
        for (del::agent ag = 0; ag < language->get_agents_number(); ++ag)
            fs.push_back(
                    std::make_shared<box_formula>(ag, (goal_id == 7 ? two_secrets  :    // All agents know 2 secrets
                                                      (goal_id == 8 ? some_secrets :    // All agents know some secrets
                                                      all_secrets))));                  // All agents know all secrets
    }

    return std::make_shared<and_formula>(std::move(fs));
}

void gossip::write_ma_star_problem(unsigned long agents_no, unsigned long secrets_no, unsigned long goal_id,
                                   label_storage &l_storage) {
    auto task = build_task(agents_no, secrets_no, goal_id, l_storage);
    std::string path = "../tests/builder/domains/ma_star/" + task.get_domain_name() + "/";
    std::string name = task.get_domain_name() + "_" + task.get_problem_id();
    std::string ext  = ".txt";

    if (not std::filesystem::exists(path))
        std::filesystem::create_directories(path);

    std::ofstream out = std::ofstream{path + name + ext};

    ma_star_utils::print_atoms(out, task);
    ma_star_utils::print_action_names(out, task);
    ma_star_utils::print_agents(out, task);

    out << std::endl << std::endl;

    for (unsigned long ag_1 = 0; ag_1 < secrets_no; ++ag_1)
        for (unsigned long ag_2 = 0; ag_2 < agents_no; ++ag_2)
            if (ag_1 != ag_2) {
                std::string act_name = "tell_" + std::to_string(ag_1) + "_" + std::to_string(ag_2);

                formula_ptr s_1 = std::make_shared<atom_formula>(ag_1);     // index of ag_1 is the same as index of s_1
                formula_ptr K_ag_2_s_1 = std::make_shared<box_formula>(ag_2, s_1);
                formula_ptr not_K_ag_2_s_1 = std::make_shared<not_formula>(K_ag_2_s_1);

                out << "executable " << act_name << " if ";
                ma_star_utils::print_formula(out, task.get_language(), not_K_ag_2_s_1);
                out << " ;" << std::endl;

                out << act_name << " announces ";
                ma_star_utils::print_formula(out, task.get_language(), s_1);
                out << " ;" << std::endl;

                out << task.get_language()->get_agent_name(ag_1) << " observes " << act_name << ";" << std::endl;
                out << task.get_language()->get_agent_name(ag_2) << " observes " << act_name << ";" << std::endl << std::endl;
            }

    out << "\ninitially ";
    for (auto s = 0; s < secrets_no; ++s)
        out << task.get_language()->get_atom_name(s) << (s+1 < secrets_no ? ", " : ";\n");

    std::string all_agents = "[";
    for (auto ag = 0; ag < agents_no; ++ag)
        all_agents += task.get_language()->get_agent_name(ag) + (ag+1 < agents_no ? ", " : "]");

    for (auto s = 0; s < secrets_no; ++s) {
        out
            << "initially C(" << all_agents << ", "
            << "( ( B( " << task.get_language()->get_agent_name(s) << " , "  << task.get_language()->get_atom_name(s) << " ) ) | "
            << "( B( " << task.get_language()->get_agent_name(s) << " , -" << task.get_language()->get_atom_name(s) << " ) ) ) "
            << ");\n";
    }

    out << std::endl << "goal ";
    ma_star_utils::print_formula(out, task.get_language(), task.get_goal());
    out << ";" << std::endl;
}
