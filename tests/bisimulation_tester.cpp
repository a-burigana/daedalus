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

#include <string>
#include "bisimulation_tester.h"
#include "builder/domains/consecutive_numbers.h"
#include "builder/state_builder.h"
#include "../include/del/semantics/kripke/bisimulation/bisimulator.h"
#include "printer.h"

using namespace daedalus::tester;
using namespace del;

void bisimulation_tester::test_bisim_1(const std::string &out_path, unsigned long k) {
    std::string s1_dir = "s1/";

    state s1 = state_builder::build_test_state1();
    printer::print_state(s1, out_path + s1_dir, "s1");

    state s1_contr = bisimulation_tester::test_full_bisim_1();
    printer::print_state(s1_contr, out_path + s1_dir, "s1_contr");

    auto [is_s1_bisim, s1_k_contr] = bisimulation_tester::test_bounded_bisim_1(k);
//    assert(is_s1_bisim);
    printer::print_state(s1_k_contr, out_path + s1_dir, "s1_" + std::to_string(k) + "_contr");
}

void bisimulation_tester::test_bisim_singleton(const std::string &out_path, unsigned long k) {
    std::string singleton_dir = "singleton/";

    state singleton = state_builder::build_singleton(true);
    printer::print_state(singleton, out_path + singleton_dir, "singleton");

    state singleton_contr = bisimulation_tester::test_full_bisim_singleton(true);
    printer::print_state(singleton_contr, out_path + singleton_dir, "singleton_contr");

    auto [is_singleton_bisim, singleton_k_contr] = bisimulation_tester::test_bounded_bisim_singleton(k, true);
    printer::print_state(singleton_k_contr, out_path + singleton_dir, "singleton_" + std::to_string(k) + "_contr");
}

void bisimulation_tester::test_bisim_singleton_no_loop(const std::string &out_path, unsigned long k) {
    std::string singleton_no_loop_dir = "singleton_no_loop/";

    state singleton_no_loop = state_builder::build_singleton(false);
    printer::print_state(singleton_no_loop, out_path + singleton_no_loop_dir, "singleton_no_loop");

    state singleton_no_loop_contr = bisimulation_tester::test_full_bisim_singleton(false);
    printer::print_state(singleton_no_loop_contr, out_path + singleton_no_loop_dir, "singleton_no_loop_contr");

    auto [is_singleton_no_loop_bisim, singleton_no_loop_k_contr] = bisimulation_tester::test_bounded_bisim_singleton(k, false);
    printer::print_state(singleton_no_loop_k_contr, out_path + singleton_no_loop_dir, "singleton_no_loop_" + std::to_string(k) + "_contr");
}

void bisimulation_tester::test_bisim_chain(const std::string &out_path, unsigned long k, unsigned long length, bool all_designated) {
    std::string chain_dir = all_designated ? "chain_des/" : "chain/";

    state chain = state_builder::build_chain(length, true, all_designated);
    printer::print_state(chain, out_path + chain_dir, "chain");

    state chain_contr = bisimulation_tester::test_full_bisim_chain(length, true, all_designated);
    printer::print_state(chain_contr, out_path + chain_dir, "chain_contr");

    auto [is_chain_bisim, chain_k_contr] = bisimulation_tester::test_bounded_bisim_chain(k, length, true, all_designated);
    printer::print_state(chain_k_contr, out_path + chain_dir, "chain_" + std::to_string(k) + "_contr");
}

void bisimulation_tester::test_bisim_chain_no_loop(const std::string &out_path, unsigned long k, unsigned long length, bool all_designated) {
    std::string chain_no_loop_dir = all_designated ? "chain_no_loop_des/" : "chain_no_loop/";

    state chain_no_loop = state_builder::build_chain(k, false, all_designated);
    printer::print_state(chain_no_loop, out_path + chain_no_loop_dir, "chain_no_loop");

    state chain_no_loop_contr = bisimulation_tester::test_full_bisim_chain(k, false, all_designated);
    printer::print_state(chain_no_loop_contr, out_path + chain_no_loop_dir, "chain_no_loop_contr");

    auto [is_chain_no_loop_bisim, chain_no_loop_k_contr] = bisimulation_tester::test_bounded_bisim_chain(k, length, false, all_designated);
    printer::print_state(chain_no_loop_k_contr, out_path + chain_no_loop_dir, "chain_no_loop_" + std::to_string(k) + "_contr");
}

