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

#include <memory>
#include <ostream>
#include <unordered_set>
#include <utility>
#include <iostream>
#include "../../include/search/planner.h"
#include "../../include/utils/time_utils.h"
#include "../../include/utils/printer/formula_printer.h"

using namespace search;

std::pair<node_deque, statistics>
planner::search(const planning_task &task, const strategy strategy, contraction_type contraction_type,
                const del::storages_ptr &storages, const daedalus::tester::printer_ptr &printer) {
    std::cout << "==================================================" << std::endl;
    std::cout << "                     DAEDALUS                     " << std::endl;
    std::cout << "==================================================" << std::endl << std::endl;

    std::cout << "------------------ CURRENT TASK ------------------" << std::endl;
    std::cout << " - DOMAIN:  " << task.get_domain_name() << std::endl;
    std::cout << " - PROBLEM: " << task.get_problem_id() << std::endl;
    std::cout << " - GOAL:    " << printer::formula_printer::to_string(*task.get_goal(), task.get_language(), false) << std::endl;
    std::cout << "--------------------------------------------------" << std::endl << std::endl;

    std::cout << "----------------- CONFIGURATION ------------------" << std::endl;

    switch (strategy) {
        case strategy::unbounded_search:
            std::cout << " - STRATEGY: Unbounded Search" << std::endl;
            break;
        case strategy::iterative_bounded_search:
            switch (contraction_type) {
                case contraction_type::full:
                    break;
                case contraction_type::rooted:
                    std::cout << " - STRATEGY: Iterative Bounded graph Search" << std::endl;
                    break;
                case contraction_type::canonical:
                    std::cout << " - STRATEGY: Iterative Bounded Graph Search" << std::endl;
                    break;
            }
    }

    std::cout << " - SEMANTICS: Kripke" << std::endl;
    std::cout << "--------------------------------------------------" << std::endl << std::endl;
    std::cout << "-------------------- SOLVING ---------------------" << std::endl;

//    std::pair<node_deque, statistics> result;
    node_deque path;
    statistics stats{};

    states_ids_set visited_states_ids;
    auto start = std::chrono::steady_clock::now();

    // If the initial state satisfies the goal, we immediately terminate
    if (task.get_initial_state()->satisfies(task.get_goal(), storages->l_storage)) {
        node_ptr n0 = init_node(contraction_type, task.get_initial_state(), nullptr, true, nullptr, 0, visited_states_ids, storages, task.get_goal()->get_modal_depth());
        if (printer) print_goal_found(printer, n0);
        return {extract_path(n0), statistics{1, n0->get_state()->get_worlds_number(), 0, 0, n0->get_bound()}};
    }

    switch (strategy) {
        case strategy::unbounded_search:
            path = unbounded_search(task, stats, visited_states_ids, storages, printer);
            break;
        case strategy::iterative_bounded_search:
            path = iterative_bounded_search(task, contraction_type, stats, visited_states_ids, storages, printer);
            break;
    }

    auto end = std::chrono::steady_clock::now();
    auto delta = since(start).count();

    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << "Computation time:       " << (delta / 1000) << "." << (delta % 1000) << "s." << std::endl;
    if (strategy == strategy::iterative_bounded_search)
        std::cout << "Bound:                  " << stats.m_bound                               << std::endl;
    std::cout << "Visited states:         " << stats.m_visited_states_no                       << std::endl;
    std::cout << "Total number of worlds: " << stats.m_visited_worlds_no                       << std::endl;
    std::cout << "Non revisited states:   " << stats.m_non_revisited_states_no                 << std::endl;
    std::cout << "Depth of search graph:  " << stats.m_graph_depth                             << std::endl;
    std::cout << "--------------------------------------------------" << std::endl << std::endl;

    return {std::move(path), stats};
}

node_deque planner::unbounded_search(const planning_task &task, statistics &stats, states_ids_set &visited_states_ids,
                                     const del::storages_ptr &storages, const daedalus::tester::printer_ptr &printer) {
    node_deque previous_iter_frontier = {};
    unsigned long long id = 0;

    return bfs(task, strategy::unbounded_search, contraction_type::full, stats, previous_iter_frontier, 0, id, visited_states_ids, storages, printer);
}

node_deque planner::iterative_bounded_search(const planning_task &task, contraction_type contraction_type, statistics &stats,
                                             states_ids_set &visited_states_ids, const del::storages_ptr &storages,
                                             const daedalus::tester::printer_ptr &printer) {
    unsigned long b = task.get_goal()->get_modal_depth();
    node_deque previous_iter_frontier;
    unsigned long long id = 0;

    while (true) {
        std::cout << "Searching with bound: " << b << " " << std::flush;
        // If iteration 'b' produces a valid result, we return it. Otherwise, we move to the next iteration
        if (node_deque result = bounded_search(task, contraction_type, stats, previous_iter_frontier, b++, id, visited_states_ids, storages, printer); not result.empty())
            return result;
        std::cout << "(no plan found, going to next iteration)" << std::endl;
    }
}

