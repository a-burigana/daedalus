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

#include "../../../include/del/bisimulation/bisimulator.h"
#include "../../../include/del/bisimulation/partition_refinement.h"
#include "../../../include/del/bisimulation/bounded_partition_refinement.h"

using namespace del;

std::pair<bool, state> bisimulator::contract(del::bisimulation_type type, del::state &s, unsigned long k) {
    switch (type) {
        case bisimulation_type::full:
            return partition_refinement::contract(s);           // Classic Paige and Tarjan algorithm
        case bisimulation_type::bounded:
            return bounded_partition_refinement::contract(s, k);
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
