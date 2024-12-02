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

#ifndef DAEDALUS_BOUNDED_CONTRACTION_BUILDER_H
#define DAEDALUS_BOUNDED_CONTRACTION_BUILDER_H

#include <set>
#include <queue>
#include "bounded_bisimulation_types.h"
#include "../../../../utils/storage.h"

namespace kripke {
    class state;

    class bounded_contraction_builder {
    public:
        static std::pair<bool, state> calculate_rooted_contraction(const state &s, unsigned long k, bool canonical = false,
                                                                   const del::storages_ptr &storages = nullptr);

        /*static std::pair<bool, state> calculate_canonical_contraction(const state &s, unsigned long k,
                                                                      const del::signature_storage_ptr &s_storage,
                                                                      const del::information_state_storage_ptr &is_storage);*/

    private:

        static std::vector<world_id> calculate_max_representatives(const state &s, unsigned long k,
                                                                   const block_matrix &worlds_blocks);

        static std::tuple<signature_matrix, signature_vector, signature_map, std::vector<world_id>>
            calculate_max_signatures(const state &s, unsigned long k, const del::storages_ptr &storages);

        static world_id get_block_max_representative(const state &s, const block_ptr &block);

        static void update_block_max_representative(const state &s, const block_ptr &block, world_id max_representative,
                                                    boost::dynamic_bitset<> &represented, std::vector<world_id> &worlds_max_reprs);

        static world_id get_max_representative_sign(const state &s, const world_bitset &worlds);

        static void update_max_representative_sign(const state &s, unsigned long k, const world_bitset &worlds,
                                                   world_id max_representative, boost::dynamic_bitset<> &represented,
                                                   const signature_matrix &worlds_signatures, signature_vector &worlds_max_signs,
                                                   std::vector<world_id> &worlds_max_reprs);

        static void update_to_visit_worlds(const state &s, unsigned long k, std::queue<world_id> &to_visit,
                                           world_id current, boost::dynamic_bitset<> &represented);
    };
}

#endif //DAEDALUS_BOUNDED_CONTRACTION_BUILDER_H
