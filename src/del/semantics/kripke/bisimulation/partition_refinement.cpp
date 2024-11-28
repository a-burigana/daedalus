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

#include "../../../../../include/del/semantics/kripke/bisimulation/partition_refinement.h"
#include "../../../../../include/del/semantics/kripke/bisimulation/bisimulator.h"
#include "../../../../../include/search/search_space.h"
#include <algorithm>
#include <memory>

using namespace kripke;

std::pair<bool, state> partition_refinement::contract(state &s) {
    return contraction_helper(s);
}

bool partition_refinement::contract(search::node_ptr &n) {
    auto [is_bisim, s_contr] = contraction_helper(*n->get_state());
    n->set_state(std::make_shared<state>(std::move(s_contr)));

    return is_bisim;
}

std::pair<bool, state> partition_refinement::contraction_helper(const state &s) {
    auto result = build_preprocessed_state(s);          // todo: try to generate the preprocessed relations on the fly
    const agent_relation &r = result.first;
    const agent_worlds_labels labels = result.second;
    const world_id preprocessed_worlds_no = r.size();

    q_partition Q, Q_sinks;
    x_partition X;
    compound_x_blocks_set C;
    agent_relation r_preimage;                                          // Preimage of R
    q_block_ptr_vector worlds_blocks = q_block_ptr_vector(preprocessed_worlds_no);    // Each element points to the block of Q containing it.

    init_data_structures(s, r, labels, Q, Q_sinks, X, C, worlds_blocks, r_preimage, preprocessed_worlds_no);

    while (not C.empty()) {
        // [STEP 1] Remove some block S from C. (Block S is a compound block of X.)
        x_block_ptr S = *C.begin();
        C.erase(C.begin());
        // [STEP 2] Examine the first two blocks in the list of blocks of Q contained in S.
        // Let B be the smaller. (Break a tie arbitrarily.)
        q_block_ptr B = get_smaller_block(S, C);

        // [...] create a new (simple) block S' of X containing B as its only block of Q.
        q_block_ptr_list sub_blocks;
        sub_blocks.emplace_back(B);
        x_block_ptr S_ = X.emplace_back(
                std::make_shared<x_block>(std::move(sub_blocks), std::move(calculate_block_counts(r, B))));
        B->super_block = S_;        // S' is the new super block of B
        counts_vector &B_counts = S_->counts;

        // [STEP 3] Copy the elements of B into a temporary set B'. (This facilitates splitting B with respect to
        // itself during the refinement.)
        q_block B_ = *B;

        // [STEPS 4 to 7]
        refine(Q, X, C, S, B_, B_counts, r_preimage, worlds_blocks, preprocessed_worlds_no);
    }

    return {true, build_full_contraction(s, r, labels, Q, Q_sinks)};
}

state partition_refinement::build_full_contraction(const state &s, const agent_relation &r, const agent_worlds_labels &labels,
                                                   q_partition &Q, const q_partition &Q_sinks) {
    // Concatenating Q and Q_sinks
    Q.insert(Q.end(), std::make_move_iterator(Q_sinks.begin()), std::make_move_iterator(Q_sinks.end()));

    world_id worlds_number = 0;
    relations quotient_r = relations(s.get_language()->get_agents_number());
    std::vector<world_id> contracted_worlds_map = std::vector<world_id>(s.get_worlds_number());

    for (const q_block_ptr &block: Q) {
        const world_id w = *block->elements.begin();

        if (w < s.get_worlds_number()) {                // If the current block contains worlds of the original state
            const world_id w_ = worlds_number++;        // (i.e., worlds that were not created by the preprocessing phase)

            for (const world_id v: block->elements)
                contracted_worlds_map[v] = w_;
        }
    }

    for (del::agent ag = 0; ag < s.get_language()->get_agents_number(); ++ag) {
        quotient_r[ag] = agent_relation(s.get_worlds_number());     // todo: shouldn't it be worlds_number?

        for (world_id w_ = 0; w_ < worlds_number; ++w_)     // We make sure that the relations of worlds with
            quotient_r[ag][w_] = block{worlds_number};      // no outgoing edges are correctly computed
    }

    for (world_id w = 0; w < s.get_worlds_number(); ++w) {
        for (const world_id w_ag: r[w]) {                       // By construction, w only sees "agent worlds"
            del::agent ag = labels[w_ag - s.get_worlds_number()];    // ag is the label of w_ag

            for (const world_id v: s.get_agent_possible_worlds(ag, w))
                quotient_r[ag][contracted_worlds_map[w]].push_back(contracted_worlds_map[v]);
        }
    }

    label_vector quotient_l = label_vector(worlds_number);
    for (world_id w = 0; w < s.get_worlds_number(); ++w)
        quotient_l[contracted_worlds_map[w]] = s.get_label(w);

    world_set designated_worlds(worlds_number);

    for (world_id wd : s.get_designated_worlds())
        designated_worlds.push_back(contracted_worlds_map[wd]);

    return state{s.get_language(), worlds_number, std::move(quotient_r), std::move(quotient_l),
                 std::move(designated_worlds)};
}

