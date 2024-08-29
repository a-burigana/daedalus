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

#include "../../../../../include/del/semantics/kripke/bisimulation/bounded_partition_refinement.h"
#include "../../../../../include/del/semantics/kripke/states/state.h"
#include "../../../../../include/search/search_space.h"
#include <memory>
#include <utility>

using namespace kripke;

std::pair<bool, block_matrix> bounded_partition_refinement::do_refinement_steps(const state &s, unsigned long k) {
    bpr_structures structures = init_structures(s, k+1);

    partition &Q = structures.Q;
    block_matrix &worlds_blocks = structures.worlds_blocks;
    relations &r_1 = structures.r_1;      // The preimage of r
    block_id &count = structures.count;
    unsigned long h = 0;

    do {
        copy_partition(s, k, h, Q, worlds_blocks, count);

        for (const block_ptr &B: Q[h])                      // We refine Q[h] wrt. all of its blocks
            refine(s, k, h, Q, B, r_1, worlds_blocks, count);

        ++h;
    } while (h < k and Q[h].size() != Q[h-1].size());     // If no block was split, then we are done

    while (h < k)
        copy_partition(s, k, h++, Q, worlds_blocks, count);

    bool is_bisim = Q[k].size() == Q[k-1].size() and s.get_max_depth() < k-1;
    return {is_bisim, std::move(worlds_blocks)};
}

void bounded_partition_refinement::copy_partition(const state &s, const unsigned long k, const unsigned long h,
                                                  partition &Q, block_matrix &worlds_blocks, block_id &count) {
    // We copy Q[h] into Q[h+1] in such a way that, after the copy, Q[h+1] will contain the original pointers to the
    // blocks of Q[h]. In this way, we guarantee that each block_ptr B = worlds_blocks[x][h+1] (and not a copy with
    // different pointer address) is in Q[h+1]. For this reason, we first move Q[h] into Q[h+1], so to preserve the
    // correct pointer addresses into Q[h+1].
    Q[h+1] = std::move(Q[h]);
    // Then, we copy the elements of Q[h+1] back into Q[h]. As a result, Q[h] will contain block_ptrs with different
    // addresses, but with the same pointed blocks
    for (const block_ptr &B : Q[h+1]) {
        block_ptr B_copy = std::make_shared<block>(*B);
        B->set_id(count++);

        for (const world_id x : *B) {
            if (h < worlds_blocks[x].size())    // We keep track of the h-block of x. A copy is necessary to avoid
                worlds_blocks[x][h] = B_copy;   // unwanted splitting of already calculated blocks

            if (k > h + s.get_depth(x))         // If x still needs to be split, we update its (h+1)-block (i.e., its
                worlds_blocks[x][h+1] = B;      // (h+1)-bisimulation class), preparing it for further splitting
        }
        Q[h].push_back(std::move(B_copy));
    }
}   // Complexity: O(|W|)

void bounded_partition_refinement::refine(const state &s, const unsigned long k, const unsigned long h, partition &Q,
                                          const block_ptr &B, const relations &r_1, block_matrix &worlds_blocks,
                                          block_id &count) {
    // [From STEP 3] Copy the elements of B into a temporary set B'. (This facilitates splitting B with respect to
    // itself during the refinement.)
    block B_ = *B;

    for (del::agent ag = 0; ag < s.get_language()->get_agents_number(); ++ag) {
        block B_preimage = calculate_preimages(s, B_, r_1, ag);

        // [STEP 4] Refine Q with respect to B
        if (not B_preimage.empty())
            split(s, k, h, Q, B_preimage, worlds_blocks, count);
    }
}

block bounded_partition_refinement::calculate_preimages(const state &s, const block &B_, const relations &r_1,
                                                        const del::agent ag) {
//    boost::dynamic_bitset<> B_preimage(s.get_worlds_number());
//
//    for (const world_id y: B_)
//        B_preimage |= s.get_agent_possible_worlds(ag, y);   // todo: this way we don't use r_1!!!!!
//
//    return block{B_preimage};
    block B_preimage = block(s.get_worlds_number());

    for (const world_id y: B_)
        for (const world_id x: r_1[ag][y])
            // [From STEP 3] Compute R^-1(B).
            B_preimage.push_back(x);

    return B_preimage;
}