node_deque planner::bounded_search(const planning_task &task, contraction_type contraction_type, statistics &stats,
                                   node_deque &previous_iter_frontier, const unsigned long b, unsigned long long &id,
                                   states_ids_set &visited_states_ids, const del::storages_ptr &storages,
                                   const daedalus::tester::printer_ptr &printer) {
    return bfs(task, strategy::iterative_bounded_search, contraction_type, stats, previous_iter_frontier, b, id, visited_states_ids, storages, printer);
}

node_deque planner::bfs(const planning_task &task, const strategy strategy, contraction_type contraction_type, statistics &stats,
                        node_deque &previous_iter_frontier, const unsigned long b, unsigned long long &id,
                        states_ids_set &visited_states_ids, const del::storages_ptr &storages, const daedalus::tester::printer_ptr &printer) {
    kripke::state_ptr s0 = task.get_initial_state();
    node_deque frontier = init_frontier(s0, contraction_type, b, previous_iter_frontier, visited_states_ids, storages);

    unsigned long goal_depth = task.get_goal()->get_modal_depth();
    unsigned long long max_graph_depth = 0, is_bisim_graph_depth = 0;     // is_bisim_graph_depth: deepest level of the search graph such that all nodes in the previous levels have is_bisim = true
    statistics non_bisim_nodes_stats{};

    if (not previous_iter_frontier.empty())
        for (const node_ptr &n : previous_iter_frontier)
            if (n->get_graph_depth() > max_graph_depth)
                max_graph_depth = n->get_graph_depth();

    if (printer) print_max_graph_depth(printer, max_graph_depth);

    while (not frontier.empty()) {
        node_ptr n = frontier.front();

        if (previous_iter_frontier.empty())
            is_bisim_graph_depth = n->get_graph_depth();

        if (n->get_graph_depth() > max_graph_depth) {
            max_graph_depth = n->get_graph_depth();
            stats.m_graph_depth = max_graph_depth;
            if (printer) print_max_graph_depth(printer, max_graph_depth);
        }

        if (printer) print_begin_expanding_node(printer, n, strategy);
        // We expand n. We have two cases:
        //   1. If n is a node that has been partially expanded in the previous iteration (b-1), then we expand it wrt
        //      the actions that produced nodes n_ such that n_.is_bisim == false. These actions are stored in the
        //      attribute 'm_to_apply_actions' of the class 'node'.
        //   2. Otherwise, we reached n for the first time. We then expand it wrt the entire set of actions of our task.
        const kripke::action_deque &actions = n->get_to_apply_actions().empty() ? task.get_actions() : n->get_to_apply_actions();
        node_deque path = expand_node(task, strategy, contraction_type, stats, non_bisim_nodes_stats, n, actions,
                                      frontier, goal_depth, id, visited_states_ids, storages, printer);

        if (not path.empty()) return path;

        if (strategy != strategy::unbounded_search and not n->get_to_apply_actions().empty()    // If there exists a node n_ calculated above such that n_.is_bisim
            and n->get_graph_depth() == is_bisim_graph_depth)
            previous_iter_frontier.push_back(n);                                                // is false, then we need to expand it in the next iteration

        frontier.pop_front();
    }
    return {};
}

node_deque planner::init_frontier(kripke::state_ptr &s0, contraction_type contraction_type, const unsigned long b,
                                  node_deque &previous_iter_frontier, const states_ids_set &visited_states_ids,
                                  const del::storages_ptr &storages) {
    node_deque frontier;

    if (previous_iter_frontier.empty()) {   // If this is the first iteration
        node_ptr n0 = init_node(contraction_type, s0, nullptr, true, nullptr, 0, visited_states_ids, storages, b);
        if (n0)
            frontier.push_back(std::move(n0));
    } else {
        frontier = std::move(previous_iter_frontier);
        previous_iter_frontier.clear();

        for (node_ptr &n : frontier)    // The nodes in previous_iter_frontier have to be refreshed
            refresh_node(n, contraction_type, storages);
    }
    return frontier;
}

