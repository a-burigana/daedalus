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

#include <chrono>
#include <memory>
#include <ostream>
#include <unordered_set>
#include <utility>
#include <iostream>
#include <variant>
#include "../../include/search/planner.h"
#include "../../include/utils/time_utils.h"

using namespace search;

std::pair<node_deque, statistics>
planner::search(const planning_task &task, const strategy strategy, contraction_type contraction_type,
                const del::storages_ptr &storages, const daedalus::tester::printer_ptr &printer) {
    print_info(task, strategy, contraction_type);
    node_deque path;
    visited_states visited_states;
    statistics stats{};

    if (contraction_type != kripke::contraction_type::rooted) visited_states = states_ids_set();
    else visited_states = kripke::state_set();

    auto start = std::chrono::high_resolution_clock::now();

    // If the initial state satisfies the goal, we immediately terminate
    if (task.get_initial_state()->satisfies(task.get_goal(), storages->l_storage)) {
        node_ptr n0 = init_node(contraction_type, task.get_initial_state(), nullptr, true, nullptr, 0,
                                visited_states, storages, task.get_goal()->get_modal_depth());
        if (printer) print_goal_found(printer, n0);
        path = extract_path(n0, stats);
        stats = statistics{0, 0, 1, n0->get_state()->get_worlds_number(), 0, 0, 0, 0};
    } else
        path = strategy == strategy::unbounded_search ?
               unbounded_search(task, stats, visited_states, storages, printer) :
               iterative_bounded_search(task, strategy, contraction_type, stats, visited_states, storages, printer);

    stats.m_plan_length = path.size() - 1;
    stats.m_computation_time = static_cast<double>(since(start).count()) / 1000;

    validate(task, path, storages);
    print_statistics(stats, strategy);

    return {std::move(path), stats};
}

node_deque planner::unbounded_search(const planning_task &task, statistics &stats, visited_states &visited_states,
                                     const del::storages_ptr &storages, const daedalus::tester::printer_ptr &printer) {
    frontier previous_iter_frontier;
    unsigned long long id = 0;
    auto path = bfs(task, strategy::unbounded_search, contraction_type::full, stats, previous_iter_frontier, 0, id,
                    visited_states, storages, printer);

//    for (const auto &n: path)
//        if (n->get_action()) stats.m_plan_bound += n->get_action()->get_maximum_depth();

    return std::move(path);
}

node_deque
planner::iterative_bounded_search(const planning_task &task, const strategy strategy, contraction_type contraction_type,
                                  statistics &stats, visited_states &visited_states, const del::storages_ptr &storages,
                                  const daedalus::tester::printer_ptr &printer) {
    unsigned long b = task.get_goal()->get_modal_depth(), iterations = 0;
    frontier previous_iter_frontier;
    unsigned long long id = 0;

    while (true) {
        std::cout << "Searching with bound: " << b << " " << std::flush;
        stats.m_iterations_no = iterations++;
        // If iteration 'b' produces a valid result, we return it. Otherwise, we move to the next iteration
        if (node_deque result = bounded_search(task, strategy, contraction_type, stats, previous_iter_frontier, b++, id,
                                               visited_states, storages, printer); not result.empty())
            return result;
        std::cout << "- No plan found, going to next iteration." << std::endl;
    }
}

node_deque
planner::bounded_search(const planning_task &task, const strategy strategy, contraction_type contraction_type,
                        statistics &stats, frontier &previous_iter_frontier, const unsigned long b,
                        unsigned long long &id,
                        visited_states &visited_states, const del::storages_ptr &storages,
                        const daedalus::tester::printer_ptr &printer) {
    return bfs(task, strategy, contraction_type, stats, previous_iter_frontier, b, id, visited_states, storages, printer);
}

node_deque
planner::bfs(const planning_task &task, const strategy strategy, contraction_type contraction_type, statistics &stats,
             frontier &previous_iter_frontier, const unsigned long b, unsigned long long &id,
             visited_states &visited_states, const del::storages_ptr &storages,
             const daedalus::tester::printer_ptr &printer) {
    kripke::state_ptr s0 = task.get_initial_state();
    frontier frontier = init_frontier(s0, strategy, contraction_type, b, previous_iter_frontier, stats, visited_states, storages);

    if (strategy == strategy::approx_iterative_bounded_search) {
        if (contraction_type != kripke::contraction_type::rooted) visited_states = states_ids_set();
        else visited_states = kripke::state_set();
    }

    unsigned long goal_depth = task.get_goal()->get_modal_depth();
    unsigned long long max_graph_depth = 0, is_bisim_graph_depth = 0;     // is_bisim_graph_depth: deepest level of the search graph such that all nodes in the previous levels have is_bisim = true

//    if (not previous_iter_frontier.empty())
    for (auto &[_, deq] : frontier)
        for (const node_ptr &n: deq)   // previous_iter_frontier
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
        //      member 'm_to_apply_actions' of the class 'node'.
        //   2. Otherwise, we reached n for the first time. We then expand it wrt the entire set of actions of our task.
        const auto &actions = n->get_to_apply_actions().empty() ? task.get_actions() : n->get_to_apply_actions();
        node_deque path = expand_node(task, strategy, contraction_type, stats, n, actions, frontier, goal_depth, id,
                                      visited_states, storages, printer);

        if (not path.empty()) return path;

        if (strategy == strategy::iterative_bounded_search and
            not n->get_to_apply_actions().empty())    // If there exists a node n_ calculated above such that n_.is_bisim
//            and n->get_graph_depth() == is_bisim_graph_depth)
            previous_iter_frontier.push(n);     // is false, then we need to expand it in the next iteration

        frontier.pop_front();
    }
    return {};
}

