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

#ifndef DAEDALUS_SEARCH_TYPES_H
#define DAEDALUS_SEARCH_TYPES_H

#include <memory>
#include <chrono>
#include <deque>
#include <list>
#include <unordered_set>
#include <variant>
#include "../del/semantics/kripke/states/states_types.h"

namespace search {
    class node;
    using node_ptr   = std::shared_ptr<node>;
    using node_deque = std::list<node_ptr>;

    using states_ids_set = std::unordered_set<kripke::state_id>;
    using visited_states = std::variant<states_ids_set, kripke::state_set>;

    class delphic_node;
    using delphic_node_ptr   = std::shared_ptr<delphic_node>;
    using delphic_node_deque = std::deque<delphic_node_ptr>;

    struct statistics {
        unsigned long m_plan_length{};
        double m_computation_time{};
        unsigned long long m_visited_states_no{};
        unsigned long long m_visited_worlds_no{};
        unsigned long long m_graph_depth{};
        unsigned long long m_non_revisited_states_no{};       // Number of computed states that are discarded because already visited
        unsigned long m_iterations_no{};
        unsigned long m_plan_bound{};
    };
}
#endif //DAEDALUS_SEARCH_TYPES_H
