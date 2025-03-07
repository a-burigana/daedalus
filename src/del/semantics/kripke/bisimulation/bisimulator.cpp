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

#include "../../../../../include/del/semantics/kripke/bisimulation/bisimulator.h"
#include "../../../../../include/del/semantics/kripke/bisimulation/partition_refinement.h"
#include "../../../../../include/del/semantics/kripke/bisimulation/bounded_contraction_builder.h"

using namespace kripke;

std::pair<bool, state> bisimulator::contract(contraction_type type, state &s, unsigned long k, const del::storages_ptr &storages) {
    switch (type) {
        case contraction_type::full:
            return bounded_contraction_builder::calculate_rooted_contraction(s, s.get_max_depth() + 1, true, storages);           // Classic Paige and Tarjan algorithm
        case contraction_type::rooted:  // todo: use min(s.get_depth, k)
            return bounded_contraction_builder::calculate_rooted_contraction(s, k);
        case contraction_type::canonical:
            return bounded_contraction_builder::calculate_rooted_contraction(s, k, true, storages);
    }
}

//bool bisimulator::contract(bisimulation_type type, search::node_ptr &n) {
//    switch (type) {
//        case bisimulation_type::full:
//            return partition_refinement::contract(n);           // Classic Paige and Tarjan algorithm
//        case bisimulation_type::bounded:
//            return bounded_partition_refinement::contract(n);
//    }
//}

//bool bisimulator::repeat_contraction(search::node_ptr &n) {
//    return bounded_partition_refinement::repeat_contraction(n);
//}
