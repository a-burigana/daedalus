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

#ifndef DAEDALUS_ACTIONS_TYPES_H
#define DAEDALUS_ACTIONS_TYPES_H

#include <memory>
#include <vector>
#include <set>
#include <list>
#include "../../../language/language_types.h"
#include "../../../../utils/bit_deque.h"
#include "../../../formulas/formula.h"

namespace kripke {
    class formula;
    class action;
    using action_ptr   = std::shared_ptr<action>;
    using action_set   = std::set<kripke::action_ptr>;
    using action_deque = std::deque<kripke::action_ptr>;

    using event_id               = unsigned long long;
    using event_deque            = std::list<event_id>;    // std::deque<event_id>; TODO: CHANGE BACK TO DEQUE!!!!!!!!!!!!!!!!!
//    using event_set              = std::set<const event_id>;
    using event_set              = bit_deque;
    using action_agent_relations = std::vector<event_set>;
    using action_relations       = std::vector<action_agent_relations>;

    using preconditions  = std::vector<del::formula_ptr>;
    using event_post     = std::unordered_map<del::atom, del::formula_ptr>;
    using postconditions = std::vector<event_post>;

    enum class action_type : uint8_t {
        public_ontic,
        private_ontic,
        semi_private_sensing,
        public_sensing,
        private_announcement,
        semi_private_announcement,
        public_announcement
    };
}

#endif //DAEDALUS_ACTIONS_TYPES_H
