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

template<typename Elem>
typename storage<Elem>::Elem_id storage<Elem>::emplace(Elem &&elem) {
    if (m_elements_ids.find(elem) != m_elements_ids.end())          // If the element is already stored, then we simply
        return m_elements_ids.at(elem);                             // return its id

    const auto &[it, _] = m_elements_ids.emplace(std::move(elem), m_elements_ids.size());
    m_elements.emplace_back(std::make_shared<Elem>(it->first));     // Otherwise, we assign the new element a fresh id,
    return m_elements_ids.size() - 1;                               // we add it to the deque to ensure constant time
}                                                                   // retrieval from id and we return its id

template<typename Elem>
typename storage<Elem>::Elem_ptr storage<Elem>::get(Elem_id id) const {
    return m_elements[id];
}
