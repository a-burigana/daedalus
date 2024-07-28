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

#ifndef BEP_PRINTER_H
#define BEP_PRINTER_H

#include <fstream>
#include "../include/del/states/state.h"
#include "../include/del/actions/action.h"
#include "../include/del/bisimulation/bisimulation_types.h"
#include "../include/search/planning_task.h"
#include "../include/search/strategies.h"
//#include "../include/search/planner.h"

namespace bep::tester {
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

        static void print_state(const del::state &s, const std::string &path, const std::string &name);
        static void print_action(const del::action &a, const std::string &path);

        static void print_states(const del::state &s, const del::action_deque &as, const std::string &path,
                                 const std::string &name, bool apply_contraction = false,
                                 del::bisimulation_type type = del::bisimulation_type::full, unsigned long k = 0);

        static void print_results(const search::planning_task &task, search::strategy strategy, const std::string &out_path);
        static void print_time_results(const search::planning_task &task, std::ofstream &table);

    private:
        std::ofstream m_output_stream;
        bool m_out_to_file;
    };
}

#endif //BEP_PRINTER_H
