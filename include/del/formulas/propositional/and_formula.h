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
#include <algorithm>

namespace del {
    class and_formula : public formula {
    public:
        explicit and_formula(formula_deque fs) :
                m_fs{std::move(fs)} {
            m_type = formula_type::and_formula;
            auto comp = [](const formula_ptr &f1, const formula_ptr &f2) { return f1->get_modal_depth() < f2->get_modal_depth(); };

            m_modal_depth = m_fs.empty() ? 0 : (*std::max_element(m_fs.begin(), m_fs.end(), comp))->get_modal_depth();
        }

        and_formula(const and_formula&) = delete;
        and_formula& operator=(const and_formula&) = delete;

        and_formula(and_formula&&) = default;
        and_formula& operator=(and_formula&&) = default;

        [[nodiscard]] const formula_deque &get_fs() const { return m_fs; }

    private:
        formula_deque m_fs;
    };
}

#endif //DAEDALUS_AND_FORMULA_H
