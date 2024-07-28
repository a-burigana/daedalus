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

#ifndef BEP_BISIMULATION_TESTER_H
#define BEP_BISIMULATION_TESTER_H

#include "../include/del/states/state.h"

namespace daedalus::tester {
    class bisimulation_tester {
    public:
        static void test_bisim_1(const std::string &out_path, unsigned long k);

        static void test_bisim_singleton(const std::string &out_path, unsigned long k);
        static void test_bisim_singleton_no_loop(const std::string &out_path, unsigned long k);

        static void test_bisim_chain(const std::string &out_path, unsigned long k, unsigned long length, bool all_designated = false);
        static void test_bisim_chain_no_loop(const std::string &out_path, unsigned long k, unsigned long length, bool all_designated = false);

        static void test_bisim_k_tree(const std::string &out_path, unsigned long k);

        static void test_bisim_cn(const std::string &out_path, unsigned long n, unsigned long k);

        static del::state test_full_bisim_1();
        static del::state test_full_bisim_singleton(bool has_loop);
        static del::state test_full_bisim_chain(unsigned long length, bool has_final_loop, bool all_designated = false);
        static del::state test_full_bisim_k_tree(unsigned long k);
        static del::state test_full_bisim_cn(unsigned long n, unsigned long k);

        static std::pair<bool, del::state> test_bounded_bisim_1(unsigned long k);
        static std::pair<bool, del::state> test_bounded_bisim_singleton(unsigned long k, bool has_loop);
        static std::pair<bool, del::state> test_bounded_bisim_chain(unsigned long k, unsigned long length, bool has_final_loop, bool all_designated = false);
        static std::pair<bool, del::state> test_bounded_bisim_k_tree(unsigned long k);
        static std::pair<bool, del::state> test_bounded_bisim_cn(unsigned long n, unsigned long k);
    };
}

#endif //BEP_BISIMULATION_TESTER_H
