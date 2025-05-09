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

#include "../../../../../include/del/semantics/kripke/bisimulation/bounded_contraction_builder.h"
#include "../../../../../include/del/semantics/kripke/bisimulation/bounded_partition_refinement.h"
#include "../../../../../include/del/semantics/kripke/states/state.h"
#include "../../../../../include/utils/storage.h"
#include "../../../../../include/del/semantics/kripke/bisimulation/bounded_identification.h"

using namespace kripke;

std::pair<bool, state> bounded_contraction_builder::calculate_rooted_contraction(const state &s, unsigned long k, bool canonical,
                                                                                 const del::storages_ptr &storages) {
    const auto &[is_bisim, worlds_blocks] = bounded_partition_refinement::do_refinement_steps(s, k+1);
    // We first calculate the maximal representatives: worlds_max_reprs[x] is the maximal representative of x
    auto worlds_max_reprs = calculate_max_representatives(s, k, worlds_blocks);

    // We now create the set 'max_reprs_set' of maximal representatives out of the vector 'worlds_max_reprs'.
    auto max_reprs_set = std::unordered_set<world_id>{worlds_max_reprs.begin(), worlds_max_reprs.end()};
    world_id bounded_worlds_number = max_reprs_set.size();

    std::vector<world_id> contracted_worlds_map = std::vector<world_id>(s.get_worlds_number());
    auto max_reprs_bitset = boost::dynamic_bitset<>(s.get_worlds_number());

    for (const world_id x : max_reprs_set)      // We keep track in max_reprs_bitset of all maximal representatives x
        max_reprs_bitset[x].flip();             // of our model (namely, all worlds x s.t. max_reprs_bitset[x] = 1)

    relations    quotient_r = relations(s.get_language()->get_agents_number());
    label_vector quotient_v = label_vector(bounded_worlds_number);

    world_id count = 0;

    for (const world_id w : max_reprs_set) {
        contracted_worlds_map[w] = count;
        quotient_v[count++] = s.get_label_id(w);
    }

    for (del::agent ag = 0; ag < s.get_language()->get_agents_number(); ++ag) {
        quotient_r[ag] = agent_relation(bounded_worlds_number);

        for (world_id w = 0; w < bounded_worlds_number; ++w)
            quotient_r[ag][w] = world_bitset(bounded_worlds_number);
    }

    for (const world_id x : max_reprs_set) {
        if (k > s.get_depth(x)) {
            unsigned long b_x = k - s.get_depth(x);

            for (del::agent ag = 0; ag < s.get_language()->get_agents_number(); ++ag) {
                for (const world_id y : s.get_agent_possible_worlds(ag, x))
                    if (s.get_depth(y) <= k) {
                        auto y_reprs = max_reprs_bitset & worlds_blocks[y][b_x-1]->get_bitset();    // We compute the maximal representatives of y in its (b(x)-1)-block
                        world_id f_y = y_reprs.find_first();                                        // We take the minimal one, namely, its (b(x)-1)-canonical representative
                        quotient_r[ag][contracted_worlds_map[x]].push_back(contracted_worlds_map[f_y]);
                    }
            }
        }
    }

    world_bitset designated_worlds(bounded_worlds_number);

    for (world_id wd : s.get_designated_worlds())
        designated_worlds.push_back(contracted_worlds_map[wd]);

    auto state_id = canonical ? bounded_identification::calculate_state_id(s, k, storages) : 0;

    return {is_bisim, state{s.get_language(), bounded_worlds_number, std::move(quotient_r), std::move(quotient_v), std::move(designated_worlds), state_id}};
}