frontier planner::init_frontier(kripke::state_ptr &s0, const strategy strategy, contraction_type contraction_type,
                                  const unsigned long b, frontier &previous_iter_frontier, statistics &stats,
                                  visited_states &visited_states, const del::storages_ptr &storages) {
    frontier frontier;
    bool fresh_frontier = previous_iter_frontier.empty() or strategy == strategy::approx_iterative_bounded_search;

    if (fresh_frontier) {       // If this is the first iteration or if we are using approximated search
        node_ptr n0 = init_node(contraction_type, s0, nullptr, true, nullptr, 0, visited_states, storages, b);

        if (n0) {
            update_visited_states(n0->get_state(), visited_states);
            update_statistics(stats, n0);
            frontier.push(n0);
        }
    } else {
        frontier = std::move(previous_iter_frontier);
        previous_iter_frontier = {};

        for (auto &[_, deq] : frontier.get())
            for (node_ptr &n: deq)    // The nodes in previous_iter_frontier have to be refreshed
                refresh_node(n, contraction_type, stats, visited_states, storages);
    }
    return frontier;
}

node_deque planner::expand_node(const planning_task &task, const strategy strategy, contraction_type contraction_type,
                                statistics &stats, node_ptr &n, const kripke::action_deque &actions,
                                frontier &frontier, const unsigned long goal_depth, unsigned long long &id,
                                visited_states &visited_states, const del::storages_ptr &storages,
                                const daedalus::tester::printer_ptr &printer) {
    kripke::action_deque to_reapply_actions;
    bool is_dead_node = true;

    if (n->get_graph_depth() == 2 and n->get_id() == 44)
        int x;

    for (const kripke::action_ptr &a: actions) {
        if (strategy == strategy::unbounded_search or n->get_bound() >= a->get_maximum_depth() + task.get_goal()->get_modal_depth()) {
            if (kripke::updater::is_applicable(*n->get_state(), *a, storages->l_storage)) {
                node_ptr n_ = update_node(strategy, contraction_type, n, a, id, visited_states, storages, goal_depth);
                is_dead_node = false;

                if (printer) print_applied_action(printer, a, n_, strategy);
                update_statistics(stats, n_);

                if (not n_->is_already_visited()) {     // If the update is successful and n_'s state was not
                    n->add_child(n_);                   // previously visited, we add n_ to the children of n
                    update_visited_states(n_->get_state(), visited_states);

                    // If n_'s state satisfies the goal, we return the path from the root of the search tree to n_
                    if (n_->get_state()->satisfies(task.get_goal(), storages->l_storage)) {
                        if (printer) print_goal_found(printer, n_);
                        return extract_path(n_, stats);
                    }

                    if (strategy == strategy::iterative_bounded_search and not n_->is_bisim())  // If n_.is_bisim is false, we need to
                        to_reapply_actions.push_back(a);            // reapply 'a' to n in the next iteration

                    frontier.push(n_);              // We update the frontier
                }
            } else if (printer) print_not_applied_action(printer, a);
        } else {
            if (strategy == strategy::iterative_bounded_search) to_reapply_actions.push_back(a);
            if (printer) print_bound_too_large_action(printer, a);
        }
    }

    n->set_to_apply_action(std::move(to_reapply_actions));          // We set the actions that have to be reapplied
    if (printer) print_end_expanding_node(printer, n, strategy, is_dead_node, n->get_to_apply_actions().empty());

    return {};
}

void planner::update_statistics(search::statistics &stats, search::node_ptr &n) {
    if (n->is_already_visited())
        ++stats.m_non_revisited_states_no;
    else {
        ++stats.m_visited_states_no;
        stats.m_visited_worlds_no += n->get_state()->get_worlds_number();
    }
}

node_deque planner::extract_path(node_ptr n, statistics &stats) {
    node_deque path;
    do {
        if (n->get_action())
            stats.m_plan_bound += n->get_action()->get_maximum_depth();
        path.push_front(n);
        n = n->get_parent();
    } while (n);
    print_plan(path);
    return path;
}

