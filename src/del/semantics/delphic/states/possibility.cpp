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

#include "../../../../../include/del/semantics/delphic/states/possibility.h"
#include "../../../../../include/del/semantics/delphic/model_checker.h"

using namespace delphic;

possibility::possibility(del::language_ptr language, kripke::label_ptr label, agents_information_state state) :
    m_language{std::move(language)},
    m_label{std::move(label)},
    m_information_state{std::move(state)} {}

del::language_ptr possibility::get_language() const {
    return m_language;
}

const kripke::label &possibility::get_label() const {
    return *m_label;
}

unsigned long possibility::get_depth() const {
    return m_depth;
}

const information_state &possibility::get_information_state(del::agent ag) const {
    return m_information_state[ag];
}

bool possibility::satisfies(const del::formula_ptr f) {
    return model_checker::holds_in(*this, *f);
}
