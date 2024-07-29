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
#include "../../kripke/states/label.h"
#include "../../../formulas/formula.h"

namespace delphic {
    class eventuality;
    using eventuality_ptr = std::shared_ptr<eventuality>;
    using postconditions = std::map<del::atom, del::formula_ptr>;

    class eventuality {
    public:
        using agent_information_state = std::set<eventuality_ptr>;
        using information_state = std::vector<agent_information_state>;

        eventuality(del::formula_ptr pre, postconditions &post, information_state state);

        eventuality(const eventuality&) = default;
        eventuality& operator=(const eventuality&) = default;

        eventuality(eventuality&&) = default;
        eventuality& operator=(eventuality&&) = default;

        ~eventuality() = default;

        [[nodiscard]] del::formula_ptr get_pre() const;
        [[nodiscard]] postconditions get_post() const;
        [[nodiscard]] const agent_information_state &get_information_state(del::agent ag) const;
        [[nodiscard]] bool is_ontic() const;

    private:
        del::formula_ptr m_pre;
        postconditions m_post;
        information_state m_information_state;
        bool m_is_ontic;
    };

    using eventuality_spectrum = eventuality::agent_information_state;

}

#endif //DAEDALUS_EVENTUALITY_H