/*
std::pair<bool, state>
bounded_contraction_builder::calculate_canonical_contraction(const state &s, unsigned long k, const signature_storage_ptr &s_storage,
                                                             const information_state_storage_ptr &is_storage) {
    // We first calculate the maximal representative signatures: worlds_max_reprs[x] is the maximal representative of x
    const auto &[worlds_signatures, worlds_max_signs, sign_map, worlds_max_reprs] = calculate_max_signatures(s, k, s_storage, is_storage);

    // We now create the set 'max_signs_set' of maximal representative signatures out of the vector 'worlds_max_signs'.
    auto max_signs_set = std::unordered_set<signature_id>{worlds_max_signs.begin(), worlds_max_signs.end()};
    max_signs_set.erase(0);     // If the null signature 0 is in the set, we delete it
    world_id bounded_worlds_number = max_signs_set.size();

    // contracted_worlds_ids_map[sign] assigns to sign a numerical id that will denote a world in the contraction
    std::vector<world_id> contracted_worlds_ids_map = std::vector<world_id>(s.get_worlds_number());
    auto max_reprs_bitset = boost::dynamic_bitset<>(s.get_worlds_number());

    relations    quotient_r = relations(s.get_language()->get_agents_number());
    label_vector quotient_v = label_vector(bounded_worlds_number);

    world_id count = 0;

    for (const signature_id &sign : max_signs_set) {
        const world_id x = sign_map.at(sign).second;        // We keep track in max_reprs_bitset of all maximal representatives x
        max_reprs_bitset[x].flip();                         // of our model (namely, all worlds x s.t. max_reprs_bitset[x] = 1)
        contracted_worlds_ids_map[x] = count;
        quotient_v[count++] = s.get_label_id(x);
    }

    for (del::agent ag = 0; ag < s.get_language()->get_agents_number(); ++ag) {
        quotient_r[ag] = agent_relation(bounded_worlds_number);

        for (world_id w = 0; w < bounded_worlds_number; ++w)
            quotient_r[ag][w] = world_set(bounded_worlds_number);
    }

    for (const signature_id &sign : max_signs_set) {
        const auto &[_, x] = sign_map.at(sign);     // same_sign_worlds

        if (k > s.get_depth(x)) {
            unsigned long b_x = k - s.get_depth(x);

            for (del::agent ag = 0; ag < s.get_language()->get_agents_number(); ++ag) {
                for (const world_id y : s.get_agent_possible_worlds(ag, x))
                    if (s.get_depth(y) <= k) {
                        signature_id sign_y = worlds_signatures[b_x-1][y];

                        auto y_reprs = bit_deque{max_reprs_bitset & sign_map.at(sign_y).first->get_bitset()};   // We compute the maximal representatives of y in its (b(x)-1)-block
                        world_id min_y = y_reprs.get_bitset().find_first();

                        for (const world_id yr : y_reprs)
                            if (worlds_max_signs[yr] < worlds_max_signs[min_y])       // We take the minimal one, namely, its (b(x)-1)-canonical representative
                                min_y = yr;

                        quotient_r[ag][contracted_worlds_ids_map[x]].push_back(contracted_worlds_ids_map[min_y]);
                    }
            }
        }
    }

    world_set designated_worlds(bounded_worlds_number);

    for (world_id wd : s.get_designated_worlds())
        designated_worlds.push_back(contracted_worlds_ids_map[wd]);

    return {false, state{s.get_language(), bounded_worlds_number, std::move(quotient_r), std::move(quotient_v),
                  std::move(designated_worlds)}};
}
*/

std::vector<world_id> bounded_contraction_builder::calculate_max_representatives(const state &s, unsigned long k,
                                                                                 const block_matrix &worlds_blocks) {
    auto worlds_max_reprs = std::vector<world_id>(s.get_worlds_number());
    std::queue<world_id> to_visit;
    boost::dynamic_bitset<> represented(s.get_worlds_number());

    // We first build 'worlds_max_reprs' with a BFS visit.
    // Queue 'to_visit' contains world_ids of the preprocessed state that still have not been visited.
    // We visit worlds from higher to lower depth. Thus, we start with the designated worlds
    for (const world_id wd : s.get_designated_worlds())
        to_visit.push(wd);

    while (not to_visit.empty()) {
        world_id current = to_visit.front();
        to_visit.pop();

        const block_ptr &block = worlds_blocks[current][k - s.get_depth(current)];

        world_id max_representative = get_block_max_representative(s, block);                           // We calculate the maximal representative of 'current', i.e., the world in 'block' with higher bound
        update_block_max_representative(s, block, max_representative, represented, worlds_max_reprs);   // We update the maximal representative of the worlds in block
        update_to_visit_worlds(s, k, to_visit, current, represented);                                   // We add the next worlds to process into the to_visit queue
    }
    return worlds_max_reprs;
}

