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

#include "../../include/utils/storage.h"
#include <memory>

template<typename Elem>
wrapper<Elem>::wrapper(Elem value, unsigned long id) :
        m_value{std::move(value)},
        m_id{id} {}

template<typename Elem>
const Elem &wrapper<Elem>::get_value() const {
    return m_value;
}

template<typename Elem>
unsigned long wrapper<Elem>::get_id() const {
    return m_id;
}

template<typename Elem>
typename storage<Elem>::elem_wrapper_ptr storage<Elem>::push(Elem &elem) {
    wrapper<elem_ptr> wrap = wrapper<elem_ptr>(std::make_shared<Elem>(std::move(elem)), m_storage_deque.size());
    auto result = m_storage_set.emplace(std::move(wrap));

    if (result.second)
        m_storage_deque.push_back(result.first->get_value());

    return *result.first;
}

template<typename Elem>
typename storage<Elem>::elem_ptr storage<Elem>::get(unsigned long id) const {
    return m_storage_deque[id];
}
