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

#include "../../../../../include/del/semantics/kripke/bisimulation/bisimulator.h"
#include "../../../../../include/del/semantics/kripke/bisimulation/partition_refinement.h"
#include "../../../../../include/del/semantics/kripke/bisimulation/bounded_contraction_builder.h"
#include "../../../../../include/del/semantics/kripke/bisimulation/bounded_partition_refinement.h"
#include "../../../../../include/del/semantics/kripke/bisimulation/bounded_identification.h"
#include <algorithm>

using namespace kripke;

std::pair<bool, state>
bisimulator::contract(contraction_type type, const state &s, unsigned long k, const del::storages_ptr &storages) {
    switch (type) {
        case contraction_type::full:
            return bounded_contraction_builder::calculate_standard_contraction(s, storages);
        case contraction_type::rooted:  // todo: std::min(k, s.get_max_depth()+1)
            return bounded_contraction_builder::calculate_rooted_contraction(s, k);
        case contraction_type::canonical:
            return bounded_contraction_builder::calculate_rooted_contraction(s, k, true, storages);
    }
}

bool bisimulator::are_bisimilar(const state &s, const state &t, unsigned long k, const del::storages_ptr &storages) {
    state u = disjoint_union(s, t);
    unsigned long offset = s.get_worlds_number();

    auto [is_bisim, structures] = bounded_partition_refinement::do_refinement_steps(u, k);
    auto worlds_blocks = structures.worlds_blocks;
    bool are_bisimilar, same_id;

    for (auto wd: s.get_designated_worlds())
        if (std::all_of(t.get_designated_worlds().begin(), t.get_designated_worlds().end(),
                    [&](world_id vd) -> bool {
                        auto block_wd = worlds_blocks[wd][k];
                        return std::find(block_wd->begin(), block_wd->end(), offset + vd) == block_wd->end();}))
            are_bisimilar = false;

    for (auto vd: t.get_designated_worlds())
        if (std::all_of(s.get_designated_worlds().begin(), s.get_designated_worlds().end(),
                        [&](world_id wd) -> bool {
                            auto block_vd = worlds_blocks[offset + vd][k];
                            return std::find(block_vd->begin(), block_vd->end(), wd) == block_vd->end();}))
            are_bisimilar = false;
/*
        for (auto vd: t.get_designated_worlds())
            if (auto block_wd = structures.worlds_blocks[wd][k],
                     block_vd = structures.worlds_blocks[s.get_worlds_number() + vd][k];
                std::find(block_wd->begin(), block_wd->end(), s.get_worlds_number() + vd) == block_wd->end())
                return false;

    for (auto vd: t.get_designated_worlds())
        for (auto wd: s.get_designated_worlds())
            if (auto block_wd = structures.worlds_blocks[wd][k],
                     block_vd = structures.worlds_blocks[s.get_worlds_number() + vd][k];
                std::find(block_vd->begin(), block_vd->end(), wd) == block_vd->end())
                return false;*/

    are_bisimilar = true;
    same_id = bounded_identification::calculate_state_id(s, k, storages) == bounded_identification::calculate_state_id(t, k, storages);

    assert(are_bisimilar == same_id);
    return are_bisimilar;
}

/*std::tuple<bool, state, bpr_structures> bisimulator::resume_contraction(contraction_type type, const state &s, unsigned long k,
                                                       bpr_structures &structures, const del::storages_ptr &storages) {
    return bounded_contraction_builder::update_rooted_contraction(s, k, structures, type == contraction_type::canonical, storages);
}*/

state bisimulator::disjoint_union(const kripke::state &s, const kripke::state &t) {
    unsigned long worlds_number = s.get_worlds_number() + t.get_worlds_number(), offset = s.get_worlds_number();

    relations r = relations(s.get_language()->get_agents_number());

    for (del::agent ag = 0; ag < s.get_language()->get_agents_number(); ++ag) {
        r[ag] = agent_relation(worlds_number);

        for (world_id w = 0; w < s.get_worlds_number(); ++w) {
            r[ag][w] = world_bitset(worlds_number);

            for (auto v: s.get_agent_possible_worlds(ag, w))
                r[ag][w].push_back(v);
        }

        for (world_id w = 0; w < t.get_worlds_number(); ++w) {
            r[ag][offset + w] = world_bitset(worlds_number);

            for (auto v: t.get_agent_possible_worlds(ag, w))
                r[ag][offset + w].push_back(offset + v);
        }
    }

    label_vector ls = label_vector(worlds_number);

    for (world_id w = 0; w < s.get_worlds_number(); ++w)
        ls[w] = s.get_label_id(w);

    for (world_id w = 0; w < t.get_worlds_number(); ++w)
        ls[offset + w] = t.get_label_id(w);

    world_bitset designated = world_bitset(worlds_number);

    for (auto w : s.get_designated_worlds())
        designated.push_back(w);

    for (auto w : t.get_designated_worlds())
        designated.push_back(offset + w);

    return state{s.get_language(), worlds_number, std::move(r), std::move(ls), std::move(designated)};
}
