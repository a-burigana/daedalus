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

#ifndef BEP_DOMAIN_UTILS_H
#define BEP_DOMAIN_UTILS_H

#include "../../../include/del/language/language.h"
#include "../../../include/del/del_types.h"
#include "../../../include/del/formulae/formula.h"
#include <deque>

namespace daedalus::tester {
    using bitset_deque = std::deque<boost::dynamic_bitset<>>;
    using ulong_vector  = std::vector<unsigned long>;

    class domain_utils {
    public:
        static bitset_deque all_combinations(unsigned long set_size);
        static bitset_deque all_combinations_with(unsigned long set_size, const boost::dynamic_bitset<> &filter);
        static bitset_deque all_combinations_without(unsigned long set_size, const boost::dynamic_bitset<> &filter);
        static std::deque<ulong_vector> all_permutations_with_repetitions(const ulong_vector &v, const unsigned long length);

    private:
        static bitset_deque combinations_helper(unsigned long set_size, const boost::dynamic_bitset<> &filter = {},
                                                bool to_check_filter = false, bool include_elems = false);

        static std::deque<ulong_vector>
        all_permutations_with_repetitions_helper(std::deque<ulong_vector> &permutations, const ulong_vector &v,
                                                 const ulong_vector &prefix, const unsigned long n, const unsigned long length);
    };
}

#endif //BEP_DOMAIN_UTILS_H