q_block_ptr partition_refinement::get_smaller_block(x_block_ptr &S, compound_x_blocks_set &C) {
// [STEP 1] Examine the first two blocks in the list of blocks of Q contained in S.
    // Let B be the smaller. (Break a tie arbitrarily.)
    auto first = S->sub_blocks.begin(), second = std::next(first);

    if ((*first)->elements.size() > (*second)->elements.size())
        std::swap(*first, *second);

    // [STEP 2] Remove B from S...
    q_block_ptr B = std::move(*first);
    S->sub_blocks.erase(first);

    // If S is still compound, put S back into C.
    if (is_compound(S))     // todo: move this as in k_paige_tarjan
        C.emplace(S);

    return B;
}

counts_vector partition_refinement::calculate_block_counts(const agent_relation &r, const q_block_ptr &B) {
    counts_vector B_counts = counts_vector(r.size());

    for (world_id x = 0; x < r.size(); ++x) {
        if (B->elements[x])
            B_counts[x] = 0;
        else {
            const block &xs = r[x];
            unsigned long count = 0;

            // During the same scan, compute count(ag, x, B) = |{y \in B | x R(ag)^-1 y}|, ...
            for (const world_id y : B->elements)   // todo: optimize by scanning B->elements and xs simultaneously exploiting the fact that they both are ordered sets
                if (xs[y])
                    ++count;
            // ...store this count in a new count record and make x point to it.
            B_counts[x] = count;
        }
    }
    return B_counts;
}

void partition_refinement::refine(q_partition &Q, x_partition &X, compound_x_blocks_set &C, x_block_ptr &S,
                                  const q_block &B_, counts_vector &B_counts, const agent_relation &r_preimage,
                                  q_block_ptr_vector &worlds_blocks, const world_id preprocessed_worlds_no) {
    block B_preimage = block{preprocessed_worlds_no}, B_S_preimage = block{preprocessed_worlds_no};

    for (const world_id y : B_.elements) {
        for (const world_id x : r_preimage[y]) {
            // [From STEP 3] Compute R(ag)^-1(B).
            B_preimage.push_back(x);
            // [From STEP 5] To process an edge x R(ag) y, determine count(ag, x, B) and count(ag, x, S).
            // If count(ag, x, B) == count(ag, x, S), add x to R(ag)^-1(B) \ R(ag)^-1(S \ B)
            if (B_counts[x] == S->counts[x])
                B_S_preimage.push_back(x);
        }
    }

    // [STEP 4] Refine Q with respect to B
    if (not B_preimage.empty())
        refine_helper(Q, X, C, B_preimage, worlds_blocks, preprocessed_worlds_no);
    // [STEP 6] Refine Q with respect to S \ B
    if (not B_S_preimage.empty())
        refine_helper(Q, X, C, B_S_preimage, worlds_blocks, preprocessed_worlds_no);

    // [STEP 7] Update counts
    update_counts(S, B_, B_counts, r_preimage);
}

void partition_refinement::refine_helper(q_partition &Q, x_partition &X, compound_x_blocks_set &C, const block &block_preimage,
                                         q_block_ptr_vector &worlds_blocks, const world_id preprocessed_worlds_no) {
    split_q_blocks_map split_blocks;
    // [From STEP 4] For each block D of Q containing some element of R(ag)^-1(B), split D into D_1 = D \cap R(ag)^-1(B)
    // and D_2 = D \ D_1. Do this by scanning the elements of R(ag)^-1(B) [i.e., block_preimage].
    // [From STEP 6] Proceed exactly as in Step 4 but scan R(ag)^-1(B) \ R(ag)^-1(S \ B) (computed in step 5) instead of R(ag)^-1(B).
    for (const world_id x : block_preimage) {
        // To process an element x \in R(ag)^-1(B), determine the block D of Q containing it...
        q_block_ptr D = worlds_blocks[x];

        if (split_blocks.find(D) == split_blocks.end()) {
            // ...and create an associated block D' if one does not already exist.
            q_block_ptr D_ =  Q.emplace_back(std::make_shared<q_block>(block{preprocessed_worlds_no}, D->super_block));
            D_->super_block->sub_blocks.push_back(D_);
            split_blocks[D] = D_;       // We associate block D' to D
        }

        q_block_ptr D_ = split_blocks[D];
        // Move x from D to D'.
        D_->elements.push_back(x);
        D->elements.remove(x);
        worlds_blocks[x] = D_;
    }
    clean_and_update_partitions(Q, X, C, split_blocks);
}

