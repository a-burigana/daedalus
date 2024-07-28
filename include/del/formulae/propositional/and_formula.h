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

#ifndef DAEDALUS_AND_FORMULA_H
#define DAEDALUS_AND_FORMULA_H


#include "../formula.h"

namespace del {
    class and_formula : public formula {
    public:
        explicit and_formula(formula_deque fs);

        and_formula(const and_formula&) = delete;
        and_formula& operator=(const and_formula&) = delete;

        and_formula(and_formula&&) = default;
        and_formula& operator=(and_formula&&) = default;

//        ~and_formula() = default;

        [[nodiscard]] bool holds_in(const kripke::state &s, const kripke::world_id &w) const override;
        [[nodiscard]] bool is_propositional() const override;

        [[nodiscard]] unsigned long get_modal_depth() const override;

        [[nodiscard]] std::string to_string(const language_ptr &language, bool escape_html) const override {
            std::string f_and;

            for (const formula_ptr &f : m_fs)
                f_and += f->to_string(language, escape_html) + " /\\ ";

            return "(" + f_and.substr(0, f_and.size() - 4) + ")";
        }

    private:
        formula_deque m_fs;
    };
}

#endif //DAEDALUS_AND_FORMULA_H
