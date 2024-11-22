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

#include "../../../../../include/del/semantics/kripke/bisimulation/bounded_identification.h"
#include "../../../../../include/del/semantics/kripke/states/state.h"
#include "../../../../utils/storage.cpp"

using namespace kripke;

std::pair<signature_matrix, signature_map>
bounded_identification::calculate_signatures(const state &s, unsigned long k, const signature_storage_ptr &s_storage,
                                             const information_state_storage_ptr &is_storage) {
    auto worlds_signatures = signature_matrix(k+1);
    signature_map sign_map;

    for (auto &h_signatures : worlds_signatures)
        h_signatures = signature_vector(s.get_worlds_number());

    for (const world_id wd : s.get_designated_worlds())
        calculate_world_signature(s, k, wd, k, s_storage, is_storage, worlds_signatures, sign_map);

    return {std::move(worlds_signatures), std::move(sign_map)};
}

information_state_id bounded_identification::calculate_state_id(const kripke::state &s, unsigned long k,
                                                                const kripke::signature_storage_ptr &s_storage,
                                                                const kripke::information_state_storage_ptr &is_storage) {
    auto worlds_signatures = signature_matrix(k+1);
    information_state designated_signatures;

    for (auto &h_signatures : worlds_signatures)
        h_signatures = signature_vector(s.get_worlds_number());

    for (const world_id wd : s.get_designated_worlds())
        designated_signatures.emplace(calculate_world_signature(s, k, wd, k, s_storage, is_storage, worlds_signatures));

    return is_storage->emplace(std::move(designated_signatures));
}

void bounded_identification::calculate_world_signature(const state &s, const unsigned long k, const world_id x,
                                                       const unsigned long h, const signature_storage_ptr &s_storage,
                                                       const information_state_storage_ptr &is_storage,
                                                       signature_matrix &worlds_signatures, signature_map &sign_map) {
    agents_information_state xs = agents_information_state(s.get_language()->get_agents_number());
//    signature_id empty_is = is_storage->emplace(information_state{});

    for (del::agent ag = 0; ag < s.get_language()->get_agents_number(); ++ag)
        xs[ag] = 0;

    if (h > 0)
        for (del::agent ag = 0; ag < s.get_language()->get_agents_number(); ++ag) {
            information_state x_ag;

            for (const world_id y: s.get_agent_possible_worlds(ag, x)) {
                if (not worlds_signatures[h-1][y])              // We check that we did not already calculate y's (h-1)-signature
                    calculate_world_signature(s, k, y, h - 1, s_storage, is_storage, worlds_signatures, sign_map);

                x_ag.emplace(worlds_signatures[h-1][y]);
            }
            xs[ag] = is_storage->emplace(std::move(x_ag));      // xs[ag] is the numerical id referring to the set of
        }                                                       // (h-1)-signatures of the worlds y such that x R_ag y

    auto sign_x_h = signature{s.get_language(), s_storage, is_storage, s.get_label(x), std::move(xs), h, x};    // We create the h-signature of x (h being x's bound),
    worlds_signatures[h][x] = s_storage->emplace(std::move(sign_x_h));                                          // we add it to the storage, and we put it into worlds_signatures[h][x]

    if (auto worlds_it = sign_map.find(worlds_signatures[h][x]); worlds_it == sign_map.end()) {
        sign_map[worlds_signatures[h][x]] = {std::make_unique<world_set>(world_set{s.get_worlds_number()}), x};
        sign_map.at(worlds_signatures[h][x]).first->push_back(x);
    } else {
        worlds_it->second.first->push_back(x);                           // The first element of the pair is the set of worlds x that have the same h-signature

        if (s.get_depth(x) < s.get_depth(worlds_it->second.second))     // The second element of the pair is the world with the maximum bound
            worlds_it->second.second = x;
    }
}

unsigned long long bounded_identification::calculate_world_signature(const state &s, const unsigned long k, const world_id x,
                                                                     const unsigned long h, const signature_storage_ptr &s_storage,
                                                                     const information_state_storage_ptr &is_storage,
                                                                     signature_matrix &worlds_signatures) {
    agents_information_state xs = agents_information_state(s.get_language()->get_agents_number());
//    signature_id empty_is = is_storage->emplace(information_state{});

    for (del::agent ag = 0; ag < s.get_language()->get_agents_number(); ++ag)
        xs[ag] = 0;

    if (h > 0)
        for (del::agent ag = 0; ag < s.get_language()->get_agents_number(); ++ag) {
            information_state x_ag;

            for (const world_id y : s.get_agent_possible_worlds(ag, x)) {
                if (s_storage->is_null(worlds_signatures[h-1][y]))               // We check that we did not already calculate y's (h-1)-signature
                    worlds_signatures[h-1][y] = calculate_world_signature(s, k, y, h-1, s_storage, is_storage, worlds_signatures);

                x_ag.emplace(worlds_signatures[h-1][y]);
            }
            xs[ag] = is_storage->emplace(std::move(x_ag));      // xs[ag] is the numerical id referring to the set of
        }                                                       // (h-1)-signatures of the worlds y such that x R_ag y

    auto sign_x_h = signature{s.get_language(), s_storage, is_storage, s.get_label(x), std::move(xs), h, x};    // We create the h-signature of x (h being x's bound),
    auto id = s_storage->emplace(std::move(sign_x_h));                                                          // we add it to the storage, and we return it
    assert(id != 0);
    return id;
}
