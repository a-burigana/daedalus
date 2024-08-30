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

#ifndef DAEDALUS_POSSIBILITY_SPECTRUM_H
#define DAEDALUS_POSSIBILITY_SPECTRUM_H

#include "../../../language/language.h"
#include "possibility_types.h"
#include "../../../formulas/formula.h"

namespace delphic {
    class possibility_spectrum {
    public:
        possibility_spectrum(del::language_ptr language, information_state designated_possibilities);

        possibility_spectrum(const possibility_spectrum&) = delete;
        possibility_spectrum& operator=(const possibility_spectrum&) = delete;

        possibility_spectrum(possibility_spectrum&&) = default;
        possibility_spectrum& operator=(possibility_spectrum&&) = default;

        ~possibility_spectrum() = default;

        [[nodiscard]] const information_state &get_designated_possibilities() const;

        [[nodiscard]] del::language_ptr get_language() const;
        [[nodiscard]] unsigned long long get_max_depth() const;
        [[nodiscard]] bool satisfies(const del::formula_ptr &f) const;

    private:
        del::language_ptr m_language;
        information_state m_designated_possibilities;
        unsigned long long m_max_depth;
    };
}

#endif //DAEDALUS_POSSIBILITY_SPECTRUM_H