void bisimulation_tester::test_bisim_k_tree(const std::string &out_path, unsigned long k) {
    std::string tree_dir = "k_tree/" + std::to_string(k) + "/";

    state tree = state_builder::build_k_tree(k);
    printer::print_state(tree, out_path + tree_dir, "tree");

    state tree_contr = bisimulation_tester::test_full_bisim_k_tree(k);
    printer::print_state(tree_contr, out_path + tree_dir, "tree_contr");

    auto [is_tree_bisim, tree_k_contr] = bisimulation_tester::test_bounded_bisim_k_tree(k);
    printer::print_state(tree_k_contr, out_path + tree_dir, "tree_" + std::to_string(k) + "_contr");
}

void bisimulation_tester::test_bisim_cn(const std::string &out_path, unsigned long n, unsigned long k) {
    std::string cn_dir = "consecutive_numbers/" + std::to_string(n) + "/";

    state cn = consecutive_numbers::build_initial_state(n);
    printer::print_state(cn, out_path + cn_dir, "cn");

    state cn_contr = bisimulation_tester::test_full_bisim_cn(n, k);
    printer::print_state(cn_contr, out_path + cn_dir, "cn_contr");

    auto [is_cn_bisim, cn_k_contr] = bisimulation_tester::test_bounded_bisim_cn(n, k);
    printer::print_state(cn_k_contr, out_path + cn_dir, "cn_" + std::to_string(k) + "_contr");
}


state bisimulation_tester::test_full_bisim_1() {
    state s = state_builder::build_test_state1();
    auto [is_bisim, s_contr] = bisimulator::contract(bisimulation_type::full, s);
    return std::move(s_contr);
}

std::pair<bool, state> bisimulation_tester::test_bounded_bisim_1(const unsigned long k) {
    state s = state_builder::build_test_state1();
    return bisimulator::contract(bisimulation_type::bounded, s, k);
}


state bisimulation_tester::test_full_bisim_singleton(bool has_loop) {
    state s = state_builder::build_singleton(has_loop);
    auto [is_bisim, s_contr] = bisimulator::contract(bisimulation_type::full, s);
    return std::move(s_contr);
}

std::pair<bool, state> bisimulation_tester::test_bounded_bisim_singleton(const unsigned long k, bool has_loop) {
    state s = state_builder::build_singleton(has_loop);
    return bisimulator::contract(bisimulation_type::bounded, s, k);
}


state bisimulation_tester::test_full_bisim_chain(const unsigned long length, bool has_final_loop, bool all_designated) {
    state s = state_builder::build_chain(length, has_final_loop, all_designated);
    auto [is_bisim, s_contr] = bisimulator::contract(bisimulation_type::full, s);
    return std::move(s_contr);
}

std::pair<bool, state> bisimulation_tester::test_bounded_bisim_chain(const unsigned long k, const unsigned long length, bool has_final_loop, bool all_designated) {
    state s = state_builder::build_chain(length, has_final_loop, all_designated);
    return bisimulator::contract(bisimulation_type::bounded, s, k);
}


state bisimulation_tester::test_full_bisim_k_tree(unsigned long k) {
    state s = state_builder::build_k_tree(k);
    auto [is_bisim, s_contr] = bisimulator::contract(bisimulation_type::full, s);
    return std::move(s_contr);
}

std::pair<bool, state> bisimulation_tester::test_bounded_bisim_k_tree(unsigned long k) {
    state s = state_builder::build_k_tree(k);
    return bisimulator::contract(bisimulation_type::bounded, s, k);
}

state bisimulation_tester::test_full_bisim_cn(unsigned long n, unsigned long k) {
    state s = consecutive_numbers::build_initial_state(n);
    auto [is_bisim, s_contr] = bisimulator::contract(bisimulation_type::full, s);
    return std::move(s_contr);
}

std::pair<bool, state> bisimulation_tester::test_bounded_bisim_cn(unsigned long n, unsigned long k) {
    state s = consecutive_numbers::build_initial_state(n);
    return bisimulator::contract(bisimulation_type::bounded, s, k);
}
