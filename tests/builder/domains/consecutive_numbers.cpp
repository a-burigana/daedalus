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

#include "consecutive_numbers.h"
#include "../../../include/del/formulas/propositional/atom_formula.h"
#include "../../../include/del/formulas/modal/box_formula.h"
#include "../../../include/del/formulas/propositional/or_formula.h"
#include "../../../include/del/formulas/propositional/not_formula.h"
#include "../action_builder.h"
#include "../../../include/del/formulas/propositional/and_formula.h"
#include <memory>
#include <string>

using namespace daedalus::tester;
using namespace del;

std::string consecutive_numbers::get_name() {
    return "consecutive_numbers";
}

del::language_ptr consecutive_numbers::build_language(unsigned long n) {
    name_vector atom_names(2 * (n + 1)), agent_names = {"a", "b"};

    for (unsigned long i = 0; i <= n; ++i) {
        atom_names[i] = "has_a_" + std::to_string(i);
        atom_names[n + i + 1] = "has_b_" + std::to_string(i);
    }

    return std::make_shared<language>(std::move(language{atom_names, agent_names}));
}

kripke::state consecutive_numbers::build_initial_state(unsigned long n) {
    language_ptr language = consecutive_numbers::build_language(n);

    world_id worlds_number = n;
    world_set designated_worlds = world_set{worlds_number, world_list{0, 1}};        // n/2      // We assume n >= 2

    relations r(language->get_agents_number());

    for (agent ag = 0; ag < language->get_agents_number(); ++ag) {
        r[ag] = agent_relation(worlds_number);

        for (world_id w = 0; w < worlds_number; ++w)
            r[ag][w] = world_set(worlds_number);
    }

    for (world_id w = 0; w < worlds_number; ++w) {
        r[0][w].push_back(w);
        r[1][w].push_back(w);

        agent ag = w % 2;
        if (w > 0) r[ag][w].push_back(w - 1);
        if (w < worlds_number - 1) r[1 - ag][w].push_back(w + 1);
    }

    label_vector ls = label_vector(worlds_number);

    for (world_id w = 0; w < worlds_number; ++w) {
        boost::dynamic_bitset<> lw(language->get_atoms_number());
        agent ag1 = w % 2, ag2 = 1 - ag1;

        atom has_ag1_n = language->get_atom_id("has_" + language->get_agent_name(ag1) + "_" + std::to_string(n - w));
        atom has_ag2_n = language->get_atom_id(
                "has_" + language->get_agent_name(ag2) + "_" + std::to_string(n - w - 1));
        lw[has_ag1_n] = true;
        lw[has_ag2_n] = true;

        ls[w] = label{std::move(lw)};
    }

    return state{language, worlds_number, std::move(r), std::move(ls), std::move(designated_worlds)};
}

