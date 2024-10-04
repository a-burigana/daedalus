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

#ifndef DAEDALUS_DELPHIC_UTILS_H
#define DAEDALUS_DELPHIC_UTILS_H

#include "states/possibility_spectrum.h"
#include "actions/eventuality_spectrum.h"
#include "../kripke/states/state.h"
#include "../kripke/actions/action.h"
#include "../../../search/planning_task.h"
#include "../../../search/delphic/delphic_planning_task.h"

namespace delphic {
    class delphic_utils {
    public:
        static possibility_spectrum convert(const kripke::state  &s);
        static eventuality_spectrum convert(const kripke::action &a);

        static kripke::state  convert(const possibility_spectrum &W);
        static kripke::action convert(const eventuality_spectrum &E);

        static search::delphic_planning_task convert(const search::planning_task &task);
    };
}

#endif //DAEDALUS_DELPHIC_UTILS_H
