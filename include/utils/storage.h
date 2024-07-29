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

#ifndef DAEDALUS_STORAGE_H
#define DAEDALUS_STORAGE_H

#include <memory>
#include <set>
#include <deque>
#include <utility>

template<typename Elem>
class wrapper {
public:
    wrapper(Elem value, unsigned long id);

    wrapper(const wrapper&) = default;
    wrapper& operator=(const wrapper&) = default;

    wrapper(wrapper&&) noexcept = default;
    wrapper& operator=(wrapper&&) noexcept = default;

    ~wrapper() = default;

    const Elem &get_value() const;
    unsigned long get_id() const;

    bool operator==(const wrapper &rhs) const { return m_value == rhs.m_value; }
    bool operator!=(const wrapper &rhs) const { return m_value != rhs.m_value; }
    bool operator< (const wrapper &rhs) const { return m_value <  rhs.m_value; }
    bool operator> (const wrapper &rhs) const { return m_value >  rhs.m_value; }
    bool operator<=(const wrapper &rhs) const { return m_value <= rhs.m_value; }
    bool operator>=(const wrapper &rhs) const { return m_value >= rhs.m_value; }

private:
    unsigned long m_id;
    Elem m_value;
};

template<typename Elem>
class storage {
public:
    using elem_ptr         = std::shared_ptr<Elem>;
    using elem_wrapper_ptr = wrapper<elem_ptr>;

    storage() = default;

    storage(const storage&) = default;
    storage& operator=(const storage&) = default;

    storage(storage&&) noexcept = default;
    storage& operator=(storage&&) noexcept = default;

    ~storage() = default;

    elem_wrapper_ptr push(Elem &elem);
    elem_ptr get(unsigned long id) const;

private:
    std::set<elem_wrapper_ptr> m_storage_set;
    std::deque<elem_ptr> m_storage_deque;
};

#endif //DAEDALUS_STORAGE_H
