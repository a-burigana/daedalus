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

#ifndef BEP_LABEL_H
#define BEP_LABEL_H

#include "boost/dynamic_bitset.hpp"
#include "../language/language_types.h"

namespace del {
    class label;
    using label_ptr = std::shared_ptr<label>;

    class label {
    public:
        label() = default;

        explicit label(boost::dynamic_bitset<> bitset);

        label(const label&) = default;
        label& operator=(const label&) = default;

        label(label&&) = default;       // todo: make it delete?
        label& operator=(label&&) = default;

        ~label() = default;

        [[nodiscard]] unsigned long get_id() const;

        boost::dynamic_bitset<> &operator*();
        bool operator[](const atom &p) const;
        void update(atom p, bool value);

        bool operator==(const label &rhs) const;
        bool operator!=(const label &rhs) const;
        bool operator< (const label &rhs) const;
        bool operator> (const label &rhs) const;
        bool operator<=(const label &rhs) const;
        bool operator>=(const label &rhs) const;

    private:
        boost::dynamic_bitset<> m_bitset;
        unsigned long m_id;
    };
}

#endif //BEP_LABEL_H
