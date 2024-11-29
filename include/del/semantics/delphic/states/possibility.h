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

#ifndef DAEDALUS_POSSIBILITY_H
#define DAEDALUS_POSSIBILITY_H

#include <memory>
#include <optional>
#include <unordered_set>
#include <vector>
#include "possibility_types.h"
#include "../../../language/label.h"
#include "../../../formulas/formula.h"
#include "../../../language/language.h"
#include "../../../../utils/storage_types.h"

namespace delphic {
    class possibility {
    public:
        possibility(del::language_ptr language, del::storages_ptr storages,
                    label_id label, agents_information_state state, unsigned long bound = 0,
                    std::optional<unsigned long> world = std::nullopt);

        possibility(const possibility&) = default;
        possibility& operator=(const possibility&) = default;

        possibility(possibility&&) = default;
        possibility& operator=(possibility&&) = default;

        ~possibility() = default;

        [[nodiscard]] del::language_ptr get_language() const;
//        [[nodiscard]] possibility_storage_ptr get_possibility_storage() const;
//        [[nodiscard]] information_state_storage_ptr get_information_state_storage() const;

//        [[nodiscard]] possibility_ptr get(possibility_id w) const;
        [[nodiscard]] const label_id &get_label_id() const;
        [[nodiscard]] unsigned long get_bound() const;
//        [[nodiscard]] const information_state &get_information_state(del::agent ag) const;
        [[nodiscard]] const information_state_id get_information_state_id(del::agent ag) const;

        void set_information_state(del::agent ag, information_state_id &is);

        [[nodiscard]] bool satisfies(const del::formula_ptr &f, const del::storages_ptr &storages) const;

        bool operator< (const possibility &rhs) const;
        bool operator> (const possibility &rhs) const;
        bool operator<=(const possibility &rhs) const;
        bool operator>=(const possibility &rhs) const;
        bool operator==(const possibility &rhs) const;
        bool operator!=(const possibility &rhs) const;

    private:
        del::language_ptr m_language;
        del::storages_ptr m_storages;

        label_id m_label;
        agents_information_state m_information_state;
        unsigned long m_bound;
        std::optional<unsigned long> m_world;
    };
}

#endif //DAEDALUS_POSSIBILITY_H
