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

#include "../../../../../include/del/semantics/delphic/actions/eventuality.h"

using namespace delphic;

eventuality::eventuality(del::formula_ptr pre, postconditions post, agents_dynamic_information_state state) :
    m_pre{std::move(pre)},
    m_post{std::move(post)},
    m_information_state{std::move(state)} {
    m_is_ontic = not m_post.empty();
    m_is_idle = m_pre->get_type() == del::formula_type::true_formula and m_post.empty();
}

del::formula_ptr eventuality::get_pre() const {
    return m_pre;
}

postconditions eventuality::get_postconditions() const {
    return m_post;
}

const dynamic_information_state &eventuality::get_information_state(del::agent ag) const {
    return m_information_state[ag];
}

bool eventuality::is_ontic() const {
    return m_is_ontic;
}

bool eventuality::is_idle() const {
    return m_is_idle;
}

void eventuality::set_information_state(del::agent ag, delphic::dynamic_information_state &is) {
    m_information_state[ag] = std::move(is);
}
