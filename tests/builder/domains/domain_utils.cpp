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

#include "domain_utils.h"
#include <deque>
#include <vector>

using namespace bep::tester;
using namespace del;

bitset_deque domain_utils::all_combinations(unsigned long set_size) {
    return combinations_helper(set_size);
}

bitset_deque domain_utils::all_combinations_with(unsigned long set_size, const boost::dynamic_bitset<> &filter) {
    return combinations_helper(set_size, filter, true, true);
}

bitset_deque domain_utils::all_combinations_without(unsigned long set_size, const boost::dynamic_bitset<> &filter) {
    return combinations_helper(set_size, filter, true, false);
}

bitset_deque domain_utils::combinations_helper(unsigned long set_size, const boost::dynamic_bitset<> &filter,
                                               bool to_check_filter, bool include_elems) {
    bitset_deque result;
    auto n = static_cast<unsigned long>(std::exp2(set_size));

    for (unsigned long i = 0; i < n; ++i) {
        boost::dynamic_bitset<> bs(set_size);
        unsigned long j = i, pos = set_size - 1;

        while (j != 0) {
            bs[pos--] = j % 2;
            j /= 2;
        }

        bool is_good_combination = include_elems ? filter.is_subset_of(bs) : (bs & filter).none();

        if (not to_check_filter or is_good_combination)
            result.push_back(bs);
    }
    return result;
}

// The main recursive method to print all possible strings of length "length"
std::deque<ulong_vector> domain_utils::all_permutations_with_repetitions(const ulong_vector &v, const unsigned long length) {
    std::deque<ulong_vector> permutations;
    return all_permutations_with_repetitions_helper(permutations, v, {}, v.size(), length);
}

std::deque<ulong_vector>
domain_utils::all_permutations_with_repetitions_helper(std::deque<ulong_vector> &permutations, const ulong_vector &v,
                                         const ulong_vector &prefix, const unsigned long n, const unsigned long length) {
    //Base case: length = 1, print the string "length" times + the remaining letter
    if (length == 1) {
        for (unsigned long i = 0; i < n; i++) {
            ulong_vector v_ = prefix;
            v_.emplace_back(v[i]);
            permutations.push_back(std::move(v_));
        }
    } else {
        // One by one add all characters from "str" and recursively call for "length" equals to "length"-1
        for (unsigned long i = 0; i < n; i++) {
            // Next character of input added
            ulong_vector prefix_ = prefix;
            prefix_.emplace_back(v[i]);
            permutations = all_permutations_with_repetitions_helper(permutations, v, prefix_, n, length - 1);
        }
        // "length" is decreased, because we have added a new character
    }
    return permutations;
}
