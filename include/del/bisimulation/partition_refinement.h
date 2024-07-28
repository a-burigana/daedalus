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

#ifndef BEP_PARTITION_REFINEMENT_H
#define BEP_PARTITION_REFINEMENT_H

#include "../language/language.h"
#include "bisimulation_types.h"
#include "../states/states_types.h"
#include "../../search/search_types.h"

namespace del {
    class state;

    class partition_refinement {
    public:
        static std::pair<bool, state> contract(state &s);
        static bool contract(search::node_ptr &n);

    private:
        static std::pair<bool, state> contraction_helper(const state &s);

        static state build_full_contraction(const state &s, const agent_relation &r, const agent_worlds_labels &labels,
                                            q_partition &Q, const q_partition &Q_sinks);

        static q_block_ptr get_smaller_block(x_block_ptr &S, compound_x_blocks_set &C);
        static counts_vector calculate_block_counts(const agent_relation &r, const q_block_ptr &B);

        static void refine(q_partition &Q, x_partition &X, compound_x_blocks_set &C, x_block_ptr &S, const q_block &B_,
                           counts_vector &B_counts, const agent_relation &r_preimage, q_block_ptr_vector &worlds_blocks,
                           const world_id preprocessed_worlds_no);

        static void refine_helper(q_partition &Q, x_partition &X, compound_x_blocks_set &C, const block &block_preimage,
                                  q_block_ptr_vector &worlds_blocks, const world_id preprocessed_worlds_no);

        static void clean_and_update_partitions(q_partition &Q, x_partition &X, compound_x_blocks_set &C,
                                                split_q_blocks_map &split_blocks);

        static void update_counts(x_block_ptr &S, const q_block &B_, const counts_vector &B_counts,
                                  const agent_relation &r_preimage);

        static std::pair<agent_relation, agent_worlds_labels> build_preprocessed_state(const state &s);

        static void init_data_structures(const state &s, const agent_relation &r, const agent_worlds_labels &labels,
                                         q_partition &Q, q_partition &Q_sinks, x_partition &X, compound_x_blocks_set &C,
                                         q_block_ptr_vector &worlds_blocks, agent_relation &r_preimage,
                                         const world_id preprocessed_worlds_no);

        static void init_partitions(const state &s, const agent_relation &r, const agent_worlds_labels &labels,
                                    q_partition &Q, q_partition &Q_sinks, x_partition &X, compound_x_blocks_set &C,
                                    q_block_ptr_vector &worlds_blocks, const world_id preprocessed_worlds_no);

        static bool is_sink(const state &s, world_id w);

        template<class T>
        static void init_partitions_helper(std::map<T, q_block> &partition, world_id w, T key, const world_id preprocessed_worlds_no);

        static void init_preimage(const agent_relation &r, agent_relation &r_preimage, const world_id preprocessed_worlds_no);
        static void init_counts(const agent_relation &r, x_block_ptr &S);

        static bool is_compound(const x_block_ptr &x);
    };
}

#endif //BEP_PARTITION_REFINEMENT_H
