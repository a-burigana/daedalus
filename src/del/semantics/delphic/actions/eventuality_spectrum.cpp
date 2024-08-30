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

#include "../../../../../include/del/semantics/delphic/actions/eventuality_spectrum.h"

using namespace del;
using namespace delphic;

eventuality_spectrum::eventuality_spectrum(language_ptr language, std::string name,
                                           dynamic_information_state designated_eventualities) :
        m_language{std::move(language)},
        m_name{std::move(name)},
        m_designated_eventualities{std::move(designated_eventualities)} {}

language_ptr eventuality_spectrum::get_language() const {
    return m_language;
}

std::string eventuality_spectrum::get_name() const {
    return m_name;
}

const dynamic_information_state &eventuality_spectrum::get_designated_eventualities() const {
    return m_designated_eventualities;
}

unsigned long eventuality_spectrum::get_maximum_depth() const {
    return m_maximum_depth;
}
