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

#ifndef DAEDALUS_PRINTER_H
#define DAEDALUS_PRINTER_H

#include <fstream>
#include "../include/del/semantics/kripke/states/state.h"
#include "../include/del/semantics/kripke/actions/action.h"
#include "../include/del/semantics/kripke/bisimulation/bisimulation_types.h"
#include "../include/search/planning_task.h"
#include "../include/search/strategies.h"
#include "../include/search/delphic/delphic_planning_task.h"
#include "../include/del/semantics/kripke/bisimulation/bounded_bisimulation_types.h"
//#include "../include/search/planner.h"

using namespace kripke;
using namespace delphic;

namespace daedalus::tester {
    class printer;
    using printer_ptr = std::unique_ptr<printer>;

    class printer {
    public:
        printer(bool out_to_file = false, const std::string &path = "", const std::string &name = "");

        printer(const printer &) = delete;
        printer &operator=(const printer &) = delete;

        printer(printer &&) = default;
        printer &operator=(printer &&) = default;

        ~printer() = default;

        [[nodiscard]] std::ostream &out();
        void set_out_to_file(bool out_to_file);

        static void to_string(std::ofstream &os, const state &s, const del::label_storage_ptr &l_storage);

        static void print_state(const state &s, const del::label_storage_ptr &l_storages, const std::string &path, const std::string &name);
        static void print_state(const delphic::possibility_spectrum &W, const std::string &path, const std::string &name);

        static void print_action(const action &a, const std::string &path);

        static state_deque print_states(state_deque &ss, kripke::action_deque &as, const del::storages_ptr &storages,
                                        const del::formula_ptr &goal, const std::string &path, const std::string &name,
                                        bool apply_contraction = false, contraction_type type = contraction_type::full,
                                        unsigned long b = 0);

        static void print_states(const possibility_spectrum_ptr &W, const delphic::action_deque &as,
                                 const del::formula_ptr &goal, const std::string &path, const std::string &name);

        static void print_states(const search::planning_task &task, const kripke::action_deque &as, const std::string &path,
                                 bool apply_contraction = false, contraction_type type = contraction_type::full,
                                 unsigned long k = 0);

        static void print_states(const search::delphic_planning_task &task, const delphic::action_deque &as, const std::string &path);

        static void print_task(const search::planning_task &task, const del::storages_ptr &storages, const std::string &path);

        static void print_results(const search::planning_task &task, search::strategy strategy, contraction_type contraction_type, const del::storages_ptr &storages, const std::string &out_path);
        static void print_delphic_results(const search::delphic_planning_task &task, search::strategy strategy, const std::string &out_path);

        static void print_domain_info(const search::planning_task &task, std::ofstream &table);
        static void print_time_results(const search::planning_task &task, search::strategy strategy, contraction_type contraction_type, const del::storages_ptr &storages, std::ofstream &table);
        static void print_delphic_time_results(const search::delphic_planning_task &task, search::strategy strategy, std::ofstream &table);

    private:
        std::ofstream m_output_stream;
        bool m_out_to_file;
    };
}

#endif //DAEDALUS_PRINTER_H
