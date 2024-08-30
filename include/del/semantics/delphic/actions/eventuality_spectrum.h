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

#ifndef DAEDALUS_EVENTUALITY_SPECTRUM_H
#define DAEDALUS_EVENTUALITY_SPECTRUM_H

#include "../../../language/language.h"
#include "eventuality_types.h"

namespace delphic {
    class eventuality_spectrum {
    public:
        eventuality_spectrum(del::language_ptr language, std::string name, dynamic_information_state designated_eventualities);

        eventuality_spectrum(const eventuality_spectrum&) = delete;
        eventuality_spectrum& operator=(const eventuality_spectrum&) = delete;

        eventuality_spectrum(eventuality_spectrum&&) = default;
        eventuality_spectrum& operator=(eventuality_spectrum&&) = default;

        ~eventuality_spectrum() = default;

        [[nodiscard]] del::language_ptr get_language() const;
        [[nodiscard]] std::string get_name() const;
        [[nodiscard]] const dynamic_information_state &get_designated_eventualities() const;

        [[nodiscard]] unsigned long get_maximum_depth() const;

    private:
        del::language_ptr m_language;
        std::string m_name;
        dynamic_information_state m_designated_eventualities;
        unsigned long m_maximum_depth;
    };
}

#endif //DAEDALUS_EVENTUALITY_SPECTRUM_H
