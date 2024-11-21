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

#ifndef DAEDALUS_BISIMULATION_TYPES_H
#define DAEDALUS_BISIMULATION_TYPES_H

#include <list>
#include <set>
#include <vector>
#include <map>
#include <unordered_map>
#include "../../../language/language_types.h"
#include "../../../../utils/bit_deque.h"

namespace kripke {
    enum class contraction_type : uint8_t {
        full,
        rooted,
        canonical
    };

    using agent_worlds_labels = std::vector<del::agent>;

    struct q_block;
    struct x_block;

    using block                 = bit_deque;
    using q_block_ptr           = std::shared_ptr<q_block>;
    using x_block_ptr           = std::shared_ptr<x_block>;
    using q_partition           = std::vector<q_block_ptr>;
    using x_partition           = std::list<x_block_ptr>;   // todo: use vector?
    using compound_x_blocks_set = std::set<x_block_ptr>;
    using q_block_ptr_vector    = std::vector<q_block_ptr>;
    using q_block_ptr_list      = std::list<q_block_ptr>;
    using counts_vector         = std::vector<unsigned long>;
    using split_q_blocks_map    = std::unordered_map<q_block_ptr, q_block_ptr>;

    // Paige and Tarjan data structures
    /* - Each block of Q has an associated integer giving its size (already present in std::list) and
     * - Points to a doubly linked list of the elements in it.
     * - Each block of Q points to the block of X containing it.*/
    struct q_block {
        block elements;
        x_block_ptr super_block;

        q_block(block elements_, x_block_ptr super_block_) :
            elements{std::move(elements_)},
            super_block{std::move(super_block_)} {}

        q_block(const q_block&) = default;
        q_block& operator=(const q_block&) = delete;

        q_block(q_block&&) = default;
        q_block& operator=(q_block&&) = default;

        ~q_block() = default;
    };

    /* - Each block of X points to a doubly linked list of the blocks of Q contained in it.
     * -*/
    struct x_block {
        q_block_ptr_list sub_blocks;  // todo: use list?
        counts_vector counts;

        x_block(q_block_ptr_list sub_blocks_, counts_vector counts_) :
            sub_blocks{std::move(sub_blocks_)},
            counts{std::move(counts_)} {}

        x_block(const x_block&) = default;
        x_block& operator=(const x_block&) = delete;

        x_block(x_block&&) = default;
        x_block& operator=(x_block&&) = default;

        ~x_block() = default;
    };
}

#endif //DAEDALUS_BISIMULATION_TYPES_H
