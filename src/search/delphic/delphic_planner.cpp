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

#include "../../../include/search/delphic/delphic_planner.h"

#include <memory>
#include <utility>
#include <iostream>
#include "../../../include/del/semantics/delphic/update/union_updater.h"
#include "../../../include/utils/time_utils.h"
#include "../../../include/utils/printer/formula_printer.h"

using namespace search;

delphic_node_deque delphic_planner::search(const delphic_planning_task &task, const strategy strategy, const daedalus::tester::printer_ptr &printer) {
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
            std::cout << " - STRATEGY: Iterative Bounded Search" << std::endl;
            break;
    }

    std::cout << " - SEMANTICS: DELPHIC" << std::endl;
    std::cout << "--------------------------------------------------" << std::endl << std::endl;
    std::cout << "-------------------- SOLVING ---------------------" << std::endl;

    delphic_node_deque path;
    auto start = std::chrono::steady_clock::now();

    // If the initial state satisfies the goal, we immediately terminate
    if (task.get_initial_state()->satisfies(task.get_goal())) {
        delphic_node_ptr n0 = init_node(strategy, task.get_initial_state(), nullptr, true, nullptr, 0, task.get_goal()->get_modal_depth());
        if (printer) print_goal_found(printer, n0);
        return extract_path(n0);
    }

    switch (strategy) {
        case strategy::unbounded_search:
            path = unbounded_search(task, printer);
            break;
        case strategy::iterative_bounded_search:
            path = iterative_bounded_search(task, printer);
            break;
    }

    auto end = std::chrono::steady_clock::now();
    auto delta = since(start).count();
    std::cout << "Plan found in " << (delta / 1000) << "." << (delta % 1000) << "s." << std::endl;
    std::cout << "--------------------------------------------------" << std::endl << std::endl;

    return path;
}

delphic_node_deque delphic_planner::unbounded_search(const delphic_planning_task &task, const daedalus::tester::printer_ptr &printer) {
    unsigned long long id = 0;

    return bfs(task, strategy::unbounded_search, 0, id, printer);
}

delphic_node_deque delphic_planner::iterative_bounded_search(const delphic_planning_task &task, const daedalus::tester::printer_ptr &printer) {
    std::cout << "Not implemented yet... " << std::endl;
   return {};
}

delphic_node_deque delphic_planner::bfs(const delphic_planning_task &task, const strategy strategy,
                        const unsigned long b, unsigned long long &id, const daedalus::tester::printer_ptr &printer) {
    delphic::possibility_spectrum_ptr s0 = task.get_initial_state();
    delphic_node_deque frontier;
    unsigned long goal_depth = task.get_goal()->get_modal_depth();
    unsigned long long max_tree_depth = 0;

    if (printer) print_max_tree_depth(printer, max_tree_depth);

    frontier.push_back(init_node(strategy, s0, nullptr, true, nullptr, 0, b));

    while (not frontier.empty()) {
        delphic_node_ptr n = frontier.front();

        if (n->get_tree_depth() > max_tree_depth) {
            max_tree_depth = n->get_tree_depth();
            if (printer) print_max_tree_depth(printer, max_tree_depth);
        }

        if (printer) print_begin_expanding_node(printer, n, strategy);
        delphic_node_deque path = expand_node(task, strategy, n, task.get_actions(), frontier, goal_depth, id, printer);

        if (not path.empty()) return path;
        frontier.pop_front();
    }
    return {};
}

delphic_node_deque delphic_planner::expand_node(const delphic_planning_task &task, const strategy strategy, delphic_node_ptr &n,
                                const delphic::action_deque &actions, delphic_node_deque &frontier, const unsigned long goal_depth,
                                unsigned long long &id, const daedalus::tester::printer_ptr &printer) {
    bool is_dead_node = true;

    for (const delphic::eventuality_spectrum_ptr &a : actions)
        if (delphic::union_updater::is_applicable(n->get_state(), a)) {      // For all applicable actions. Let n_ be the
            delphic_node_ptr n_ = update_node(strategy, n, a, id, goal_depth);  // result of updating node n with 'a'
            if (printer) print_applying_action(printer, a, n_, strategy);

            if (n_) {                                                   // If the update is successful
                is_dead_node = false;                                   // Then n is not a dead node
                n->add_child(n_);                                       // We add n_ to the children of n

                // If n_'s state satisfies the goal, we return the path from the root of the search tree to n_
                if (n_->get_state()->satisfies(task.get_goal())) {
                    if (printer) print_goal_found(printer, n_);
                    return extract_path(n_);
                }

                frontier.push_back(std::move(n_));                      // We update the frontier
            }
        }

    if (printer) print_end_expanding_node(printer, n, strategy, is_dead_node, true);

    return {};
}

