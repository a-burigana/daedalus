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
#include "../builder/domains/consecutive_numbers.h"
#include "../builder/state_builder.h"
#include "../../include/del/semantics/kripke/bisimulation/bisimulator.h"
#include "../printer.h"

#define BISIM_PATH std::string{"bisimulation/"}

using namespace daedalus::tester;
using namespace del;

void bisimulation_tester::test_bisim_1(const std::string &out_path, unsigned long k, const del::storages_ptr &storages) {
    std::string s1_dir = "s1/";

    state s1 = state_builder::build_test_state1(storages->l_storage);
    printer::print_state(s1, storages->l_storage, out_path + BISIM_PATH + s1_dir, "s1");

    auto [is_bisim, s1_contr] = bisimulator::contract(contraction_type::full, s1);
    printer::print_state(s1_contr, storages->l_storage, out_path + BISIM_PATH + s1_dir, "s1_contr");

    auto [is_s1_bisim, s1_k_contr] = bisimulator::contract(contraction_type::rooted, s1, k);
//    assert(is_s1_bisim);
    printer::print_state(s1_k_contr, storages->l_storage, out_path + BISIM_PATH + s1_dir, "s1_" + std::to_string(k) + "_contr");

    auto [is_s1_bisim_id, s1_k_contr_id] = bisimulator::contract(contraction_type::canonical, s1, k, storages);
    printer::print_state(s1_k_contr_id, storages->l_storage, out_path + BISIM_PATH + s1_dir, "s1_" + std::to_string(k) + "_can_contr");
}

void bisimulation_tester::test_bisim_singleton(const std::string &out_path, unsigned long k, const del::storages_ptr &storages) {
    std::string singleton_dir = "singleton/";

    state singleton = state_builder::build_singleton(true, storages->l_storage);
    printer::print_state(singleton, storages->l_storage, out_path + BISIM_PATH + singleton_dir, "singleton");

    state singleton_contr = bisimulation_tester::test_full_bisim_singleton(true, storages->l_storage);
    printer::print_state(singleton_contr, storages->l_storage, out_path + BISIM_PATH + singleton_dir, "singleton_contr");

    auto [is_singleton_bisim, singleton_k_contr] = bisimulation_tester::test_bounded_bisim_singleton(k, true, storages->l_storage);
    printer::print_state(singleton_k_contr, storages->l_storage, out_path + BISIM_PATH + singleton_dir, "singleton_" + std::to_string(k) + "_contr");

    auto [is_singleton_bisim_id, singleton_k_contr_id] = bisimulator::contract(contraction_type::canonical, singleton, k, storages);
    printer::print_state(singleton_k_contr_id, storages->l_storage, out_path + BISIM_PATH + singleton_dir, "singleton_" + std::to_string(k) + "_can_contr");
}

void bisimulation_tester::test_bisim_singleton_no_loop(const std::string &out_path, unsigned long k, const del::storages_ptr &storages) {
    std::string singleton_no_loop_dir = "singleton_no_loop/";

    state singleton_no_loop = state_builder::build_singleton(false, storages->l_storage);
    printer::print_state(singleton_no_loop, storages->l_storage, out_path + BISIM_PATH + singleton_no_loop_dir, "singleton_no_loop");

    state singleton_no_loop_contr = bisimulation_tester::test_full_bisim_singleton(false, storages->l_storage);
    printer::print_state(singleton_no_loop_contr, storages->l_storage, out_path + BISIM_PATH + singleton_no_loop_dir, "singleton_no_loop_contr");

    auto [is_singleton_no_loop_bisim, singleton_no_loop_k_contr] = bisimulation_tester::test_bounded_bisim_singleton(k, false, storages->l_storage);
    printer::print_state(singleton_no_loop_k_contr, storages->l_storage, out_path + BISIM_PATH + singleton_no_loop_dir, "singleton_no_loop_" + std::to_string(k) + "_contr");

    auto [is_singleton_no_loop_bisim_id, singleton_no_loop_k_contr_id] = bisimulator::contract(contraction_type::canonical, singleton_no_loop, k, storages);
    printer::print_state(singleton_no_loop_k_contr_id, storages->l_storage, out_path + BISIM_PATH + singleton_no_loop_dir, "singleton_no_loop_" + std::to_string(k) + "_can_contr");
}

