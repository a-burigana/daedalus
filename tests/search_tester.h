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

#ifndef DAEDALUS_SEARCH_TESTER_H
#define DAEDALUS_SEARCH_TESTER_H

#include "../include/search/planning_task.h"
#include "../include/search/strategies.h"
#include "../include/del/semantics/kripke/bisimulation/bisimulation_types.h"

namespace daedalus::tester {
    class search_tester {
    public:
        static void run_formulas_tests(const del::label_storage_ptr &l_storage);
        static void run_actions_tests();
        static void run_product_update_tests(const del::label_storage_ptr &l_storage);
        static void run_contractions_tests(const del::storages_ptr &storage);

        static void run_coin_in_the_box_search_tests(const del::storages_ptr &storages);
        static void run_consecutive_numbers_search_tests(const del::storages_ptr &storages);
        static void run_selective_communication_search_tests(const del::storages_ptr &storages);
        static void run_collaboration_communication_search_tests(const del::storages_ptr &storages);

        static void print_coin_in_the_box_search_tests(const del::storages_ptr &storages);
        static void print_consecutive_numbers_search_tests(const del::storages_ptr &storages);
        static void print_selective_communication_search_tests(const del::storages_ptr &storages);
        static void print_collaboration_communication_search_tests(const del::storages_ptr &storages);

        static void run_search_tests(const std::vector<search::planning_task> &tasks, const del::storages_ptr &storages);
        static void print_search_results(const std::vector<search::planning_task> &tasks, const del::storages_ptr &storages);
        static void print_time_results(const std::vector<search::planning_task> &tasks, search::strategy strategy,
                                       kripke::contraction_type contraction_type, const del::storages_ptr &storages,
                                       const std::string &table_path);

//        static void print_coin_in_the_box_time_results();
//        static void print_consecutive_numbers_time_results();
//        static void print_switches_time_results();
//        static void print_selective_communication_time_results();
//        static void print_collaboration_communication_time_results();
    };
}

#endif //DAEDALUS_SEARCH_TESTER_H
