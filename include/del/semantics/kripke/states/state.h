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

#ifndef DAEDALUS_STATE_H
#define DAEDALUS_STATE_H

#include <set>
#include <map>
#include <ostream>
#include <vector>
#include "states_types.h"
#include "../../../formulas/formula.h"
#include "../../../language/language.h"

namespace kripke {
    class state {
    public:
        state(del::language_ptr language, unsigned long long worlds_number, relations relations,
              label_vector valuation, world_set designated_worlds, unsigned long long state_id = 0);

        state(const state&) = delete;
        state& operator=(const state&) = delete;

        state(state&&) = default;
        state& operator=(state&&) = default;

        ~state() = default;

        [[nodiscard]] unsigned long long get_worlds_number() const;
        [[nodiscard]] const world_set &get_agent_possible_worlds(del::agent ag, world_id w) const;
        [[nodiscard]] bool has_edge(del::agent ag, world_id w, world_id v) const;
        [[nodiscard]] const del::label &get_label(world_id w) const;
        [[nodiscard]] const world_set &get_designated_worlds() const;
        [[nodiscard]] unsigned long long get_id() const;
        [[nodiscard]] bool is_designated(world_id w) const;

        [[nodiscard]] del::language_ptr get_language() const;
        [[nodiscard]] unsigned long long get_depth(world_id w) const;
        [[nodiscard]] unsigned long long get_max_depth() const;
        [[nodiscard]] bool satisfies(const del::formula_ptr &f) const;

        bool operator< (const state &rhs) const;
        bool operator<=(const state &rhs) const;
        bool operator> (const state &rhs) const;
        bool operator>=(const state &rhs) const;
        bool operator==(const state &rhs) const;
        bool operator!=(const state &rhs) const;

        friend std::ostream &operator<<(std::ostream &os, const state &s);

    private:
        del::language_ptr m_language;
        unsigned long long m_worlds_number;
        relations m_relations;
        label_vector m_labels;
        world_set m_designated_worlds;
        unsigned long long m_state_id;
        std::vector<unsigned long long> m_worlds_depth;
        unsigned long long m_max_depth;

        void calculate_worlds_depth();
    };
}

#endif //DAEDALUS_STATE_H
