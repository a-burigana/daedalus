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

#ifndef DAEDALUS_EVENTUALITY_H
#define DAEDALUS_EVENTUALITY_H

#include <memory>
#include <set>
#include <vector>
#include "eventuality_types.h"
#include "../../kripke/states/label.h"
#include "../../../formulas/formula.h"

namespace delphic {


    class eventuality {
    public:
        eventuality(del::formula_ptr pre, postconditions &post, agents_dynamic_information_state state);

        eventuality(const eventuality&) = default;
        eventuality& operator=(const eventuality&) = default;

        eventuality(eventuality&&) = default;
        eventuality& operator=(eventuality&&) = default;

        ~eventuality() = default;

        [[nodiscard]] del::formula_ptr get_pre() const;
        [[nodiscard]] postconditions get_post() const;
        [[nodiscard]] const dynamic_information_state &get_information_state(del::agent ag) const;
        [[nodiscard]] bool is_ontic() const;
        [[nodiscard]] bool is_idle() const;

    private:
        del::formula_ptr m_pre;
        postconditions m_post;
        agents_dynamic_information_state m_information_state;
        bool m_is_ontic;
        bool m_is_idle;
    };
}

#endif //DAEDALUS_EVENTUALITY_H
