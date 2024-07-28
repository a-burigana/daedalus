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

#include "../../../../include/del/formulae/modal/box_formula.h"
#include "../../../../include/del/states/state.h"

using namespace del;

box_formula::box_formula(const agent ag, formula_ptr f) :
    m_ag{ag},
    m_f{std::move(f)} {}

bool box_formula::holds_in(const state &s, const world_id &w) const {
    const auto &worlds = s.get_agent_possible_worlds(m_ag, w);
    return std::all_of(worlds.begin(), worlds.end(), [&](const world_id &v) { return m_f->holds_in(s, v); });
}

bool box_formula::is_propositional() const {
    return false;
}

unsigned long box_formula::get_modal_depth() const {
    return 1 + m_f->get_modal_depth();
}
