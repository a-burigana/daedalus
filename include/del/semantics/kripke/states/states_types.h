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

#ifndef DAEDALUS_STATES_TYPES_H
#define DAEDALUS_STATES_TYPES_H

#include <cmath>
#include <cstddef>
#include <vector>
#include <deque>
#include <list>
#include <boost/dynamic_bitset.hpp>
#include <ostream>
#include "../../../language/language_types.h"
#include "label.h"
#include "../../../../utils/bit_deque.h"

namespace kripke {
    class state;
    using state_ptr = std::shared_ptr<state>;

    using state_id       = unsigned long long;
    using world_id       = unsigned long long;
    using world_list     = std::list<world_id>;       // TODO: REVERT BACK TO DEQUE!!!!!!!!!!!!!!
    using world_deque    = std::deque<world_id>;
    using world_bitset   = boost::dynamic_bitset<>;
    using world_set      = bit_deque;
    using agent_relation = std::vector<world_set>;
//    using agent_relation = std::vector<world_deque>;
    using relations      = std::vector<agent_relation>;
    using label_vector   = std::vector<label>;

//    class world_set {
//    public:
//        using T_iterator   = world_list::const_iterator;      // TODO: REVERT BACK TO DEQUE!!!!!!!!!!!!!!
//
//        world_set() = default;
//
//        explicit world_set(unsigned long long size, unsigned long id = 0) :
//                m_bitset{boost::dynamic_bitset<>(size)},
//                m_id{id} {}
//
//        world_set(unsigned long long size, world_list deque_, unsigned long id = 0) :       // TODO: REVERT BACK TO DEQUE!!!!!!!!!!!!!!
//                m_bitset{boost::dynamic_bitset<>(size)},
//                m_deque{std::move(deque_)},
//                m_id{id} {
//            for (const world_id w : m_deque)
//                if (not m_bitset[w])
//                    m_bitset[w].flip();
//        }
//
//        explicit world_set(const boost::dynamic_bitset<> &bitset, unsigned long id = 0) : m_id{id} {
//            m_bitset = boost::dynamic_bitset<>(bitset.size());
//
//            for (world_id w = 0; w < bitset.size(); ++w)
//                if (bitset[w])
//                    push_back(w);
//        }
//
//        world_set(const world_set&) = default;
//        world_set& operator=(const world_set&) = default;
//
//        world_set(world_set&&) = default;
//        world_set& operator=(world_set&&) = default;
//
//        ~world_set() = default;
//
//        [[nodiscard]] const boost::dynamic_bitset<> &get_bitset() const { return m_bitset; }
//        [[nodiscard]] bool empty() const { return m_deque.empty(); }
//        [[nodiscard]] size_t size() const { return m_deque.size(); }
//
//        void push_back(const world_id w) {
//            if (not m_bitset[w]) {
//                m_bitset[w].flip();
//                m_deque.push_back(w);
//            }
//        }
//
//        void remove(const world_id w) {
//            if (m_bitset[w]) {
//                m_bitset[w].flip();
//                m_deque.remove(w);      // todo: this takes O(n)... Can we do better?
//            }
//        }
//
//        const unsigned long get_id() const { return m_id; }
//        void set_id(const unsigned long id) { m_id = id; }
//
//        boost::dynamic_bitset<> operator&(const world_set &rhs) { return this->m_bitset & rhs.m_bitset; }
//        boost::dynamic_bitset<> operator-(const world_set &rhs) { return this->m_bitset - rhs.m_bitset; }
//
//        bool operator< (const world_set &rhs) const { return m_bitset.to_ulong() < rhs.m_bitset.to_ulong(); }
//        bool operator<=(const world_set &rhs) const { return m_bitset.to_ulong() <= rhs.m_bitset.to_ulong(); }
//        bool operator> (const world_set &rhs) const { return m_bitset.to_ulong() > rhs.m_bitset.to_ulong(); }
//        bool operator>=(const world_set &rhs) const { return m_bitset.to_ulong() >= rhs.m_bitset.to_ulong(); }
//        bool operator==(const world_set &rhs) const { return m_bitset.to_ulong() == rhs.m_bitset.to_ulong(); }
//        bool operator!=(const world_set &rhs) const { return m_bitset.to_ulong() != rhs.m_bitset.to_ulong(); }
//
//        const boost::dynamic_bitset<> &operator*() const { return m_bitset; }
//        const bool operator[](const world_id w) const { return m_bitset[w]; }
//
//        [[nodiscard]] iterator begin() const { return m_deque.begin(); }
//        [[nodiscard]] iterator end()   const { return m_deque.end();   }
//
//    private:
//        boost::dynamic_bitset<> m_bitset;
//        world_list m_deque;      // TODO: REVERT BACK TO DEQUE!!!!!!!!!!!!!!
//        unsigned long m_id;
//    };
}

#endif //DAEDALUS_STATES_TYPES_H