std::tuple<signature_matrix, signature_vector, signature_map, std::vector<world_id>>
bounded_contraction_builder::calculate_max_signatures(const state &s, unsigned long k, const del::storages_ptr &storages) {
    auto worlds_max_signs = signature_vector(s.get_worlds_number());
    auto worlds_max_reprs = std::vector<world_id>(s.get_worlds_number());
    std::queue<world_id> to_visit;
    boost::dynamic_bitset<> represented(s.get_worlds_number());

    // worlds_signatures[h][x] contains the h-signature of world x
    // sign_map[sign_x_h] = (X, max_x), where X is the set of worlds y s.t. sign_y_h = sign_x_h and max_x is a world
    // in X with greater bound (wrt. the worlds in X)
    auto [worlds_signatures, sign_map] = bounded_identification::calculate_signatures(s, k, storages);

    // We build 'worlds_max_signs' with a BFS visit.
    // Queue 'to_visit' contains world_ids of the state that still have not been visited.
    // We visit worlds from higher to lower depth, starting with the designated worlds
    for (const world_id wd : s.get_designated_worlds())
        to_visit.push(wd);

    while (not to_visit.empty()) {
        world_id current = to_visit.front();
        to_visit.pop();

        const signature_id &sign = (worlds_signatures[k - s.get_depth(current)][current]);      // We take the representative signature of 'current'
        const auto &[worlds, max_representative] = sign_map.at(sign);                           // max_representative is a world in 'worlds' with greater bound

        update_max_representative_sign(s, k, *worlds, max_representative, represented, worlds_signatures, worlds_max_signs, worlds_max_reprs);   // We update the maximal representative signature of the worlds in 'worlds'
        update_to_visit_worlds(s, k, to_visit, current, represented);                                                                           // We add the next worlds to process into the to_visit queue
    }
    return {std::move(worlds_signatures), std::move(worlds_max_signs), std::move(sign_map), std::move(worlds_max_reprs)};
}

world_id bounded_contraction_builder::get_block_max_representative(const state &s, const block_ptr &block) {
    world_id max_representative = *block->begin();

    for (const world_id w : *block)
        if (s.get_depth(w) < s.get_depth(max_representative))   // Equivalent to: b(w) >= b(max_representative)
            max_representative = w;

    return max_representative;
}

void bounded_contraction_builder::update_block_max_representative(const state &s, const block_ptr &block, world_id max_representative,
                                                                  boost::dynamic_bitset<> &represented, std::vector<world_id> &worlds_max_reprs) {
    // If a world in 'block' is not yet represented, we set its maximal representative
    for (const world_id w : *block) {
        if (not represented[w]) {
            worlds_max_reprs[w] = max_representative;
            represented[w] = true;
        }
    }
}

/*world_id bounded_contraction_builder::get_max_representative_sign(const state &s, const world_set &worlds) {
    world_id max_representative = *worlds.begin();

    for (const world_id w : worlds)
        if (s.get_depth(w) < s.get_depth(max_representative))   // Equivalent to: b(w) >= b(max_representative)
            max_representative = w;

    return max_representative;
}*/

void bounded_contraction_builder::update_max_representative_sign(const state &s, const unsigned long k, const world_bitset &worlds,
                                                                 world_id max_representative, boost::dynamic_bitset<> &represented,
                                                                 const signature_matrix &worlds_signatures, signature_vector &worlds_max_signs,
                                                                 std::vector<world_id> &worlds_max_reprs) {
    // If a world in 'worlds' is not yet represented, we set its maximal representative signature
    for (const world_id w : worlds) {
        if (not represented[w]) {
            worlds_max_signs[w] = worlds_signatures[k - s.get_depth(max_representative)][max_representative];
            worlds_max_reprs[w] = max_representative;
            represented[w] = true;
        }
    }
}

void bounded_contraction_builder::update_to_visit_worlds(const state &s, unsigned long k, std::queue<world_id> &to_visit,
                                                         world_id current, boost::dynamic_bitset<> &represented) {
    // We visit unrepresented worlds that are directly accessible from 'current'. In this way,
    // we implement a visit by descending depth.
    for (del::agent ag = 0; ag < s.get_language()->get_agents_number(); ++ag)
        for (const world_id w : s.get_agent_possible_worlds(ag, current))
            if (s.get_depth(w) <= k and not represented[w])
                to_visit.push(w);
}
