//
// BEP - Bounded Epistemic Planner (MIT License)
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

#include <utility>

#include "../../../../include/del/formulae/propositional/imply_formula.h"
#include "../../../../include/del/states/states_types.h"

using namespace del;

imply_formula::imply_formula(formula_ptr f1, formula_ptr f2) :
        m_f1{std::move(f1)},
        m_f2{std::move(f2)} {}

bool imply_formula::holds_in(const state &s, const world_id &w) const {
    return not m_f1->holds_in(s, w) or m_f2->holds_in(s, w);
}

bool imply_formula::is_propositional() const {
    return m_f1->is_propositional() and m_f2->is_propositional();
}

unsigned long imply_formula::get_modal_depth() const {
    return std::max(m_f1->get_modal_depth(), m_f2->get_modal_depth());
}