void bisimulation_tester::test_bisim_chain(const std::string &out_path, unsigned long k, unsigned long length,
                                           const del::storages_ptr &storages, bool all_designated) {
    std::string chain_dir = all_designated ? "chain_des/" : "chain/";

    state chain = state_builder::build_chain(length, storages->l_storage, true, all_designated);
    printer::print_state(chain, storages->l_storage, out_path + BISIM_PATH + chain_dir, "chain");

    state chain_contr = bisimulation_tester::test_full_bisim_chain(length, storages->l_storage, true, all_designated);
    printer::print_state(chain_contr, storages->l_storage, out_path + BISIM_PATH + chain_dir, "chain_contr");

    auto [is_chain_bisim, chain_k_contr] = bisimulation_tester::test_bounded_bisim_chain(k, storages->l_storage, length, true, all_designated);
    printer::print_state(chain_k_contr, storages->l_storage, out_path + BISIM_PATH + chain_dir, "chain_" + std::to_string(k) + "_contr");

    auto [is_chain_bisim_id, chain_k_contr_id] = bisimulator::contract(contraction_type::canonical, chain, k, storages);
    printer::print_state(chain_k_contr_id, storages->l_storage, out_path + BISIM_PATH + chain_dir, "chain_" + std::to_string(k) + "_can_contr");
}

void bisimulation_tester::test_bisim_chain_no_loop(const std::string &out_path, unsigned long k, unsigned long length, const del::storages_ptr &storages,
                                                   bool all_designated) {
    std::string chain_no_loop_dir = all_designated ? "chain_no_loop_des/" : "chain_no_loop/";

    state chain_no_loop = state_builder::build_chain(k, storages->l_storage, false, all_designated);
    printer::print_state(chain_no_loop, storages->l_storage, out_path + BISIM_PATH + chain_no_loop_dir, "chain_no_loop");

    state chain_no_loop_contr = bisimulation_tester::test_full_bisim_chain(k, storages->l_storage, false, all_designated);
    printer::print_state(chain_no_loop_contr, storages->l_storage, out_path + BISIM_PATH + chain_no_loop_dir, "chain_no_loop_contr");

    auto [is_chain_no_loop_bisim, chain_no_loop_k_contr] = bisimulation_tester::test_bounded_bisim_chain(k, storages->l_storage, length, false, all_designated);
    printer::print_state(chain_no_loop_k_contr, storages->l_storage, out_path + BISIM_PATH + chain_no_loop_dir, "chain_no_loop_" + std::to_string(k) + "_contr");

    auto [is_chain_no_loop_bisim_id, chain_no_loop_k_contr_id] = bisimulator::contract(contraction_type::canonical, chain_no_loop, k, storages);
    printer::print_state(chain_no_loop_k_contr_id, storages->l_storage, out_path + BISIM_PATH + chain_no_loop_dir, "chain_no_loop_" + std::to_string(k) + "_can_contr");
}

void bisimulation_tester::test_bisim_k_tree(const std::string &out_path, unsigned long k, const del::storages_ptr &storages) {
    std::string tree_dir = "k_tree/" + std::to_string(k) + "/";

    state tree = state_builder::build_k_tree(k, storages->l_storage);
    printer::print_state(tree, storages->l_storage, out_path + BISIM_PATH + tree_dir, "tree");

    state tree_contr = bisimulation_tester::test_full_bisim_k_tree(k, storages->l_storage);
    printer::print_state(tree_contr, storages->l_storage, out_path + BISIM_PATH + tree_dir, "tree_contr");

    auto [is_tree_bisim, tree_k_contr] = bisimulation_tester::test_bounded_bisim_k_tree(k, storages->l_storage);
    printer::print_state(tree_k_contr, storages->l_storage, out_path + BISIM_PATH + tree_dir, "tree_" + std::to_string(k) + "_contr");

    auto [is_tree_bisim_id, tree_k_contr_id] = bisimulator::contract(contraction_type::canonical, tree, k, storages);
    printer::print_state(tree_k_contr_id, storages->l_storage, out_path + BISIM_PATH + tree_dir, "tree_" + std::to_string(k) + "_can_contr");
}

