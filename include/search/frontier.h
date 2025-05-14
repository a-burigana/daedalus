// MIT License
//
// Copyright (c) 2022 Alessandro Burigana and Francesco Fabiano
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

#ifndef DAEDALUS_FRONTIER_H
#define DAEDALUS_FRONTIER_H

#include "search_space.h"
#include "search_types.h"
#include <deque>
#include <map>

namespace search {
    class frontier {
    public:
        frontier() = default;

        frontier(const frontier&) = delete;
        frontier& operator=(const frontier&) = delete;

        frontier(frontier&&) = default;
        frontier& operator=(frontier&&) = default;

        ~frontier() = default;

        [[nodiscard]] bool empty() const {
            return m_frontiers.empty();
        }

        void push(const node_ptr &n) {
            m_frontiers[n->get_graph_depth()].push_back(n);
        }

        [[nodiscard]] const node_ptr &front() const {
            return m_frontiers.begin()->second.front();
        }

        void pop_front() {
            auto &[depth, deq] = *m_frontiers.begin();
            deq.pop_front();

            if (deq.empty())
                m_frontiers.erase(depth);
        }

        void clear() {
            m_frontiers.clear();
        }

        [[nodiscard]] std::map<unsigned long, node_deque> &get() {
            return m_frontiers;
        }

        [[nodiscard]] auto begin() const { return m_frontiers.begin(); }
        [[nodiscard]] auto end()   const { return m_frontiers.end();   }

    private:
        std::map<unsigned long, node_deque> m_frontiers;
    };
}

#endif //DAEDALUS_FRONTIER_H