node_deque planner::expand_node(const planning_task &task, const strategy strategy, contraction_type contraction_type,
                                statistics &stats, statistics &non_bisim_nodes_stats, node_ptr &n,
                                const kripke::action_deque &actions, node_deque &frontier, const unsigned long goal_depth,
                                unsigned long long &id, states_ids_set &visited_states_ids,
                                const del::storages_ptr &storages, const daedalus::tester::printer_ptr &printer) {
    kripke::action_deque to_reapply_actions;
    bool is_dead_node = true;

    for (const kripke::action_ptr &a : actions) {
        if (strategy == strategy::iterative_bounded_search and a->get_maximum_depth() > n->get_bound())     // If the modal depth of 'a' is too big
            to_reapply_actions.push_back(a);
        else if (kripke::updater::is_applicable(*n->get_state(), *a, storages->l_storage)) {      // For all applicable actions. Let n_ be the result of updating node n with 'a'
            node_ptr n_ = update_node(strategy, contraction_type, n, a, id, visited_states_ids, storages, goal_depth);
            if (printer) print_applying_action(printer, a, n_, strategy);

            if (not n_) {
                if (strategy != strategy::unbounded_search)             // If the update is unsuccessful, we need to
                    to_reapply_actions.push_back(a);                    // reapply 'a' to n in the next iteration
            } else if (n_->is_already_visited())
                ++stats.m_non_revisited_states_no;
            else {                                                      // If the update is successful and n_'s state
                is_dead_node = false;                                   // was not previously visited, then n is not a
                n->add_child(n_);                                       // dead node and we add n_ to the children of n
                visited_states_ids.emplace(n_->get_state()->get_id());
                update_statistics(stats, non_bisim_nodes_stats, n_);

                // If n_'s state satisfies the goal, we return the path from the root of the search graph to n_
                if (n_->get_state()->satisfies(task.get_goal(), storages->l_storage)) {
                    if (printer) print_goal_found(printer, n_);
                    calculate_final_statistics(stats, non_bisim_nodes_stats, n_);
                    return extract_path(n_);
                }

                if (not n_->is_bisim())                                 // If n_.is_bisim is false, we need to
                    to_reapply_actions.push_back(a);                    // reapply 'a' to n in the next iteration

                frontier.push_back(std::move(n_));                      // We update the frontier
            }
        }
    }
    n->set_to_apply_action(std::move(to_reapply_actions));              // We set the actions that have to be reapplied
    if (printer) print_end_expanding_node(printer, n, strategy, is_dead_node, n->get_to_apply_actions().empty());

    return {};
}

void
planner::update_statistics(search::statistics &stats, search::statistics &non_bisim_nodes_stats, search::node_ptr &n) {
    if (n->is_already_visited())
        ++stats.m_non_revisited_states_no;

    if (n->is_bisim()) {
        ++stats.m_visited_states_no;
        stats.m_visited_worlds_no += n->get_state()->get_worlds_number();
    } else {
        ++non_bisim_nodes_stats.m_visited_states_no;
        non_bisim_nodes_stats.m_visited_worlds_no += n->get_state()->get_worlds_number();
    }
}

void planner::calculate_final_statistics(search::statistics &stats, search::statistics &non_bisim_nodes_stats, search::node_ptr &n) {
    stats.m_visited_states_no += non_bisim_nodes_stats.m_visited_states_no;
    stats.m_visited_worlds_no += non_bisim_nodes_stats.m_visited_worlds_no;
    stats.m_bound = n->get_bound();
}

node_deque planner::extract_path(node_ptr n) {
    node_deque path;
    do {
        path.push_front(n);
        n = n->get_parent();
    } while (n);
    print_plan(path);
    return path;
}

node_ptr planner::update_node(const strategy strategy, contraction_type contraction_type, const node_ptr &n, const kripke::action_ptr &a,
                              unsigned long long &id, const states_ids_set &visited_states_ids, const del::storages_ptr &storages, unsigned long goal_depth) {
    switch (strategy) {
        case strategy::unbounded_search: {
            kripke::state_ptr s_ = std::make_shared<kripke::state>(kripke::updater::product_update(*n->get_state(), *a, storages->l_storage));
            return init_node(contraction_type, s_, a, true, n, ++id, visited_states_ids, storages);
        } case strategy::iterative_bounded_search:
            if (n->is_bisim() or n->get_bound() - a->get_maximum_depth() >= goal_depth) {
                kripke::state_ptr s_ = std::make_shared<kripke::state>(kripke::updater::product_update(*n->get_state(), *a, storages->l_storage));

                if (n->is_bisim())
                    return init_node(contraction_type, s_, a, true, n, ++id, visited_states_ids, storages, n->get_bound());
                if (n->get_bound() - a->get_maximum_depth() >= goal_depth)
                    return init_node(contraction_type, s_, a, false, n, ++id, visited_states_ids, storages, n->get_bound() - a->get_maximum_depth());
            }
            return nullptr;
    }
}

