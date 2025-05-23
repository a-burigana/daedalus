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

#ifndef DAEDALUS_BOUNDED_BISIMULATION_TYPES_H
#define DAEDALUS_BOUNDED_BISIMULATION_TYPES_H

#include <deque>
#include <list>
#include <memory>
#include <vector>
#include <set>
#include <unordered_map>
#include "../../../../utils/bit_deque.h"
#include "../../../../utils/storage.h"
#include "../states/states_types.h"
#include "../../delphic/states/possibility.h"

namespace kripke {
    using block_id         = unsigned long;
    using block            = bit_deque;
    using block_ptr        = std::shared_ptr<block>;
    using block_vector     = std::vector<block_ptr>;
    using block_matrix     = std::vector<block_vector>;
    using block_deque      = std::deque<block_ptr>;
    using partition        = std::vector<block_deque>;
    using split_blocks_map = std::unordered_map<block_ptr, block_ptr>;

    struct bpr_structures {
        partition Q;
        block_matrix worlds_blocks;
        relations r_1;
        block_id count;
    };

    using bpr_structures_ptr = std::shared_ptr<bpr_structures>;

    // CANONICAL BOUNDED CONTRACTIONS
    using signature = delphic::possibility;
    using signature_ptr = std::shared_ptr<signature>;
    using signature_id = delphic::possibility_id;
    using signature_vector = std::vector<signature_id>;
    using signature_matrix = std::vector<signature_vector>;
    using signature_map = std::unordered_map<signature_id, std::pair<std::unique_ptr<world_bitset>, world_id>>;

    using information_state = delphic::information_state;
    using information_state_id = delphic::information_state_id;
    using agents_information_state = delphic::agents_information_state;
}

#endif //DAEDALUS_BOUNDED_BISIMULATION_TYPES_H