void bounded_partition_refinement::split(const state &s, const unsigned long k, const unsigned long h, partition &Q,
                                         const block &block_preimage, block_matrix &worlds_blocks, block_id &count) {
    split_blocks_map split_blocks;
    // [From STEP 4] For each block D of Q containing some element of R^-1(B), split D into D_1 = D \cap R^-1(B)
    // and D_2 = D \ D_1. Do this by scanning the elements of R^-1(B) [i.e., block_preimage].
    // [From STEP 6] Proceed exactly as in Step 4 but scan R^-1(B) \ R^-1(S \ B) (computed in step 5) instead of R^-1(B).
    for (const world_id x: block_preimage) {
        if (k > h + s.get_depth(x)) {
            // To process an element x \in R^-1(B), determine the block D of Q containing it...
            block_ptr D = worlds_blocks[x][h+1];

            // ...and create an associated block D' if one does not already exist.
            if (split_blocks.find(D) == split_blocks.end())
                split_blocks[D] = Q[h+1].emplace_back(std::make_shared<block>(block{s.get_worlds_number(), count++}));

            // We move x from D to D'
            block_ptr D_ = split_blocks[D];
            D_->push_back(x);
            D->remove(x);
            worlds_blocks[x][h+1] = D_;
        }
    }
    // After the scanning, process the list of split blocks. For each such block D with associated block D'...
    for (auto &[D, D_]: split_blocks)
        if (D->empty())
            Q[h+1].remove(D);
}

bpr_structures bounded_partition_refinement::init_structures(const state &s, unsigned long long k) {
    partition Q;
    block_matrix worlds_blocks;
    relations r_1 = init_preimage(s);
    block_id count = 0;
    std::map<const label, block_ptr> initial_partition;

    Q = partition{k + 1};
    worlds_blocks = block_matrix(s.get_worlds_number());

    for (world_id x = 0; x < s.get_worlds_number(); ++x)
        worlds_blocks[x] = block_vector(k+1);
//        if (k >= s.get_depth(x))                                       // If bound(x) >= 0, we create a block_vector v of
//            worlds_blocks[x] = block_vector(k - s.get_depth(x) + 1);   // size bound(x)+1, and we let worlds_blocks[x] = v
//        else
//            worlds_blocks[x] = block_vector(0);

    // Initializing initial partition. Here we split worlds depending on their propositional valuation.
    // In each q_block we insert worlds sharing the same valuation.
    for (world_id x = 0; x < s.get_worlds_number(); ++x)
        if (k >= s.get_depth(x))        // Namely, b(x) >= 0
            init_partitions_helper(s, initial_partition, x, worlds_blocks, count);
    // Complexity: O(|P|*|W|)

    // Initializing Q
    for (auto &[val, b]: initial_partition)
        Q[0].push_back(std::move(b));
    // Complexity: O(|W|)

    return bpr_structures{std::move(Q), std::move(worlds_blocks), std::move(r_1), count};
}   // Complexity: O(|P|*|W|)

void bounded_partition_refinement::init_partitions_helper(const state &s, std::map<const label, block_ptr> &partition,
                                                          world_id w, block_matrix &worlds_blocks, block_id &count) {
    const auto it = partition.find(s.get_label(w));

    if (it == partition.end()) {
        block_ptr b = std::make_shared<block>(block{s.get_worlds_number(), world_list{w}, count++});
        worlds_blocks[w][0] = b;
        partition.emplace(s.get_label(w), std::move(b));
    } else {
        block_ptr &b = it->second;
        b->push_back(w);
        worlds_blocks[w][0] = b;
    }
}   // Complexity: O(|P|)

relations bounded_partition_refinement::init_preimage(const state &s) {
    relations r_1 = relations(s.get_language()->get_agents_number());

    for (del::agent ag = 0; ag < s.get_language()->get_agents_number(); ++ag) {
        agent_relation r_ag = agent_relation(s.get_worlds_number());

        for (world_id w = 0; w < s.get_worlds_number(); ++w)    // Initializing world sets
            r_ag[w] = world_set(s.get_worlds_number());

        for (world_id w = 0; w < s.get_worlds_number(); ++w)
            for (const world_id v: s.get_agent_possible_worlds(ag, w))
                r_ag[v].push_back(w);

        r_1[ag] = std::move(r_ag);
    }
    return r_1;
}   // Complexity: O(|R|)