delphic_node_deque delphic_planner::extract_path(delphic_node_ptr n) {
    delphic_node_deque path;
    do {
        path.push_front(n);
        n = n->get_parent();
    } while (n);
    print_plan(path);
    return path;
}

void delphic_planner::print_plan(const delphic_node_deque &path) {
    if (path.empty())
        std::cout << "No plan was found" << std::endl;
    else if (path.size() == 1)
        std::cout << "Goal found in initial state!" << std::endl;
    else {
        std::cout << "Found plan of length " << (path.size() - 1) << ":" << std::endl;
        unsigned long count = 0;

        for (const auto &node : path)
            if (node->get_action())
                std::cout << "  " << ++count << ". " << node->get_action()->get_name() << std::endl;
    }
    std::cout << std::endl;
}

delphic_node_ptr delphic_planner::update_node(const strategy strategy, const delphic_node_ptr &n, const delphic::eventuality_spectrum_ptr &a,
                              unsigned long long &id, unsigned long goal_depth) {
    delphic::possibility_spectrum_ptr s_ = delphic::union_updater::update(n->get_state(), a);
    return init_node(strategy, s_, a, true, n, ++id);
}

delphic_node_ptr delphic_planner::init_node(const strategy strategy, const delphic::possibility_spectrum_ptr &s, const delphic::eventuality_spectrum_ptr &a, bool was_bisim,
                            const delphic_node_ptr &parent, unsigned long long id, unsigned long b) {
    return std::make_shared<delphic_node>(id, s, a, parent);
}


// Print utilities
void delphic_planner::print_max_tree_depth(const daedalus::tester::printer_ptr &printer, unsigned long long max_tree_depth) {
    printer->out() << "*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << std::endl;
    printer->out() << "                                      Search Tree Depth: " << max_tree_depth << "                                           " << std::endl;
    printer->out() << "*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << std::endl;
}

void delphic_planner::print_node_id(const daedalus::tester::printer_ptr &printer, const search::delphic_node_ptr &n) {
    printer->out() << "(" << n->get_tree_depth() << ", " << n->get_id() << ")";
}

void delphic_planner::print_begin_expanding_node(const daedalus::tester::printer_ptr &printer, const delphic_node_ptr &n, const strategy strategy) {
    printer->out() << "--------------------------------------------------" << std::endl;
    printer->out() << "~Expanding node: ";
//    printer->out() << (n->get_to_apply_actions().empty() ? "~Expanding node: " : "~Resuming expansion of node: ");
    print_node_id(printer, n);
    printer->out() << "  -  ";

    if (n->get_parent()) {
        printer->out() << "Parent = ";
        print_node_id(printer, n->get_parent());
        printer->out() << "; Action = '" << n->get_action()->get_name() << "'";
    } else
        printer->out() << "Initial node"; // << (n->is_bisim() ? "(bisimilar)" : "(not bisimilar)");

    /*if (strategy != strategy::unbounded_search)
        printer->out() << "; Bound = " << n->get_bound() << "." << std::endl << std::endl;
    else*/
    printer->out() << "." << std::endl << std::endl;
}

void delphic_planner::print_goal_found(const daedalus::tester::printer_ptr &printer, const delphic_node_ptr &n) {
    printer->out() << "Goal found in node ";
    print_node_id(printer, n);
    printer->out() << "!" << std::endl;
}

void delphic_planner::print_applying_action(const daedalus::tester::printer_ptr &printer, const delphic::eventuality_spectrum_ptr &a, const delphic_node_ptr &n_,
                                    const strategy strategy) {
    printer->out() <<"\t~Applying action: " << a->get_name();

    if (n_) {
        printer->out() << (strategy != strategy::unbounded_search ? " (successful). " : ". ");
        printer->out() << "Generated child node ";   // << n_->get_tree_depth() << ", " << n_->get_id() << ")";
        print_node_id(printer, n_);
        /*printer->out() << (strategy != strategy::unbounded_search
                           ? (n_->is_bisim() ? " (bisimilar)." : " (not bisimilar).")
                           : ".") << std::endl;*/
    } else if (strategy != strategy::unbounded_search)
        printer->out() << " (unsuccessful)." << std::endl;
}

void delphic_planner::print_end_expanding_node(const daedalus::tester::printer_ptr &printer, const delphic_node_ptr &n,
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
