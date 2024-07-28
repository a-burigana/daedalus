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

#include "../../../include/del/states/label.h"

using namespace del;

label::label(boost::dynamic_bitset<> bitset) :
        m_bitset{std::move(bitset)},
        m_id{m_bitset.to_ulong()} {}

unsigned long label::get_id() const {
    return m_id;
}

boost::dynamic_bitset<> &label::operator*() {
    return m_bitset;
}

bool label::operator[](const atom &p) const {
    return m_bitset[p];
}

void label::update(const atom p, const bool value) {
    m_bitset[p] = value;
    auto offset = static_cast<unsigned long>(std::exp2(m_bitset.size() - p-1));
    m_id = value ? m_id + offset : m_id - offset;
}

bool label::operator==(const label &rhs) const {
    return m_id == rhs.m_id;
}

bool label::operator!=(const label &rhs) const {
    return m_id != rhs.m_id;
}

bool label::operator<(const label &rhs) const {
    return m_id < rhs.m_id;
}

bool label::operator>(const label &rhs) const {
    return m_id > rhs.m_id;
}

bool label::operator<=(const label &rhs) const {
    return m_id <= rhs.m_id;
}

bool label::operator>=(const label &rhs) const {
    return m_id >= rhs.m_id;
}