action_deque consecutive_numbers::build_actions(unsigned long n) {
    language_ptr language = consecutive_numbers::build_language(n);
    action_deque actions;

    agent a = language->get_agent_id("a");
    agent b = language->get_agent_id("b");

    formula_deque fs_a, fs_b;

    for (unsigned long k = 0; k <= n; ++k) {
        formula_ptr has_a_k     = std::make_shared<atom_formula>(language->get_atom_id("has_a_" + std::to_string(k)));
        formula_ptr has_b_k     = std::make_shared<atom_formula>(language->get_atom_id("has_b_" + std::to_string(k)));

//        formula_ptr B_a_has_a_k = std::make_shared<box_formula>(a, has_a_k);
        formula_ptr B_a_has_b_k = std::make_shared<box_formula>(a, has_b_k);
        formula_ptr B_b_has_a_k = std::make_shared<box_formula>(b, has_a_k);
//        formula_ptr B_b_has_b_k = std::make_shared<box_formula>(b, has_b_k);

        formula_ptr f_a         = std::make_shared<not_formula>(B_a_has_b_k);
        formula_ptr f_b         = std::make_shared<not_formula>(B_b_has_a_k);

        actions.push_back(std::make_shared<action>(std::move(action_builder::build_public_announcement("a_not_knows_b_" + std::to_string(k), language, f_a))));
        actions.push_back(std::make_shared<action>(std::move(action_builder::build_public_announcement("b_not_knows_a_" + std::to_string(k), language, f_b))));
//        formula_ptr f_b         = std::make_shared<and_formula>(formula_deque{B_b_has_a_k, B_a_has_a_k});

//        fs_a.push_back(std::move(f_a));
//        fs_b.push_back(std::move(f_b));
    }
//
//    formula_ptr psi_a = std::make_shared<or_formula>(std::move(fs_a));
//    formula_ptr psi_b = std::make_shared<or_formula>(std::move(fs_b));
//
//    formula_ptr not_psi_a = std::make_shared<not_formula>(psi_a);
//    formula_ptr not_psi_b = std::make_shared<not_formula>(psi_b);
//
////    actions.push_back(std::make_shared<action>(std::move(action_builder::build_public_announcement("a_knows_b_n", language, psi_a))));
////    actions.push_back(std::make_shared<action>(std::move(action_builder::build_public_announcement("b_knows_a_n", language, psi_b))));
//    actions.push_back(std::make_shared<action>(std::move(action_builder::build_public_announcement("a_not_knows_b_n", language, not_psi_a))));
//    actions.push_back(std::make_shared<action>(std::move(action_builder::build_public_announcement("b_not_knows_a_n", language, not_psi_b))));

    /*for (unsigned long k = 0; k <= n; ++k) {
        formula_ptr has_a_k     = std::make_shared<atom_formula>(language->get_atom_id("has_a_" + std::to_string(k)));
        formula_ptr has_b_k     = std::make_shared<atom_formula>(language->get_atom_id("has_b_" + std::to_string(k)));

        formula_ptr B_a_has_b_k = std::make_shared<box_formula>(a, has_b_k);
        formula_ptr B_b_has_a_k = std::make_shared<box_formula>(b, has_a_k);

        formula_ptr f_a         = std::make_shared<and_formula>(formula_deque{B_a_has_b_k, has_b_k});
        formula_ptr f_b         = std::make_shared<and_formula>(formula_deque{B_b_has_a_k, has_a_k});

        formula_ptr not_f_a     = std::make_shared<not_formula>(f_a);
        formula_ptr not_f_b     = std::make_shared<not_formula>(f_b);

        actions.push_back(std::make_shared<action>(std::move(action_builder::build_public_announcement("a_not_knows_b_" + std::to_string(k), language, not_f_a))));
        actions.push_back(std::make_shared<action>(std::move(action_builder::build_public_announcement("b_not_knows_a_" + std::to_string(k), language, not_f_b))));
    }

    formula_ptr has_b_n_minus_1 = std::make_shared<atom_formula>(language->get_atom_id("has_b_" + std::to_string(n-1)));
    formula_ptr has_a_n_minus_2 = std::make_shared<atom_formula>(language->get_atom_id("has_a_" + std::to_string(n-2)));

    formula_ptr B_a_has_b_n_minus_1 = std::make_shared<box_formula>(a, has_b_n_minus_1);
    formula_ptr B_b_has_a_n_minus_2 = std::make_shared<box_formula>(b, has_a_n_minus_2);

    formula_ptr f_a = std::make_shared<and_formula>(formula_deque{B_a_has_b_n_minus_1, has_a_n_minus_2, has_b_n_minus_1});
    formula_ptr f_b = std::make_shared<and_formula>(formula_deque{B_b_has_a_n_minus_2, has_b_n_minus_1, has_a_n_minus_2});

    actions.push_back(std::make_shared<action>(std::move(action_builder::build_public_announcement("tell_a", language, f_a))));
    actions.push_back(std::make_shared<action>(std::move(action_builder::build_public_announcement("tell_b", language, f_b))));*/


//    if (k < n) {
//        formula_ptr has_a_k_plus_1 = std::make_shared<atom_formula>(language->get_atom_id("has_a_" + std::to_string(k+1)));
//        formula_ptr has_b_k_plus_1 = std::make_shared<atom_formula>(language->get_atom_id("has_b_" + std::to_string(k+1)));
//
//        formula_ptr f_a_1 = std::make_shared<and_formula>(formula_deque{B_a_has_b_k, has_a_k_plus_1});
//        formula_ptr f_b_1 = std::make_shared<and_formula>(formula_deque{B_b_has_a_k, has_b_k_plus_1});
//
//        actions.push_back(std::make_shared<action>(std::move(action_builder::build_public_announcement("a_tells_plus_" + std::to_string(k+1), language, f_a_1))));
//        actions.push_back(std::make_shared<action>(std::move(action_builder::build_public_announcement("b_tells_plus_" + std::to_string(k+1), language, f_b_1))));
//    }
    return actions;
}

search::planning_task consecutive_numbers::build_task(unsigned long n) {
    language_ptr language = consecutive_numbers::build_language(n);

    state s0 = consecutive_numbers::build_initial_state(n);
    action_deque actions = consecutive_numbers::build_actions(n);

    agent a = language->get_agent_id("a");
    agent b = language->get_agent_id("b");

    unsigned long n_a = n, n_b = n - 1;
//    unsigned long n_ag = n/2 % 2 == 0 ? n - n/2 : n - n/2 - 1;

//    formula_ptr has_a_n     = std::make_shared<atom_formula>(language->get_atom_id("has_a_" + std::to_string(n_a)));
//    formula_ptr has_b_n     = std::make_shared<atom_formula>(language->get_atom_id("has_b_" + std::to_string(n_b)));
//    formula_ptr B_a_has_b_n = std::make_shared<box_formula>(a, std::move(has_b_n));
//    formula_ptr B_b_has_a_n = std::make_shared<box_formula>(b, std::move(has_a_n));
//    formula_ptr goal        = std::make_shared<and_formula>(formula_deque{std::move(B_a_has_b_n), std::move(B_b_has_a_n)});

    formula_ptr has_b_n_b     = std::make_shared<atom_formula>(language->get_atom_id("has_b_" + std::to_string(n_b)));
    formula_ptr B_a_has_b_n_b = std::make_shared<box_formula>(a, std::move(has_b_n_b));
    formula_ptr goal          = std::make_shared<box_formula>(b, std::move(B_a_has_b_n_b));

    return search::planning_task{consecutive_numbers::get_name(), std::to_string(n), language, std::move(s0), std::move(actions), std::move(goal)};
}

std::vector<search::planning_task> consecutive_numbers::build_tasks() {
    const unsigned long N_MIN_TASKS = 2, N_MAX_TASK = 5;
    std::vector<search::planning_task> tasks;

    for (unsigned long n = N_MIN_TASKS; n <= N_MAX_TASK; ++n)
        tasks.emplace_back(consecutive_numbers::build_task(n));

    return tasks;
}