void partition_refinement::clean_and_update_partitions(q_partition &Q, x_partition &X, compound_x_blocks_set &C,
                                                       split_q_blocks_map &split_blocks) {
    // After the scanning, process the list of split blocks. For each such block D with associated block D'...
    for (const auto &[D, D_]: split_blocks) {
        if (D->elements.empty()) {                      // ...eliminate the record for D if D is now empty;...
            Q.erase(std::find(Q.begin(), Q.end(), D));     // todo: this has complexity O(|Q|). See if it can be lowered to O(1)
            q_block_ptr_list &sub_blocks = D->super_block->sub_blocks;
            sub_blocks.erase(std::find(sub_blocks.begin(), sub_blocks.end(), D));

            if (sub_blocks.empty()) {       // todo: do I need this?
                X.erase(std::find(X.begin(), X.end(), D->super_block));
                C.erase(std::find(C.begin(), C.end(), D->super_block));
            }
        } else if (is_compound(D->super_block))         // ...and, if D is nonempty and the block of X containing D and
            C.emplace(D->super_block);                  // D' has been made compound by the split, add this block to C
    }
}

void partition_refinement::update_counts(x_block_ptr &S, const q_block &B_, const counts_vector &B_counts,
                                         const agent_relation &r_preimage) {
    // Scan the edges x R(ag) y such that y \in B.
    for (const world_id y : B_.elements)
        for (const world_id x : r_preimage[y])
            // To process an edge x R(ag) y, decrement count(ag, x, S).
            // If this count becomes zero, delete the count record and make x R(ag) y point to count(ag, x, B)
            if (--S->counts[x] == 0)
                S->counts[x] = B_counts[x];
}

// Paige-Tarjan data structures initialization
std::pair<agent_relation, agent_worlds_labels> partition_refinement::build_preprocessed_state(const state &s) {
    world_id worlds_number = s.get_worlds_number();

    for (del::agent ag = 0; ag < s.get_language()->get_agents_number(); ++ag)
        for (world_id w = 0; w < s.get_worlds_number(); ++w)
            worlds_number += s.get_agent_possible_worlds(ag, w).size();

    agent_relation r = agent_relation(worlds_number);

    for (world_id w = 0; w < worlds_number; ++w)
        r[w] = world_set{worlds_number};

    world_id count = s.get_worlds_number() - 1;
    agent_worlds_labels agent_labels = std::vector<del::agent>(worlds_number - s.get_worlds_number());

    for (del::agent ag = 0; ag < s.get_language()->get_agents_number(); ++ag) {
        for (world_id w = 0; w < s.get_worlds_number(); ++w) {
            for (world_id v : s.get_agent_possible_worlds(ag, w)) {
                world_id w_ag = ++count;
                agent_labels[w_ag - s.get_worlds_number()] = ag;

                r[w].push_back(w_ag);
                r[w_ag].push_back(v);
            }
        }
    }

    return std::make_pair(std::move(r), std::move(agent_labels));
}

void partition_refinement::init_data_structures(const state &s, const agent_relation &r, const agent_worlds_labels &labels,
                                                q_partition &Q, q_partition &Q_sinks, x_partition &X, compound_x_blocks_set &C,
                                                q_block_ptr_vector &worlds_blocks, agent_relation &r_preimage,
                                                const world_id preprocessed_worlds_no) {
    // Complexity: O(|W| + |R|)
    init_partitions(s, r, labels, Q, Q_sinks, X, C, worlds_blocks, preprocessed_worlds_no);
    init_preimage(r, r_preimage, preprocessed_worlds_no);
}

