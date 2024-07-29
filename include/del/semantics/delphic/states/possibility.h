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

#ifndef DAEDALUS_POSSIBILITY_H
#define DAEDALUS_POSSIBILITY_H

#include <memory>
#include <set>
#include <vector>
#include "../../kripke/states/label.h"
#include "../../../formulas/formula.h"
#include "../../../language/language.h"

namespace delphic {
    class possibility;
    using possibility_ptr = std::shared_ptr<possibility>;
    using possibility_id = unsigned long;

    class possibility {
    public:
        using agent_information_state = std::set<possibility_id>;
        using information_state = std::vector<agent_information_state>;

        possibility(del::language_ptr language, kripke::label_ptr label, information_state state);

        possibility(const possibility&) = default;
        possibility& operator=(const possibility&) = default;

        possibility(possibility&&) = default;
        possibility& operator=(possibility&&) = default;

        ~possibility() = default;

        [[nodiscard]] del::language_ptr get_language() const;
        [[nodiscard]] const kripke::label &get_label() const;
        [[nodiscard]] const agent_information_state &get_information_state(del::agent ag) const;

        bool satisfies(del::formula_ptr f);

    private:
        del::language_ptr m_language;
        kripke::label_ptr m_label;      // todo: move label outside kripke
        information_state m_information_state;
        unsigned long m_depth;
    };

    using possibility_spectrum = possibility::agent_information_state;
}

#endif //DAEDALUS_POSSIBILITY_H
