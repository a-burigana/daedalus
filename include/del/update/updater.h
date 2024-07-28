//
// BEP - Bounded Epistemic Planner (MIT License)
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

#ifndef BEP_UPDATER_H
#define BEP_UPDATER_H

#include "../states/state.h"
#include "../actions/action.h"
#include "../language/language.h"
#include "boost/dynamic_bitset.hpp"
#include "../bisimulation/bisimulation_types.h"

namespace del {
    class updater {
    public:
        static bool is_applicable(const state &s, const action &a);
        static state product_update(const state &s, const action &a);

        static state product_update(const state &s, const action_deque &as, bool apply_contraction = false,
                                    del::bisimulation_type type = del::bisimulation_type::full, const unsigned long k = 0);

    private:
        using updated_world            = std::pair<const world_id, const event_id>;
        using updated_world_pair       = std::pair<const updated_world, const updated_world>;
        using updated_worlds_map       = std::map<updated_world, world_id>;
        using updated_world_pair_deque = std::deque<updated_world_pair>;        // std::deque<updated_world_pair>;   TODO: CHANGE BACK TO DEQUE!!!!!!!!!!!!!!!!!
        using updated_edges_vector     = std::vector<updated_world_pair_deque>;

        static bool is_applicable_world(const state &s, const action &a, world_id wd);

        static std::pair<world_id, world_set> calculate_worlds(const state &s, const action &a,
                                                               updated_worlds_map &w_map, updated_edges_vector &r_map);

        static relations calculate_relations(const state &s, const action &a, world_id worlds_number,
                                             const updated_worlds_map &w_map, const updated_edges_vector &r_map);

        static label_vector calculate_labels(const state &s, const action &a, world_id worlds_number,
                                             const updated_worlds_map &w_map);

        static label update_world(const state &s, const world_id &w, const action &a, const event_id &e);
    };
}

#endif //BEP_UPDATER_H
