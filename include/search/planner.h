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

#ifndef DAEDALUS_PLANNER_H
#define DAEDALUS_PLANNER_H

#include <functional>
#include <memory>
#include <queue>
#include "planning_task.h"
#include "search_space.h"
#include "../del/semantics/kripke/states/state.h"
#include "../del/semantics/kripke/update/updater.h"
#include "../del/semantics/kripke/bisimulation/bisimulator.h"
#include "../../tests/printer.h"
#include "search_types.h"
#include "strategies.h"
#include "frontier.h"

namespace search {
    class planner {
    public:
        static std::pair<node_deque, statistics>
        search(const planning_task &task, strategy strategy, contraction_type contraction_type,
               del::storages_handler_ptr handler, const daedalus::tester::printer_ptr &printer = nullptr);

        static void print_plan(const node_deque &path);

    private:
        static node_deque unbounded_search(const planning_task &task, statistics &stats, visited_states &visited_states,
                         del::storages_handler_ptr handler, const daedalus::tester::printer_ptr &printer);

        static node_deque
        iterative_bounded_search(const planning_task &task, strategy strategy, contraction_type contraction_type, statistics &stats,
                                 visited_states &visited_states, del::storages_handler_ptr handler,
                                 const daedalus::tester::printer_ptr &printer);

        static node_deque bounded_search(const planning_task &task, strategy strategy, contraction_type contraction_type, statistics &stats,
                                         frontier &previous_iter_frontier, unsigned long b,
                                         unsigned long long &id, visited_states &visited_states,
                                         del::storages_handler_ptr handler, const daedalus::tester::printer_ptr &printer);

        static node_deque bfs(const planning_task &task, strategy strategy, contraction_type contraction_type, statistics &stats,
                              frontier &previous_iter_frontier, unsigned long b, unsigned long long &id,
                              visited_states &visited_states, del::storages_handler_ptr handler,
                              const daedalus::tester::printer_ptr &printer);

        static frontier init_frontier(kripke::state_ptr &s0, strategy strategy, contraction_type contraction_type,
                                        unsigned long b, frontier &previous_iter_frontier, statistics &stats,
                                        visited_states &visited_states, del::storages_handler_ptr handler);

        static node_deque expand_node(const planning_task &task, strategy strategy, contraction_type contraction_type,
                                      statistics &stats, node_ptr &n, const kripke::action_deque &actions, frontier &frontier,
                                      unsigned long goal_depth, unsigned long long &id, visited_states &visited_states,
                                      del::storages_handler_ptr handler, const daedalus::tester::printer_ptr &printer);

        static void update_statistics(search::statistics &stats, search::node_ptr &n);

        static node_deque extract_path(node_ptr n, statistics &stats);

        static bool validate(const planning_task &task, const node_deque &path, del::storages_handler_ptr handler);

        static search::node_ptr update_node(strategy strategy, contraction_type contraction_type, const node_ptr &n,
                                            const kripke::action_ptr &a, unsigned long long &id,
                                            const visited_states &visited_states, del::storages_handler_ptr handler,
                                            unsigned long goal_depth = 0);

        static void refresh_node(node_ptr &n, contraction_type contraction_type, statistics &stats,
                                 visited_states &visited_states, del::storages_handler_ptr handler);

        static search::node_ptr init_node(contraction_type contraction_type, const kripke::state_ptr &s,
                                          const kripke::action_ptr &a, bool was_bisim, const node_ptr &parent, unsigned long long id,
                                          const visited_states &visited_states, del::storages_handler_ptr handler, unsigned long b = 0);

        static void update_visited_states(const kripke::state_ptr &s, visited_states &visited_states);

        static bool is_already_visited(const kripke::state &s, unsigned long b, const visited_states &visited_states, del::storages_handler_ptr handler);

        // Print utilities
        static void print_info(const planning_task &task, strategy strategy, contraction_type contraction_type);
        static void print_statistics(statistics &stats, strategy strategy);

        static void print_max_graph_depth(const daedalus::tester::printer_ptr &printer, unsigned long long max_graph_depth);
        static void print_node_id(const daedalus::tester::printer_ptr &printer, const node_ptr &n);
        static void print_begin_expanding_node(const daedalus::tester::printer_ptr &printer, const node_ptr &n, strategy strategy);
        static void print_goal_found(const daedalus::tester::printer_ptr &printer, const node_ptr &n);
        static void print_applied_action(const daedalus::tester::printer_ptr &printer, const kripke::action_ptr &a, const node_ptr &n_,
                                          strategy strategy);

        static void print_bound_too_large_action(const daedalus::tester::printer_ptr &printer, const kripke::action_ptr &a);
        static void print_not_applied_action(const daedalus::tester::printer_ptr &printer, const kripke::action_ptr &a);

        static void print_end_expanding_node(const daedalus::tester::printer_ptr &printer, const node_ptr &n, strategy strategy,
                                             bool is_dead_node, bool is_fully_expanded_node);
    };
}

#endif //DAEDALUS_PLANNER_H
