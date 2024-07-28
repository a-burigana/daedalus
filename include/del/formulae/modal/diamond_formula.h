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

#ifndef DAEDALUS_DIAMOND_FORMULA_H
#define DAEDALUS_DIAMOND_FORMULA_H

#include "../../language/language_types.h"
#include "../formula.h"
#include "../../semantics/kripke/states/states_types.h"

namespace del {
    class diamond_formula : public formula {
    public:
        diamond_formula(agent ag, const formula_ptr f);

        diamond_formula(const diamond_formula&) = delete;
        diamond_formula& operator=(const diamond_formula&) = delete;

        diamond_formula(diamond_formula&&) = default;
        diamond_formula& operator=(diamond_formula&&) = default;

//        ~diamond_formula() = default;

        [[nodiscard]] bool holds_in(const kripke::state &s, const kripke::world_id &w) const override;
        [[nodiscard]] bool is_propositional() const override;

        [[nodiscard]] unsigned long get_modal_depth() const override;

        [[nodiscard]] std::string to_string(const language_ptr &language, bool escape_html) const override {
            return escape_html
                ? "&lt;" + language->get_agent_name(m_ag) + "&gt;" + m_f->to_string(language, escape_html)
                : "<"    + language->get_agent_name(m_ag) + ">"    + m_f->to_string(language, escape_html);
        }

    private:
        agent m_ag;
        formula_ptr m_f;
    };
}

#endif //DAEDALUS_DIAMOND_FORMULA_H
