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

#include <queue>
#include <utility>
#include "../../../../../include/del/formulas/formula_types.h"
#include "../../../../../include/del/semantics/kripke/update/updater.h"
#include "../../../../../include/del/semantics/kripke/model_checker.h"
#include "../../../../../include/del/semantics/kripke/bisimulation/bisimulator.h"
#include "../../../../../include/utils/storage.h"

using namespace kripke;

bool updater::is_applicable(const state &s, const action &a, const del::label_storage &l_storage) {
    const auto check = [&](const world_id wd) { return is_applicable_world(s, a, wd, l_storage); };
    return std::all_of(s.get_designated_worlds().begin(), s.get_designated_worlds().end(), check);
}

bool updater::is_applicable_world(const state &s, const action &a, const world_id wd, const del::label_storage &l_storage) {
    const auto check = [&](const event_id ed) { return model_checker::holds_in(s, wd, *a.get_precondition(ed), l_storage); };
    return std::any_of(a.get_designated_events().begin(), a.get_designated_events().end(), check);
}

state updater::product_update(const state &s, const action_deque &as, del::storages_handler_ptr handler,
                              bool apply_contraction, contraction_type type, const unsigned long k) {
    state s_ = product_update(s, *as.front(), handler->get_label_storage());

    if (apply_contraction)
        s_ = std::get<1>(bisimulator::contract(type, s_, k, handler));

    action_deque as_ = as;
    as_.pop_front();
    return as_.empty() ? std::move(s_) : product_update(s_, as_, handler, apply_contraction, type, k);
}

state updater::product_update(const state &s, const action &a, del::label_storage &l_storage) {
    updated_worlds_map w_map;
    updated_edges_vector r_map(s.get_language()->get_agents_number());

    auto [worlds_number, designated_worlds] = calculate_worlds(s, a, w_map, r_map, l_storage);
    relations r = calculate_relations(s, a, worlds_number, w_map, r_map);
    label_vector labels = calculate_labels(s, a, worlds_number, w_map, l_storage);

    return state{s.get_language(), worlds_number, std::move(r), std::move(labels), std::move(designated_worlds)};
}

std::pair<world_id, world_bitset> updater::calculate_worlds(const state &s, const action &a, updated_worlds_map &w_map,
                                                            updated_edges_vector &r_map, del::label_storage &l_storage) {
    world_id worlds_number = 0;
    world_set designated_worlds;

    std::unordered_set<updated_world> to_expand;
    std::unordered_set<updated_world> visited;

    for (del::agent ag = 0; ag < s.get_language()->get_agents_number(); ++ag)
        r_map[ag] = updated_world_pair_deque{};

    for (const world_id wd : s.get_designated_worlds())
        for (const event_id ed : a.get_designated_events())
            if (model_checker::holds_in(s, wd, *a.get_precondition(ed), l_storage))
                to_expand.emplace(wd, ed);

    while (not to_expand.empty()) {
        const auto first = to_expand.begin();
        const auto &[w, e] = *first;
        w_map[updated_world{w, e}] = worlds_number++;
        visited.emplace(w, e);

        if (s.is_designated(w) and a.is_designated(e))
            designated_worlds.emplace(worlds_number-1);

        for (del::agent ag = 0; ag < s.get_language()->get_agents_number(); ++ag) {
            const world_bitset &ag_worlds = s.get_agent_possible_worlds(ag, w);
            const event_bitset &ag_events = a.get_agent_possible_events(ag, e);

            for (const world_id v : ag_worlds) {
                for (const event_id f : ag_events) {
                    if (model_checker::holds_in(s, v, *a.get_precondition(f), l_storage)) {
                        updated_world w_ = {w, e}, v_ = {v, f};
                        r_map[ag].emplace_back(w_, v_);

                        if (visited.find(v_) == visited.end())
                            to_expand.emplace(v_);
                    }
                }
            }
        }
        to_expand.erase(first);
    }
    return {worlds_number, world_bitset{worlds_number, std::move(designated_worlds)}};
}

relations updater::calculate_relations(const state &s, const action &a, const world_id worlds_number,
                                       const updated_worlds_map &w_map, const updated_edges_vector &r_map) {
    relations r = relations(s.get_language()->get_agents_number());

    for (del::agent ag = 0; ag < s.get_language()->get_agents_number(); ++ag) {
        r[ag] = agent_relation(worlds_number);

        for (world_id w = 0; w < worlds_number; ++w)
            r[ag][w] = world_bitset(worlds_number);
    }

    for (del::agent ag = 0; ag < s.get_language()->get_agents_number(); ++ag) {
        for (const auto &[w_, v_] : r_map[ag]) {
            const auto &[w, e] = w_;
            const auto &[v, f] = v_;

            if (s.has_edge(ag, w, v) and a.has_edge(ag, e, f))
                r[ag][w_map.at(w_)].push_back(w_map.at(v_));
        }
    }
    return r;
}

label_vector updater::calculate_labels(const state &s, const action &a, const world_id worlds_number,
                                       const updated_worlds_map &w_map, del::label_storage &l_storage) {
    label_vector labels = label_vector(worlds_number);

    for (const auto &[w_, w_id] : w_map) {
        const auto &[w, e] = w_;
        labels[w_id] = a.is_ontic(e) ? update_world(s, w, a, e, l_storage) : s.get_label_id(w);
    }
    return labels;
}

label_id updater::update_world(const state &s, const world_id &w, const action &a, const event_id &e,
                               del::label_storage &l_storage) {
    auto bitset = l_storage.get(s.get_label_id(w))->get_bitset();

    for (const auto &[p, post] : a.get_postconditions(e))
        bitset[p] = model_checker::holds_in(s, w, *post, l_storage);

    return l_storage.emplace(del::label{std::move(bitset)});
}
