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

#include <utility>

#include "../../../../include/del/formulae/propositional/or_formula.h"

using namespace del;

or_formula::or_formula(formula_deque fs) :
    m_fs{std::move(fs)} {}

bool or_formula::holds_in(const state &s, const world_id &w) const {
    auto check = [&](const formula_ptr &f) { return f->holds_in(s, w); };
    return std::any_of(m_fs.begin(), m_fs.end(), check);
}

bool or_formula::is_propositional() const {
    auto check = [&](const formula_ptr &f) { return f->is_propositional(); };
    return std::all_of(m_fs.begin(), m_fs.end(), check);
}

unsigned long or_formula::get_modal_depth() const {
    auto comp = [](const formula_ptr &f1, const formula_ptr &f2) { return f1->get_modal_depth() < f2->get_modal_depth(); };
    return (*std::max_element(m_fs.begin(), m_fs.end(), comp))->get_modal_depth();
}
