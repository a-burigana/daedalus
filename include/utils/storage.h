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
#include <deque>
#include <map>

namespace del {
    template<typename Elem>
    class storage {
        using storage_ptr = std::shared_ptr<storage<Elem>>;
        using Elem_ptr = std::shared_ptr<Elem>;
        using Elem_id = unsigned long long;

    public:
        storage();

        explicit storage(Elem &&null);

        storage(const storage &) = default;
        storage &operator=(const storage &) = default;

        storage(storage &&) noexcept = default;
        storage &operator=(storage &&) noexcept = default;

        ~storage() = default;

        Elem_id emplace(Elem &&elem);
        Elem_ptr get(Elem_id id) const;
        [[nodiscard]] bool is_null(Elem_id id) const;

    private:
        std::map<Elem, Elem_id> m_elements_ids;
        std::deque<Elem_ptr> m_elements;
        unsigned long m_count;
    };
}

#endif //DAEDALUS_STORAGE_H
