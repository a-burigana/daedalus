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

#ifndef BEP_SEARCH_SPACE_H
#define BEP_SEARCH_SPACE_H

#include <memory>
#include <list>
#include "search_types.h"
#include "../del/states/state.h"
#include "../del/actions/action.h"
#include "../del/bisimulation/bounded_bisimulation_types.h"

namespace search {
    class node {
    public:
        node(unsigned long long id, del::state_ptr state, del::action_ptr action, unsigned long bound, bool is_bisim,
             node_ptr parent = nullptr);

        node(const node&) = delete;
        node& operator=(const node&) = delete;

        node(node&&) = default;
        node& operator=(node&&) = default;

        ~node() = default;

        [[nodiscard]] unsigned long long get_id() const;
        [[nodiscard]] unsigned long long get_tree_depth() const;

        [[nodiscard]] del::state_ptr get_state() const;
        [[nodiscard]] del::state_ptr get_original_state() const;
        [[nodiscard]] del::action_ptr get_action() const;
        [[nodiscard]] unsigned long get_bound() const;
        [[nodiscard]] bool is_bisim() const;

        [[nodiscard]] node_ptr get_parent() const;
        [[nodiscard]] const node_deque &get_children() const;
        [[nodiscard]] const del::action_deque &get_to_apply_actions() const;

//        [[nodiscard]] del::bpr_structures &get_bpr_structures();

        void set_state(del::state_ptr s);
        void increment_bound();
        void set_is_bisim(bool is_bisim);
        void add_child(const node_ptr &child);
        void clear_non_bisim_children();
        void set_to_apply_action(del::action_deque actions);

        void set_original_state(del::state_ptr s);
        void clear_original_state();

    private:
        unsigned long long m_id, m_tree_depth;

        del::state_ptr m_state, m_original_state;
        del::action_ptr m_action;
        unsigned long m_bound;
        bool m_is_bisim;

        node_ptr m_parent;
        node_deque m_children, m_non_bisim_children;
        del::action_deque m_to_apply_actions;

//        std::optional<del::bpr_structures> m_structures;
    };
}

#endif //BEP_SEARCH_SPACE_H
