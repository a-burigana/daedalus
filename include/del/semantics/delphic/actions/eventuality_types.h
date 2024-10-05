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


#ifndef DAEDALUS_EVENTUALITY_TYPES_H
#define DAEDALUS_EVENTUALITY_TYPES_H

#include <cstdint>
#include <memory>
#include <unordered_set>
#include <unordered_map>
#include "../../../language/language_types.h"
#include "../../../formulas/formula.h"
#include "../../kripke/actions/actions_types.h"

namespace delphic {
    class eventuality;
    class eventuality_spectrum;
    using eventuality_ptr = std::shared_ptr<eventuality>;
    using eventuality_spectrum_ptr = std::shared_ptr<eventuality_spectrum>;

    using dynamic_information_state = std::unordered_set<eventuality_ptr>;
    using agents_dynamic_information_state = std::vector<dynamic_information_state>;

    using postconditions = std::unordered_map<del::atom, del::formula_ptr>;

    using action_deque = std::deque<eventuality_spectrum_ptr>;
    using action_type = kripke::action_type;
}

#endif //DAEDALUS_EVENTUALITY_TYPES_H
