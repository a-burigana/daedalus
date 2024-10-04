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

#ifndef DAEDALUS_DELPHIC_SEARCH_SPACE_H
#define DAEDALUS_DELPHIC_SEARCH_SPACE_H

#include "../search_types.h"
#include "../../del/semantics/delphic/actions/eventuality_types.h"
#include "../../del/semantics/delphic/actions/eventuality_spectrum.h"
#include "../../del/semantics/delphic/states/possibility_types.h"
#include "../../del/semantics/delphic/states/possibility_spectrum.h"

namespace search {
    class delphic_node {
    public:
        delphic_node(unsigned long long id, delphic::possibility_spectrum_ptr state, delphic::eventuality_spectrum_ptr action, delphic_node_ptr parent = nullptr);

        delphic_node(const delphic_node&) = delete;
        delphic_node& operator=(const delphic_node&) = delete;

        delphic_node(delphic_node&&) = default;
        delphic_node& operator=(delphic_node&&) = default;

        ~delphic_node() = default;

        [[nodiscard]] unsigned long long get_id() const;
        [[nodiscard]] unsigned long long get_tree_depth() const;

        [[nodiscard]] delphic::possibility_spectrum_ptr get_state() const;
        [[nodiscard]] delphic::eventuality_spectrum_ptr get_action() const;
        [[nodiscard]] delphic_node_ptr get_parent() const;
        [[nodiscard]] const delphic_node_deque &get_children() const;

        void add_child(const delphic_node_ptr &node);

    private:
        unsigned long long m_id, m_tree_depth;

        delphic::possibility_spectrum_ptr m_state;
        delphic::eventuality_spectrum_ptr m_action;

        delphic_node_ptr m_parent;
        delphic_node_deque m_children;
    };
}

#endif //DAEDALUS_DELPHIC_SEARCH_SPACE_H
