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

#include "possibility_types.h"
#include "../../../language/language.h"
#include "../../../formulas/formula.h"
#include "../../../../utils/storage_types.h"

namespace delphic {
    class possibility_spectrum {
    public:
        possibility_spectrum(del::language_ptr language, const del::possibility_storage &p_storage,
                             const del::information_state_storage &is_storage, information_state_id designated_possibilities); // , unsigned long possibilities_number);

        possibility_spectrum(const possibility_spectrum&) = delete;
        possibility_spectrum& operator=(const possibility_spectrum&) = delete;

        possibility_spectrum(possibility_spectrum&&) = default;
        possibility_spectrum& operator=(possibility_spectrum&&) = default;

        ~possibility_spectrum() = default;

        [[nodiscard]] const information_state &get_designated_possibilities() const;

        [[nodiscard]] del::language_ptr get_language() const;
//        [[nodiscard]] possibility_storage_ptr get_possibility_storage() const;
//        [[nodiscard]] information_state_storage_ptr get_information_state_storage() const;
//        [[nodiscard]] unsigned long get_possibilities_number() const;

//        [[nodiscard]] possibility_ptr get(possibility_id w) const;
//        [[nodiscard]] possibility_id emplace_possibility(possibility &&w);
//        [[nodiscard]] information_state_id emplace_information_state(information_state &&is);

        [[nodiscard]] bool is_designated(possibility_id w) const;
        [[nodiscard]] unsigned long long get_max_depth() const;
        [[nodiscard]] bool satisfies(const del::formula_ptr &f, del::storages_handler_ptr handler) const;

        friend std::ostream &operator<<(std::ostream &os, const possibility_spectrum &s);

    private:
        del::language_ptr m_language;
//        possibility_storage_ptr m_p_storage;
//        information_state_storage_ptr m_is_storage;
        information_state m_designated_possibilities;
//        unsigned long m_possibilities_number;
        unsigned long long m_max_depth;
    };
}

#endif //DAEDALUS_POSSIBILITY_SPECTRUM_H
