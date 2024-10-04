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

#ifndef DAEDALUS_DELPHIC_PLANNER_H
#define DAEDALUS_DELPHIC_PLANNER_H

#include <functional>
#include <list>
#include <memory>
#include <queue>
#include "delphic_planning_task.h"
#include "delphic_search_space.h"
#include "../../../tests/printer.h"
#include "../strategies.h"

namespace search {
    class delphic_planner {
    public:
        static delphic_node_deque search(const delphic_planning_task &task, strategy strategy, const daedalus::tester::printer_ptr &printer = nullptr);

        static void print_plan(const delphic_node_deque &path);

    private:
        static delphic_node_deque unbounded_search(const delphic_planning_task &task, const daedalus::tester::printer_ptr &printer);
        static delphic_node_deque iterative_bounded_search(const delphic_planning_task &task, const daedalus::tester::printer_ptr &printer);

        static delphic_node_deque bfs(const delphic_planning_task &task, strategy strategy,
                              unsigned long b, unsigned long long &id, const daedalus::tester::printer_ptr &printer);

        static delphic_node_deque expand_node(const delphic_planning_task &task, strategy strategy, delphic_node_ptr &n,
                                      const delphic::action_deque &actions, delphic_node_deque &frontier, unsigned long goal_depth,
                                      unsigned long long &id, const daedalus::tester::printer_ptr &printer);

        static delphic_node_deque extract_path(delphic_node_ptr n);

        static search::delphic_node_ptr update_node(strategy strategy, const delphic_node_ptr &n, const delphic::eventuality_spectrum_ptr &a,
                                            unsigned long long &id, unsigned long goal_depth = 0);

        static search::delphic_node_ptr init_node(strategy strategy, const delphic::possibility_spectrum_ptr &s, const delphic::eventuality_spectrum_ptr &a,
                                          bool was_bisim, const delphic_node_ptr &parent, unsigned long long id, unsigned long b = 0);

        // Print utilities
        static void print_max_tree_depth(const daedalus::tester::printer_ptr &printer, unsigned long long max_tree_depth);
        static void print_node_id(const daedalus::tester::printer_ptr &printer, const delphic_node_ptr &n);
        static void print_begin_expanding_node(const daedalus::tester::printer_ptr &printer, const delphic_node_ptr &n, strategy strategy);
        static void print_goal_found(const daedalus::tester::printer_ptr &printer, const delphic_node_ptr &n);
        static void print_applying_action(const daedalus::tester::printer_ptr &printer, const delphic::eventuality_spectrum_ptr &a, const delphic_node_ptr &n_,
                                          strategy strategy);
        static void print_end_expanding_node(const daedalus::tester::printer_ptr &printer, const delphic_node_ptr &n, strategy strategy,
                                             bool is_dead_node, bool is_fully_expanded_node);
    };
}

#endif //DAEDALUS_DELPHIC_PLANNER_H