bool planner::validate(const search::planning_task &task, const search::node_deque &path,
                       const del::storages_ptr &storages) {
    kripke::state_ptr s = task.get_initial_state();
    bool valid = true;
    auto n = path.begin();
    unsigned long count = 0;

    if (path.size() > 1)
        do {
            ++n;
            ++count;
            valid = updater::is_applicable(*s, *(*n)->get_action(), storages->l_storage);

            if (valid)
                s = std::make_shared<kripke::state>(
                        updater::product_update(*s, *(*n)->get_action(), storages->l_storage));
            else
                std::cout << "\n\nWARNING! Action '" << count << ". " << (*n)->get_action()->get_name()
                          << "' is not applicable.\n\n";
        } while (valid and count < path.size() - 1);

    if (valid) {
        valid = s->satisfies(task.get_goal(), storages->l_storage);

        if (not valid)
            std::cout << "\n\nWARNING! Action sequence is NOT a solution.\n\n";
    }

    return valid;
}

node_ptr planner::update_node(const strategy strategy, contraction_type contraction_type, const node_ptr &n,
                              const kripke::action_ptr &a,
                              unsigned long long &id, const visited_states &visited_states,
                              const del::storages_ptr &storages, unsigned long goal_depth) {
    kripke::state_ptr s_ = std::make_shared<kripke::state>(
            kripke::updater::product_update(*n->get_state(), *a, storages->l_storage));

    if (strategy == strategy::unbounded_search)
        return init_node(contraction_type, s_, a, true, n, ++id, visited_states, storages);
    else if (strategy == strategy::approx_iterative_bounded_search)
        return init_node(contraction_type, s_, a, false, n, ++id, visited_states, storages,
                         n->get_bound() - a->get_maximum_depth());
    else {
        assert(n->is_bisim() or n->get_bound() >= a->get_maximum_depth() + goal_depth);
        return n->is_bisim() ?
               init_node(contraction_type, s_, a, true, n, ++id, visited_states, storages, n->get_bound()) :
               init_node(contraction_type, s_, a, false, n, ++id, visited_states, storages,
                         n->get_bound() - a->get_maximum_depth());
    }
//        if (n->is_bisim() or n->get_bound() >= a->get_maximum_depth() + goal_depth) {
//            kripke::state_ptr s_ = std::make_shared<kripke::state>(kripke::updater::product_update(*n->get_state(), *a, storages->l_storage));
    // if (n->get_bound() >= a->get_maximum_depth() + goal_depth)
//        }
//        return nullptr;
}

void planner::refresh_node(node_ptr &n, contraction_type contraction_type, statistics &stats,
                           visited_states &visited_states, const del::storages_ptr &storages) {
    n->increment_bound();           // We increment the bound value. Moreover,  node n might have children n' such that
    n->clear_non_bisim_children();  // n'.is_bisim == false. We have to discard them before we move to the next iteration

    if (not n->is_bisim()) {                                        // If we can still do some refinement steps
        stats.m_visited_worlds_no -= n->get_state()->get_worlds_number();

//        auto [is_bisim, s_contr, structures] = kripke::bisimulator::resume_contraction(contraction_type, *n->get_original_state(), n->get_bound(), n->get_bpr_structures(), storages);    // We do another refinement step
        auto [is_bisim, s_contr] = kripke::bisimulator::contract(contraction_type, *n->get_original_state(), n->get_bound(), storages);    // We do another refinement step

        n->set_is_bisim(is_bisim);                                  // And we update the value of is_bisim
        n->set_state(std::make_shared<kripke::state>(std::move(s_contr)));
        update_visited_states(n->get_state(), visited_states);
        update_statistics(stats, n);

        if (is_bisim) n->clear_original_state();
//            n->clear_bpr_structures();
    }
}

node_ptr planner::init_node(contraction_type contraction_type, const state_ptr &s, const action_ptr &a, bool was_bisim,
                            const node_ptr &parent, unsigned long long id, const visited_states &visited_states,
                            const del::storages_ptr &storages, unsigned long b) {
    auto [is_bisim, s_contr] = kripke::bisimulator::contract(contraction_type, *s, b, storages);
    bool already_visited = is_already_visited(s_contr, b, visited_states, storages);

    node_ptr n = std::make_shared<node>(id, std::make_shared<kripke::state>(std::move(s_contr)), a, b,
                                        is_bisim and was_bisim, already_visited, parent);

    if (not n->is_bisim()) n->set_original_state(s);
//        n->set_bpr_structures(std::move(structures));

    return n;
}

