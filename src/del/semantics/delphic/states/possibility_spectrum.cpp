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

#include "../../../../../include/del/semantics/delphic/states/possibility_spectrum.h"
#include "../../../../../include/del/semantics/delphic/states/possibility.h"
#include "../../../../../include/del/semantics/delphic/model_checker.h"
#include "../../../../../include/del/semantics/delphic/delphic_utils.h"
#include "../../../../../include/del/semantics/kripke/states/state.h"

using namespace del;
using namespace delphic;

possibility_spectrum::possibility_spectrum(del::language_ptr language, delphic::information_state designated_possibilities):
//                                           unsigned long possibilities_number) :
        m_language{std::move(language)},
        m_designated_possibilities{std::move(designated_possibilities)} {
//        m_possibilities_number{possibilities_number}
    m_max_depth = 0;

    for (const possibility_ptr &w : m_designated_possibilities)
        if (w->get_bound() > m_max_depth)
            m_max_depth = w->get_bound();
}

del::language_ptr possibility_spectrum::get_language() const {
    return m_language;
}

const information_state &possibility_spectrum::get_designated_possibilities() const {
    return m_designated_possibilities;
}

bool possibility_spectrum::satisfies(const del::formula_ptr &f) const {
    return std::all_of(m_designated_possibilities.begin(), m_designated_possibilities.end(),
                       [&](const possibility_ptr &w) { return model_checker::holds_in(*w, *f); });
}

//unsigned long possibility_spectrum::get_possibilities_number() const {
//    return m_possibilities_number;
//}

bool possibility_spectrum::is_designated(const possibility_ptr &w) const {
    return std::any_of(m_designated_possibilities.begin(), m_designated_possibilities.end(),
                       [&](const possibility_ptr &w_d) { return *w_d == *w; });

//    for (const auto &w_d : m_designated_possibilities)
//        if (*w_d == *w)
//            return true;
//
//    return false;

//    return m_designated_possibilities.find(w) != m_designated_possibilities.end();
}

unsigned long long possibility_spectrum::get_max_depth() const {
    return m_max_depth;
}

std::ostream &delphic::operator<<(std::ostream &os, const possibility_spectrum &W) {
    kripke::state s = delphic_utils::convert(W);
    os << s;

    return os;
}