void partition_refinement::init_partitions(const state &s, const agent_relation &r, const agent_worlds_labels &labels,
                                           q_partition &Q, q_partition &Q_sinks, x_partition &X, compound_x_blocks_set &C,
                                           q_block_ptr_vector &worlds_blocks, const world_id preprocessed_worlds_no) {
    std::map<const del::label, q_block> initial_partition, initial_partition_sinks;
    std::map<const del::agent, q_block> initial_agent_worlds_partition;      // By construction, "agent worlds" can not be sinks

    worlds_blocks = q_block_ptr_vector(r.size());

    // Initializing initial partition. Here we split worlds depending on their propositional valuation.
    // In each q_block we insert worlds sharing the same valuation.
    // We store blocks containing sink worlds in a separate partition (see (*))
    // We do not update the 'super_block' field, as we will do it later in the X partition initialization
    // Complexity (assuming |P| is constant): O(|W|)
    for (world_id w = 0; w < r.size(); ++w)
        if (w < s.get_worlds_number()) {        // If w is a world of the original state
            if (is_sink(s, w))
                init_partitions_helper<const del::label>(initial_partition_sinks, w, s.get_label(w), preprocessed_worlds_no);
            else
                init_partitions_helper<const del::label>(initial_partition, w, s.get_label(w), preprocessed_worlds_no);
        } else
            init_partitions_helper<const del::agent>(initial_agent_worlds_partition, w, labels[w - s.get_worlds_number()], preprocessed_worlds_no);

    // (*) In an efficient implementation of the algorithm, it is useful to reduce the problem instance to one in which
    // |R(x)| >= 1 for all x \in W. To do this we preprocess the partition P by splitting each block B \in P [where
    // P = {V(p) | p \in Atoms}] into B' = B \cap R^-1(W) and B'' = B \ R^-1(W). The blocks B'' will never be split by
    // the refinement algorithm; thus we can run the refinement algorithm on the partition P' consisting of the set of blocks
    // B'. [...] The coarsest stable refinement of P' together with the blocks B'' is the coarsest stable refinement of P.

    // Initializing partitions Q and Q_sinks. Complexity (assuming |P| is constant): O(1)
    for (auto &[val, block]: initial_partition)
        Q.emplace_back(std::make_shared<q_block>(std::move(block)));

    for (auto &[ags, block] : initial_agent_worlds_partition)
        Q.emplace_back(std::make_shared<q_block>(std::move(block)));

    for (auto &[val, block] : initial_partition_sinks)
        Q_sinks.emplace_back(std::make_shared<q_block>(std::move(block)));

    // Making worlds point to their block. Complexity (assuming |AG| is constant): O(|W|)
    for (const q_block_ptr &block : Q)
        for (const world_id w : block->elements)
            worlds_blocks[w] = block;

    // Initializing partition X. Complexity (assuming |P| is constant): O(1)
    q_block_ptr_list sub_blocks;
    counts_vector counts = counts_vector(s.get_language()->get_agents_number());

    for (const q_block_ptr &block : Q)
        sub_blocks.push_back(block);

    for (const q_block_ptr &block : Q_sinks)
        sub_blocks.push_back(block);

    // Initializing X and C. Complexity: O(1)
    if (not sub_blocks.empty()) {
        x_block_ptr x = X.emplace_back(std::make_shared<x_block>(std::move(sub_blocks), std::move(counts)));
        // Setting pointers to relative x_blocks for each q_block. Complexity (assuming |P| is constant): O(1)
        for (const q_block_ptr &block: Q)
            block->super_block = x;

        if (is_compound(x)) {
            C.emplace(x);
            init_counts(r, x);
        }
    }
}

bool partition_refinement::is_sink(const state &s, const world_id w) {
    for (del::agent ag = 0; ag < s.get_language()->get_agents_number(); ++ag)
        if (not s.get_agent_possible_worlds(ag, w).empty())
            return false;
    return true;
}

template<class T>
void partition_refinement::init_partitions_helper(std::map<T, q_block> &partition, const world_id w, T key,
                                                  const world_id preprocessed_worlds_no) {
    const auto it = partition.find(key);

    if (it == partition.end())
        partition.emplace(key, q_block{block{preprocessed_worlds_no, world_deque{w}}, std::shared_ptr<x_block>(nullptr)});
    else {
        q_block &block = it->second;
        block.elements.push_back(w);
    }
}

void partition_refinement::init_preimage(const agent_relation &r, agent_relation &r_preimage, const world_id preprocessed_worlds_no) {
    // Complexity: O(|R|)
    r_preimage = agent_relation(r.size());

    for (world_id w = 0; w < r.size(); ++w)                 // We make sure that the preimage of worlds with
        r_preimage[w] = world_set{preprocessed_worlds_no};  // no incoming edges is correctly computed

    for (world_id w = 0; w < r.size(); ++w)
        for (const auto &v : r[w])
            r_preimage[v].push_back(w);
}

void partition_refinement::init_counts(const agent_relation &r, x_block_ptr &x) {
    // Complexity: O(|W|)
    x->counts = counts_vector(r.size());

    for (world_id w = 0; w < r.size(); ++w)
        x->counts[w] = r[w].size();
}

bool partition_refinement::is_compound(const x_block_ptr &x) {
    return x->sub_blocks.size() > 1;
}