void planner::refresh_node(node_ptr &n, contraction_type contraction_type, const del::storages_ptr &storages) {
    n->increment_bound();           // We increment the bound value. Moreover,  node n might have children n' such that
    n->clear_non_bisim_children();  // n'.is_bisim == false. We have to discard them before we move to the next iteration

    if (not n->is_bisim()) {                                        // If we can still do some refinement steps
        auto [is_bisim, s_contr] = kripke::bisimulator::contract(contraction_type, *n->get_original_state(), n->get_bound(), storages);    // We do another refinement step
        n->set_is_bisim(is_bisim);                                  // And we update the value of is_bisim
        n->set_state(std::make_shared<kripke::state>(std::move(s_contr)));
        if (is_bisim) n->set_original_state(nullptr);
    }
}

node_ptr planner::init_node(contraction_type contraction_type, const kripke::state_ptr &s, const kripke::action_ptr &a, bool was_bisim,
                            const node_ptr &parent, unsigned long long id, const states_ids_set &visited_states_ids,
                            const del::storages_ptr &storages, unsigned long b) {
    auto [is_bisim, s_contr] = kripke::bisimulator::contract(contraction_type, *s, b, storages);
    bool already_visited = visited_states_ids.find(s_contr.get_id()) != visited_states_ids.end();

    node_ptr n = std::make_shared<node>(id, std::make_shared<kripke::state>(std::move(s_contr)), a, b, is_bisim and was_bisim, already_visited, parent);

    if (not n->is_bisim()) n->set_original_state(s);
    return n;
}


// Print utilities
void planner::print_plan(const node_deque &path) {
    if (path.empty())
        std::cout << std::endl << std::endl << "No plan was found" << std::endl;
    else if (path.size() == 1)
        std::cout << "Goal found in initial state!" << std::endl;
    else {
        std::cout << std::endl << std::endl << "Found plan of length " << (path.size() - 1) << ":" << std::endl;
        unsigned long count = 0;

        for (const auto &node : path)
            if (node->get_action())
                std::cout << "  " << ++count << ". " << node->get_action()->get_name();
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void planner::print_max_graph_depth(const daedalus::tester::printer_ptr &printer, unsigned long long max_graph_depth) {
    printer->out() << "*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << std::endl;
    printer->out() << "                                      Search Graph Depth: " << max_graph_depth << "                                           " << std::endl;
    printer->out() << "*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << std::endl;
}

void planner::print_node_id(const daedalus::tester::printer_ptr &printer, const search::node_ptr &n) {
    printer->out() << "(" << n->get_graph_depth() << ", " << n->get_id() << ")";
}

void planner::print_begin_expanding_node(const daedalus::tester::printer_ptr &printer, const node_ptr &n, const strategy strategy) {
    printer->out() << "--------------------------------------------------" << std::endl;
    printer->out() << (n->get_to_apply_actions().empty() ? "~Expanding node: " : "~Resuming expansion of node: ");
    print_node_id(printer, n);
    printer->out() << "  -  ";

    if (n->get_parent()) {
        printer->out() << "Parent = ";
        print_node_id(printer, n->get_parent());
        printer->out() << "; Action = '" << n->get_action()->get_name() << "'";
    } else
        printer->out() << "Initial node " << (n->is_bisim() ? "(bisimilar)" : "(not bisimilar)");

    if (strategy != strategy::unbounded_search)
        printer->out() << "; Bound = " << n->get_bound() << "." << std::endl << std::endl;
    else
        printer->out() << "." << std::endl << std::endl;
}

void planner::print_goal_found(const daedalus::tester::printer_ptr &printer, const node_ptr &n) {
    printer->out() << "Goal found in node ";
    print_node_id(printer, n);
    printer->out() << "!" << std::endl;
}

void planner::print_applying_action(const daedalus::tester::printer_ptr &printer, const kripke::action_ptr &a, const node_ptr &n_,
                                    const strategy strategy) {
    printer->out() <<"\t~Applying action: " << a->get_name();

    if (n_) {
        printer->out() << (strategy != strategy::unbounded_search ? " (successful). " : ". ");
        printer->out() << "Generated child node ";   // << n_->get_graph_depth() << ", " << n_->get_id() << ")";
        print_node_id(printer, n_);
        printer->out() << (strategy != strategy::unbounded_search
            ? (n_->is_bisim() ? " (bisimilar)." : " (not bisimilar).")
            : ".") << std::endl;
    } else if (strategy != strategy::unbounded_search)
        printer->out() << " (unsuccessful)." << std::endl;
}

void planner::print_end_expanding_node(const daedalus::tester::printer_ptr &printer, const node_ptr &n,
                                       strategy strategy, bool is_dead_node, bool is_fully_expanded_node) {
    printer->out() << std::endl << "~Expanded node: ";
    print_node_id(printer, n);
    printer->out() << ".";

    if (strategy != strategy::unbounded_search)
        printer->out() << (is_dead_node
            ? " Dead node." : (is_fully_expanded_node
                ? " Node fully expanded."
                : " Node partially expanded."));

    printer->out() << std::endl << "--------------------------------------------------" << std::endl << std::endl;
}
