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

#ifndef BEP_PLANNER_H
#define BEP_PLANNER_H

#include <functional>
#include <list>
#include <memory>
#include <queue>
#include "planning_task.h"
#include "search_space.h"
#include "../del/states/state.h"
#include "../del/update/updater.h"
#include "../del/bisimulation/bisimulator.h"
#include "../../tests/printer.h"
#include "strategies.h"

namespace search {
    class planner {
    public:
        static node_deque search(const planning_task &task, strategy strategy, const bep::tester::printer_ptr &printer = nullptr);

        static void print_plan(const node_deque &path);

    private:
        static node_deque unbounded_search(const planning_task &task, const bep::tester::printer_ptr &printer);
        static node_deque iterative_bounded_search(const planning_task &task, const bep::tester::printer_ptr &printer);

        static node_deque bounded_search(const planning_task &task, node_deque &previous_iter_frontier, unsigned long b,
                                         unsigned long long &id, const bep::tester::printer_ptr &printer);

        static node_deque bfs(const planning_task &task, strategy strategy, node_deque &previous_iter_frontier,
                              unsigned long b, unsigned long long &id, const bep::tester::printer_ptr &printer);

        static node_deque init_frontier(del::state_ptr &s0, strategy strategy, unsigned long b,
                                        node_deque &previous_iter_frontier);

        static node_deque expand_node(const planning_task &task, strategy strategy, node_ptr &n,
                                      const del::action_deque &actions, node_deque &frontier, unsigned long goal_depth,
                                      unsigned long long &id, const bep::tester::printer_ptr &printer);
        
        static node_deque extract_path(node_ptr n);

        static search::node_ptr update_node(strategy strategy, const node_ptr &n, const del::action_ptr &a,
                                            unsigned long long &id, unsigned long goal_depth = 0);

        static void refresh_node(node_ptr &n);

        static search::node_ptr init_node(strategy strategy, const del::state_ptr &s, const del::action_ptr &a,
                                          bool was_bisim, const node_ptr &parent, unsigned long long id, unsigned long b = 0);

        // Print utilities
        static void print_max_tree_depth(const bep::tester::printer_ptr &printer, unsigned long long max_tree_depth);
        static void print_node_id(const bep::tester::printer_ptr &printer, const node_ptr &n);
        static void print_begin_expanding_node(const bep::tester::printer_ptr &printer, const node_ptr &n, strategy strategy);
        static void print_goal_found(const bep::tester::printer_ptr &printer, const node_ptr &n);
        static void print_applying_action(const bep::tester::printer_ptr &printer, const del::action_ptr &a, const node_ptr &n_,
                                          strategy strategy);
        static void print_end_expanding_node(const bep::tester::printer_ptr &printer, const node_ptr &n, strategy strategy,
                                             bool is_dead_node, bool is_fully_expanded_node);
    };
}

#endif //BEP_PLANNER_H
