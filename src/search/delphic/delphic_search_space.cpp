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

#include "../../../include/search/delphic/delphic_search_space.h"

using namespace search;

delphic_node::delphic_node(unsigned long long id, delphic::possibility_spectrum_ptr state, delphic::eventuality_spectrum_ptr action,
           delphic_node_ptr parent) :
        m_id{id},
        m_state{std::move(state)},
        m_action{std::move(action)},
        m_parent{std::move(parent)} {
    m_tree_depth = m_parent ? m_parent->get_tree_depth() + 1 : 0;
}

unsigned long long delphic_node::get_id() const {
    return m_id;
}

unsigned long long delphic_node::get_tree_depth() const {
    return m_tree_depth;
}

delphic::possibility_spectrum_ptr delphic_node::get_state() const {
    return m_state;
}

delphic::eventuality_spectrum_ptr delphic_node::get_action() const {
    return m_action;
}

delphic_node_ptr delphic_node::get_parent() const {
    return m_parent;
}

const delphic_node_deque &delphic_node::get_children() const {
    return m_children;
}

void delphic_node::add_child(const delphic_node_ptr &child) {
    m_children.emplace_back(child);
}