void planner::update_visited_states(const kripke::state_ptr &s, visited_states &visited_states) {
    std::visit([&](auto &&arg) {
        using arg_type = std::remove_reference_t<decltype(arg)>;

        if constexpr (std::is_same_v<arg_type, states_ids_set>) arg.emplace(s->get_id());
        else if constexpr (std::is_same_v<arg_type, kripke::state_set>) arg.emplace(s);
    }, visited_states);
}

bool planner::is_already_visited(const kripke::state &s, unsigned long b, const visited_states &visited_states, const del::storages_ptr &storages) {
    return std::visit([&](auto &&arg) -> bool {
        using arg_type = std::remove_reference_t<decltype(arg)>;

        if constexpr (std::is_same_v<arg_type, const states_ids_set>)
            return arg.find(s.get_id()) != arg.end();
        else if constexpr (std::is_same_v<arg_type, const kripke::state_set>)
            return std::any_of(arg.begin(), arg.end(),
                       [&](const state_ptr &t) {
                           return bisimulator::are_bisimilar(s, *t, b, storages);
                       });
        else return false;
    }, visited_states);
}


// Print utilities
void planner::print_info(const search::planning_task &task, search::strategy strategy,
                         kripke::contraction_type contraction_type) {
//    std::string strategy_str =
//            strategy == strategy::unbounded_search ? "BFS" :
//            (strategy == strategy::iterative_bounded_search ? "IBDS" : "Approx-IBDS");
//
//    std::string contraction_str =
//            contraction_type == contraction_type::full ? "Standard" :
//            (contraction_type == contraction_type::rooted ? "Rooted" : "Canonical");

    std::cout << "DAEDALUS" << std::endl;

//    std::cout << "Domain: " << task.get_domain_name()
//              << "   Problem: " << task.get_problem_id()
//              << "   Goal: " << printer::formula_printer::to_string(*task.get_goal(), task.get_language(), false)
//              << "" << std::endl;
//
//    std::cout
//        << "Strategy: " << strategy_str
//        << "   Contraction type: " << contraction_str
//        << "" << std::endl << std::endl;

    std::cout << "Solving..." << std::endl;
}

void planner::print_statistics(search::statistics &stats, search::strategy strategy) {
    std::cout << "--------------------------------------------------"          << std::endl;
    std::cout << "Computation time:       " << stats.m_computation_time        << "s." << std::endl;
    if (strategy != strategy::unbounded_search) {
        std::cout << "Iterations:             " << stats.m_iterations_no << std::endl;
        std::cout << "Reasoning depth:        " << stats.m_plan_bound << std::endl;
    } else
        std::cout << "Bound:                  " << "\u221E"                    << std::endl;
    std::cout << "Visited states:         " << stats.m_visited_states_no       << std::endl;
    std::cout << "Total number of worlds: " << stats.m_visited_worlds_no       << std::endl;
    std::cout << "Non revisited states:   " << stats.m_non_revisited_states_no << std::endl;
//    std::cout << "Depth of search graph:  " << stats.m_graph_depth             << std::endl;
    std::cout << "--------------------------------------------------";
}

void planner::print_plan(const node_deque &path) {
    if (path.empty())
        std::cout << std::endl << std::endl << "No plan was found" << std::endl;
    else if (path.size() == 1)
        std::cout << "Goal found in initial state!" << std::endl;
    else {
        std::cout << "- Found plan of length " << (path.size() - 1) << "!" << std::endl << std::endl;
        unsigned long count = 0;

        for (const auto &node : path)
            if (node->get_action())
                std::cout << ++count << ". " << node->get_action()->get_name() << "  ";
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

void planner::print_applied_action(const daedalus::tester::printer_ptr &printer, const kripke::action_ptr &a, const node_ptr &n_,
                                    const strategy strategy) {
    printer->out() <<"\t~Applying action: " << a->get_name();

    if (n_) {
        printer->out() << (strategy != strategy::unbounded_search ? " (successful). " : ". ");
        printer->out() << "Generated child node ";   // << n_->get_graph_depth() << ", " << n_->get_id() << ")";
        print_node_id(printer, n_);
        printer->out() << (strategy != strategy::unbounded_search
            ? (n_->is_already_visited() ? " (already visited)." : (n_->is_bisim() ? " (bisimilar)." : " (not bisimilar)."))
            : ".") << std::endl;
    } else if (strategy != strategy::unbounded_search)
        printer->out() << " (unsuccessful)." << std::endl;
}

void planner::print_bound_too_large_action(const daedalus::tester::printer_ptr &printer, const kripke::action_ptr &a) {
    printer->out() <<"\t~Action '" << a->get_name() << "' not applied: bound is too large" << std::endl;
}

void planner::print_not_applied_action(const daedalus::tester::printer_ptr &printer, const kripke::action_ptr &a) {
    printer->out() <<"\t~Action '" << a->get_name() << "' is not applicable" << std::endl;
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
