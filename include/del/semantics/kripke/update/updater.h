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

#ifndef DAEDALUS_UPDATER_H
#define DAEDALUS_UPDATER_H

#include <unordered_set>
#include <unordered_map>
#include "../../../language/language.h"
#include "../../../../utils/storage_types.h"
#include "../states/state.h"
#include "../actions/action.h"
#include "boost/dynamic_bitset.hpp"
#include "../bisimulation/bisimulation_types.h"
#include "../bisimulation/bounded_bisimulation_types.h"

namespace kripke {
    class updater {
    public:
        struct updated_world {
            world_id m_w;
            event_id m_e;

            updated_world(world_id w, event_id e) : m_w{w}, m_e{e} {}

            bool operator==(const updated_world &rhs) const { return m_w == rhs.m_w and m_e == rhs.m_e; }
            bool operator!=(const updated_world &rhs) const { return !(rhs == *this); }
        };

        static bool is_applicable(const state &s, const action &a, const del::label_storage &l_storage);
        static state product_update(const state &s, const action &a, del::label_storage &l_storage);

        static state product_update(const state &s, const action_deque &as, del::storages_handler_ptr handler,
                                    bool apply_contraction = false, contraction_type type = contraction_type::full,
                                    unsigned long k = 0);

    private:
        using updated_world_pair       = std::pair<const updated_world, const updated_world>;
        using updated_worlds_map       = std::unordered_map<updated_world, world_id>;
        using updated_world_pair_deque = std::deque<updated_world_pair>;
        using updated_edges_vector     = std::vector<updated_world_pair_deque>;

        static bool is_applicable_world(const state &s, const action &a, world_id wd, const del::label_storage &l_storage);

        static std::pair<world_id, world_bitset> calculate_worlds(const state &s, const action &a, updated_worlds_map &w_map,
                                                                  updated_edges_vector &r_map, del::label_storage &l_storage);

        static relations calculate_relations(const state &s, const action &a, world_id worlds_number,
                                             const updated_worlds_map &w_map, const updated_edges_vector &r_map);

        static label_vector calculate_labels(const state &s, const action &a, world_id worlds_number,
                                             const updated_worlds_map &w_map, del::label_storage &l_storage);

        static label_id update_world(const state &s, const world_id &w, const action &a, const event_id &e,
                                     del::label_storage &l_storage);
    };
}

template<>
struct std::hash<kripke::updater::updated_world> {
    std::size_t operator()(const kripke::updater::updated_world& uw) const noexcept {
        std::size_t hw = std::hash<kripke::world_id>{}(uw.m_w);
        std::size_t he = std::hash<kripke::event_id>{}(uw.m_e);
        return hw ^ (he << 1);
    }
};

#endif //DAEDALUS_UPDATER_H