void bisimulation_tester::test_bisim_cn(const std::string &out_path, unsigned long n, unsigned long k, const del::storages_ptr &storages) {
    std::string cn_dir = "consecutive_numbers/" + std::to_string(n) + "/";

    state cn = consecutive_numbers::build_initial_state(n, storages->l_storage);
    printer::print_state(cn, storages->l_storage, out_path + BISIM_PATH + cn_dir, "cn");

    state cn_contr = bisimulation_tester::test_full_bisim_cn(n, k, storages->l_storage);
    printer::print_state(cn_contr, storages->l_storage, out_path + BISIM_PATH + cn_dir, "cn_contr");

    auto [is_cn_bisim, cn_k_contr] = bisimulation_tester::test_bounded_bisim_cn(n, k, storages->l_storage);
    printer::print_state(cn_k_contr, storages->l_storage, out_path + BISIM_PATH + cn_dir, "cn_" + std::to_string(k) + "_contr");

    auto [is_cn_bisim_id, cn_k_contr_id] = bisimulator::contract(contraction_type::canonical, cn, k, storages);
    printer::print_state(cn_k_contr_id, storages->l_storage, out_path + BISIM_PATH + cn_dir, "cn_" + std::to_string(k) + "_can_contr");
}



// ******************************  FULL CONTRACTIONS  ******************************



state bisimulation_tester::test_full_bisim_1(const del::label_storage_ptr &l_storage) {
    state s = state_builder::build_test_state1(l_storage);
    auto [is_bisim, s_contr] = bisimulator::contract(contraction_type::full, s);
    return std::move(s_contr);
}

state bisimulation_tester::test_full_bisim_singleton(bool has_loop, const del::label_storage_ptr &l_storage) {
    state s = state_builder::build_singleton(has_loop, l_storage);
    auto [is_bisim, s_contr] = bisimulator::contract(contraction_type::full, s);
    return std::move(s_contr);
}

state bisimulation_tester::test_full_bisim_chain(const unsigned long length, const del::label_storage_ptr &l_storage, bool has_final_loop, bool all_designated) {
    state s = state_builder::build_chain(length, l_storage, has_final_loop, all_designated);
    auto [is_bisim, s_contr] = bisimulator::contract(contraction_type::full, s);
    return std::move(s_contr);
}

state bisimulation_tester::test_full_bisim_k_tree(unsigned long k, const del::label_storage_ptr &l_storage) {
    state s = state_builder::build_k_tree(k, l_storage);
    auto [is_bisim, s_contr] = bisimulator::contract(contraction_type::full, s);
    return std::move(s_contr);
}

state bisimulation_tester::test_full_bisim_cn(unsigned long n, unsigned long k, const del::label_storage_ptr &l_storage) {
    state s = consecutive_numbers::build_initial_state(n, l_storage);
    auto [is_bisim, s_contr] = bisimulator::contract(contraction_type::full, s);
    return std::move(s_contr);
}



// ******************************  ROOTED CONTRACTIONS  ******************************



std::pair<bool, state> bisimulation_tester::test_bounded_bisim_1(const unsigned long k, const del::label_storage_ptr &l_storage) {
    state s = state_builder::build_test_state1(l_storage);
    return bisimulator::contract(contraction_type::rooted, s, k);
}

std::pair<bool, state> bisimulation_tester::test_bounded_bisim_singleton(const unsigned long k, bool has_loop, const del::label_storage_ptr &l_storage) {
    state s = state_builder::build_singleton(has_loop, l_storage);
    return bisimulator::contract(contraction_type::rooted, s, k);
}

std::pair<bool, state> bisimulation_tester::test_bounded_bisim_chain(const unsigned long k, const del::label_storage_ptr &l_storage, const unsigned long length, bool has_final_loop, bool all_designated) {
    state s = state_builder::build_chain(length, l_storage, has_final_loop, all_designated);
    return bisimulator::contract(contraction_type::rooted, s, k);
}

std::pair<bool, state> bisimulation_tester::test_bounded_bisim_k_tree(unsigned long k, const del::label_storage_ptr &l_storage) {
    state s = state_builder::build_k_tree(k, l_storage);
    return bisimulator::contract(contraction_type::rooted, s, k);
}

std::pair<bool, state> bisimulation_tester::test_bounded_bisim_cn(unsigned long n, unsigned long k, const del::label_storage_ptr &l_storage) {
    state s = consecutive_numbers::build_initial_state(n, l_storage);
    return bisimulator::contract(contraction_type::rooted, s, k);
}
