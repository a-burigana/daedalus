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

#include <optional>
#include <utility>

#include "../../include/search/search_space.h"

using namespace search;

node::node(unsigned long long id, kripke::state_ptr state, kripke::action_ptr action, unsigned long bound, bool is_bisim,
           node_ptr parent) :
        m_id{id},
        m_state{std::move(state)},
        m_action{std::move(action)},
        m_bound{bound},
        m_is_bisim{is_bisim},
        m_parent{std::move(parent)} {
    m_tree_depth = m_parent ? m_parent->get_tree_depth() + 1 : 0;
}

unsigned long long node::get_id() const {
    return m_id;
}

unsigned long long node::get_tree_depth() const {
    return m_tree_depth;
}

kripke::state_ptr node::get_state() const {
    return m_state;
}

kripke::state_ptr node::get_original_state() const {
    return m_original_state;
}

kripke::action_ptr node::get_action() const {
    return m_action;
}

unsigned long node::get_bound() const {
    return m_bound;
}

bool node::is_bisim() const {
    return m_is_bisim;
}

node_ptr node::get_parent() const {
    return m_parent;
}

const node_deque &node::get_children() const {
    return m_children;
}

const kripke::action_deque &node::get_to_apply_actions() const {
    return m_to_apply_actions;
}

//del::bpr_structures &node::get_bpr_structures() {
//    return *m_structures;
//}

void node::set_state(kripke::state_ptr s) {
    m_state = std::move(s);
}

void node::increment_bound() {
    ++m_bound;
}

void node::set_is_bisim(bool is_bisim) {
    m_is_bisim = is_bisim;
}

void node::add_child(const node_ptr &child) {
    if (child->is_bisim())
        m_children.emplace_back(child);
    else
        m_non_bisim_children.emplace_back(child);
}

void node::clear_non_bisim_children() {
    m_non_bisim_children.clear();
}

void node::set_to_apply_action(kripke::action_deque actions) {
    m_to_apply_actions = std::move(actions);
}

void node::set_original_state(kripke::state_ptr s) {
    m_original_state = s;
}

void node::clear_original_state() {
    m_original_state = nullptr;
}
