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

#ifndef DAEDALUS_BISIMULATOR_H
#define DAEDALUS_BISIMULATOR_H

#include <utility>
#include "../../../language/language.h"
#include "bounded_bisimulation_types.h"
#include "../../../../utils/storages_handler.h"
#include "../states/state.h"
#include "bisimulation_types.h"
#include "../../../../search/search_types.h"

namespace kripke {
    class bisimulator {
    public:
        static std::pair<bool, state> contract(contraction_type type, const state &s, unsigned long k,
                                               del::storages_handler_ptr handler = nullptr);

        static bool are_bisimilar(const state &s, const state &t, unsigned long k, del::storages_handler_ptr handler);

        /*static std::tuple<bool, state, bpr_structures> resume_contraction(contraction_type type, const state &s, unsigned long k,
                                                         bpr_structures &structures, del::storages_handler_ptr handler = nullptr);*/

    private:
        static state disjoint_union(const state &s, const state &t);
    };
}

#endif //DAEDALUS_BISIMULATOR_H
