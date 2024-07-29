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

#ifndef DAEDALUS_FORMULA_H
#define DAEDALUS_FORMULA_H

//#include "../semantics/kripke/states/states_types.h"
//#include "../language/language.h"
#include <deque>
#include <variant>
#include "formula_types.h"

namespace del {
//    class true_formula;
//    class false_formula;
//    class atom_formula;
//    class not_formula;
//    class and_formula;
//    class or_formula;
//    class imply_formula;
//    class box_formula;
//    class diamond_formula;
//
//    using formula       = std::variant<true_formula, false_formula, atom_formula,
//                                       not_formula, and_formula, or_formula, imply_formula,
//                                       box_formula, diamond_formula>;
    class formula;
    using formula_ptr   = std::shared_ptr<formula>;
    using formula_deque = std::deque<formula_ptr>;

    class formula {
    public:
        formula() :
            m_type{formula_type::true_formula},
            m_modal_depth{0} {}

        formula(const formula&) = delete;
        formula& operator=(const formula&) = delete;

        formula(formula&&) = default;
        formula& operator=(formula&&) = default;

        virtual ~formula() = default;

        [[nodiscard]] virtual formula_type get_type() const { return m_type; }
        [[nodiscard]] virtual unsigned long get_modal_depth() const { return m_modal_depth; }
        [[nodiscard]] virtual bool is_propositional() const { return m_modal_depth == 0; }

    protected:
        formula_type m_type;
        unsigned long m_modal_depth;
    };
}

//#include "modal/box_formula.h"
//#include "modal/diamond_formula.h"
//#include "propositional/and_formula.h"
//#include "propositional/atom_formula.h"
//#include "propositional/false_formula.h"
//#include "propositional/imply_formula.h"
//#include "propositional/not_formula.h"
//#include "propositional/or_formula.h"
//#include "propositional/true_formula.h"

/*namespace delphic {
    class possibility;
}

namespace del {
    class formula;
    using formula_ptr   = std::shared_ptr<formula>;
    using formula_deque = std::deque<formula_ptr>;

    class formula {
    public:
        formula();

        formula(const formula&) = delete;
        formula& operator=(const formula&) = delete;

        formula(formula&&) = default;
        formula& operator=(formula&&) = default;

        virtual ~formula() = default;

        [[nodiscard]] virtual bool holds_in(const kripke::state &s, const kripke::world_id &w) const = 0;
        [[nodiscard]] virtual bool holds_in(const delphic::possibility &w) const = 0;
        [[nodiscard]] virtual bool is_propositional() const = 0;

        [[nodiscard]] virtual unsigned long get_modal_depth() const = 0;

        [[nodiscard]] virtual std::string to_string(const language_ptr &language, bool escape_html) const = 0;

    protected:
        unsigned long m_modal_depth;
    };
}*/

#endif //DAEDALUS_FORMULA_H
