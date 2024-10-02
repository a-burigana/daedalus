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

#ifndef DAEDALUS_DOMAIN_UTILS_H
#define DAEDALUS_DOMAIN_UTILS_H

#include "../../../include/del/language/language.h"
#include "../../../include/del/del_types.h"
#include "../../../include/del/formulas/formula.h"
#include <deque>

namespace daedalus::tester {
    using bitset_deque = std::deque<boost::dynamic_bitset<>>;
    using ulong_vector = std::vector<unsigned long>;
    using combination_checker = std::function<bool(const boost::dynamic_bitset<> &)>;

    class domain_utils {
    public:
        static constexpr inline size_t binom(size_t n, size_t k) noexcept {
            return
                    (        k> n  )? 0 :          // out of range
                    (k==0 || k==n  )? 1 :          // edge
                    (k==1 || k==n-1)? n :          // first
                    (     k+k < n  )?              // recursive:
                    (binom(n-1,k-1) * n)/k :       //  path to k=1   is faster
                    (binom(n-1,k) * n)/(n-k);      //  path to k=n-1 is faster
        }

        static bitset_deque all_combinations(unsigned long set_size);
        static bitset_deque all_non_empty_combinations(unsigned long set_size);

        static bitset_deque all_combinations_with(unsigned long set_size, const boost::dynamic_bitset<> &filter);
        static bitset_deque all_combinations_without(unsigned long set_size, const boost::dynamic_bitset<> &filter);

        static bitset_deque all_sized_combinations(unsigned long set_size, unsigned long comb_size);
        static bitset_deque all_sized_combinations_with(unsigned long set_size, unsigned long comb_size, const boost::dynamic_bitset<> &filter);
        static bitset_deque all_sized_combinations_without(unsigned long set_size, unsigned long comb_size, const boost::dynamic_bitset<> &filter);

        static std::deque<ulong_vector> all_permutations_with_repetitions(const ulong_vector &v, unsigned long length);

    private:
        static bitset_deque combinations_helper(unsigned long set_size, const combination_checker &checker);
//        , const boost::dynamic_bitset<> &filter = {},
//                                                bool to_check_filter = false, bool include_elems = false,
//                                                bool is_fixed_comb_size = false, unsigned long comb_size = 0);

        static std::deque<ulong_vector>
        all_permutations_with_repetitions_helper(std::deque<ulong_vector> &permutations, const ulong_vector &v,
                                                 const ulong_vector &prefix, unsigned long n, unsigned long length);
    };
}

#endif //DAEDALUS_DOMAIN_UTILS_H
