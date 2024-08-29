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

#ifndef DAEDALUS_BOUNDED_PARTITION_REFINEMENT_H
#define DAEDALUS_BOUNDED_PARTITION_REFINEMENT_H

#include <vector>
#include <list>
#include <map>
#include <memory>
#include <deque>
#include <set>
#include <boost/dynamic_bitset.hpp>
#include "bounded_bisimulation_types.h"
#include "../../../del_types.h"
#include "../states/states_types.h"
#include "../../../../search/search_types.h"

namespace kripke {
    class state;

    class bounded_partition_refinement {
    public:
        static std::pair<bool, block_matrix> do_refinement_steps(const state &s, unsigned long k);

    private:
        static bool contraction_helper(search::node_ptr &n);

        static void copy_partition(const state &s, unsigned long k, unsigned long h, partition &Q,
                                   block_matrix &worlds_blocks, block_id &count);

        static void refine(const state &s, unsigned long k, unsigned long h, partition &Q, const block_ptr &B,
                           const relations &r_1, block_matrix &worlds_blocks, block_id &count);

        static block calculate_preimages(const state &s, const block &B_, const relations &r_1, del::agent ag);

        static void split(const state &s, unsigned long k, unsigned long h, partition &Q,
                          const block &block_preimage, block_matrix &worlds_blocks, block_id &count);

        /*static std::pair<block_id, block_set> compact_block_ids(block_matrix &worlds_blocks);

        static void build_signatures(const state &s, unsigned long long k, block_matrix &worlds_blocks);*/

        // Initialization of structures
        static bpr_structures init_structures(const state &s, unsigned long long k);

        static void init_partitions_helper(const state &s, std::map<const label, block_ptr> &partition,
                                           world_id w, block_matrix &worlds_blocks, block_id &count);

        static relations init_preimage(const state &s);

//        static void update_structures(search::node_ptr &n);
    };
}

#endif //DAEDALUS_BOUNDED_PARTITION_REFINEMENT_H
