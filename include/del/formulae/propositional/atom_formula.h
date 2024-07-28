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

#ifndef BEP_ATOM_FORMULA_H
#define BEP_ATOM_FORMULA_H

#include "../formula.h"
#include "../../language/language_types.h"
#include "../../states/states_types.h"
#include "../../language/language.h"


namespace del {
    class atom_formula : public formula {
    public:
        explicit atom_formula(atom atom);

        atom_formula(const atom_formula&) = delete;
        atom_formula& operator=(const atom_formula&) = delete;

        atom_formula(atom_formula&&) = default;
        atom_formula& operator=(atom_formula&&) = default;

        [[nodiscard]] bool holds_in(const state &s, const world_id &w) const override;
        [[nodiscard]] bool is_propositional() const override;

        [[nodiscard]] unsigned long get_modal_depth() const override;

        [[nodiscard]] std::string to_string(const language_ptr &language, bool escape_html) const override {
            return language->get_atom_name(m_atom);
        }

    private:
        atom m_atom;
    };
}

#endif //BEP_ATOM_FORMULA_H
