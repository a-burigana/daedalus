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

#include "active_muddy_children.h"
#include "domain_utils.h"
#include "../action_builder.h"
#include "../../../include/del/formulas/propositional/atom_formula.h"
#include "../../../include/del/formulas/modal/box_formula.h"
#include "../../../include/del/formulas/propositional/not_formula.h"
#include "../../../include/del/formulas/propositional/or_formula.h"
#include "../../../include/del/formulas/propositional/true_formula.h"
#include <memory>
#include <string>

using namespace daedalus::tester;
using namespace del;

std::string active_muddy_children::get_name() {
    return "active_muddy_children";
}

del::language_ptr active_muddy_children::build_language(unsigned long children_no, unsigned long muddy_no) {
    name_vector atom_names(children_no), agent_names(children_no);

    for (unsigned long i = 0; i < children_no; ++i) {
        atom_names[i] = "muddy_" + std::to_string(i);
        agent_names[i] = "child_" + std::to_string(i);
    }

    return std::make_shared<language>(std::move(language{atom_names, agent_names}));
}

kripke::state
active_muddy_children::build_initial_state(unsigned long children_no, unsigned long muddy_no, bool is_0_muddy, const label_storage_ptr &l_storage) {
    assert(children_no >= muddy_no and muddy_no > 0);

    language_ptr language = active_muddy_children::build_language(children_no, muddy_no);

    const auto worlds_number = static_cast<const world_id>(std::exp2(children_no)) - 1;
    world_id count = 0, designated = 0;

    //  muddy_0, ...,  muddy_k, ~muddy_{k+1}, ..., ~muddy_n,    if is_0_muddy
    // ~muddy_0, ..., ~muddy_k,  muddy_{k+1}, ...,  muddy_n,    otherwise
    boost::dynamic_bitset<> actual_muddy_children(children_no);
    unsigned long min_child = is_0_muddy ? 0 : children_no - muddy_no, max_child = is_0_muddy ? muddy_no : children_no;

    for (agent ag = min_child; ag < max_child; ++ag)
        actual_muddy_children[ag] = true;

    const auto ag_combinations = domain_utils::all_non_empty_combinations(children_no);
    label_vector ls = label_vector(worlds_number);

    for (const auto &combination: ag_combinations) {
        if (combination == actual_muddy_children)
            designated = count;

        label l = label{combination};
        ls[count++] = l_storage->emplace(std::move(l));
    }

    relations r(language->get_agents_number());

    for (agent ag = 0; ag < language->get_agents_number(); ++ag) {
        r[ag] = agent_relation(worlds_number);

        for (world_id w = 0; w < worlds_number; ++w)
            r[ag][w] = world_set(worlds_number);
    }

    for (agent ag = 0; ag < children_no; ++ag)
        for (world_id w = 0; w < worlds_number; ++w)
            for (world_id v = 0; v < worlds_number; ++v) {
                bool is_good_edge = true;

                for (agent ag_1 = 0; ag_1 < children_no; ++ag_1) {
                    label &lw = *l_storage->get(ls[w]), &lv = *l_storage->get(ls[v]);

                    if ((ag_1 == ag) == (lw[ag_1] == lv[ag_1]))
                        is_good_edge = false;
                }

                if (w == v or is_good_edge)
                    r[ag][w].push_back(v);
            }

    world_set designated_worlds = world_set{worlds_number, world_deque{designated}};

    return state{language, worlds_number, std::move(r), std::move(ls), std::move(designated_worlds)};
}

kripke::action_deque active_muddy_children::build_actions(unsigned long children_no, unsigned long muddy_no) {
    action_deque actions;

    for (agent ag = 1; ag < children_no; ++ag)
        actions.push_back(std::make_shared<action>(build_ask(children_no, muddy_no, ag)));

    return actions;
}

search::planning_task active_muddy_children::build_task(unsigned long children_no, unsigned long muddy_no, bool is_0_muddy, const label_storage_ptr &l_storage) {
    std::string name = active_muddy_children::get_name();
    std::string id   = std::to_string(children_no) + "_" + std::to_string(muddy_no) + "_" + std::to_string(is_0_muddy);
    language_ptr language = active_muddy_children::build_language(children_no, muddy_no);

    state s0 = active_muddy_children::build_initial_state(children_no, muddy_no, is_0_muddy, l_storage);
    action_deque actions = active_muddy_children::build_actions(children_no, muddy_no);

    formula_ptr muddy_0     = std::make_shared<atom_formula>(0);
    formula_ptr not_muddy_0 = std::make_shared<not_formula>(muddy_0);

    formula_ptr K_0_muddy_0     = std::make_shared<box_formula>(0, muddy_0);
    formula_ptr K_0_not_muddy_0 = std::make_shared<box_formula>(0, not_muddy_0);

    formula_deque fs  = {std::move(K_0_muddy_0), std::move(K_0_not_muddy_0)};
    formula_ptr goal = std::make_shared<or_formula>(std::move(fs));

    return search::planning_task{std::move(name), std::move(id), language, std::move(s0), std::move(actions), std::move(goal)};
}

std::vector<search::planning_task> active_muddy_children::build_tasks(const label_storage_ptr &l_storage) {
    const unsigned long N_MIN_CHILDREN = 3, N_MAX_CHILDREN = 12, N_MIN_MUDDY = 1;
    std::vector<search::planning_task> tasks;

    for (unsigned long children_no = N_MIN_CHILDREN; children_no <= N_MAX_CHILDREN; ++children_no)
        for (unsigned long muddy_no = N_MIN_MUDDY; muddy_no <= children_no; ++muddy_no) {
            tasks.push_back(build_task(children_no, muddy_no, true, l_storage));
            if (muddy_no < children_no)
                tasks.push_back(build_task(children_no, muddy_no, false, l_storage));
        }

    return tasks;
}

kripke::action active_muddy_children::build_ask(unsigned long children_no, unsigned long muddy_no, del::agent ag) {
    language_ptr language = active_muddy_children::build_language(children_no, muddy_no);

    formula_ptr f_pre = std::make_shared<true_formula>();

    formula_ptr muddy_ag     = std::make_shared<atom_formula>(ag);
    formula_ptr not_muddy_ag = std::make_shared<not_formula>(muddy_ag);

    formula_ptr K_ag_muddy_ag     = std::make_shared<box_formula>(ag, muddy_ag);
    formula_ptr K_ag_not_muddy_ag = std::make_shared<box_formula>(ag, not_muddy_ag);

    formula_deque fs  = {std::move(K_ag_muddy_ag), std::move(K_ag_not_muddy_ag)};
    formula_ptr sensed = std::make_shared<or_formula>(std::move(fs));

    std::string name = "ask_" + std::to_string(ag);

    return action_builder::build_public_sensing(std::move(name), language, f_pre, sensed);
}
