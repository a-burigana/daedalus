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

#include "action_builder.h"
#include "../../include/del/formulas/formula.h"
#include "../../include/del/formulas/propositional/true_formula.h"
#include "../../include/del/formulas/propositional/and_formula.h"
#include "../../include/del/formulas/propositional/not_formula.h"
#include <memory>

using namespace daedalus::tester;
using namespace del;
using namespace kripke;

action action_builder::build_public_announcement(std::string name, const language_ptr &language, const formula_ptr &f_pre) {
    const event_id events_number = 1;
    action_relations q(language->get_agents_number());

    for (agent ag = 0; ag < language->get_agents_number(); ++ag) {
        q[ag] = action_agent_relations(events_number);
        q[ag][0] = event_set(events_number, event_deque{0});
    }

    preconditions pre(events_number);
    pre[0] = f_pre;

    postconditions post;

    boost::dynamic_bitset<> is_ontic(events_number);
    event_deque designated_events = {0};

    return action{language, std::move(name), events_number, q, pre, post, is_ontic, designated_events};
}

action action_builder::build_public_ontic(std::string name, const del::language_ptr &language,
                                          const del::formula_ptr &f_pre, const event_post &e_post) {
    const event_id events_number = 1;
    action_relations q(language->get_agents_number());

    for (agent ag = 0; ag < language->get_agents_number(); ++ag) {
        q[ag] = action_agent_relations(events_number);
        q[ag][0] = event_set(events_number, event_deque{0});
    }

    preconditions pre(events_number);
    pre[0] = f_pre;

    postconditions post = postconditions(events_number);
    post[0] = e_post;

    boost::dynamic_bitset<> is_ontic(events_number);
    is_ontic.set(0);

    event_deque designated_events = {0};

    return action{language, std::move(name), events_number, q, pre, post, is_ontic, designated_events};
}

action action_builder::build_private_ontic(std::string name, const language_ptr &language, const formula_ptr &f_pre,
                                           const event_post &e_post, const agent_set &fo_ags) {
    const event_id events_number = 2;
    action_relations q(language->get_agents_number());

    for (agent ag = 0; ag < language->get_agents_number(); ++ag) {
        q[ag] = action_agent_relations(events_number);

        for (event_id e = 0; e < events_number; ++e)
            q[ag][e] = event_set(events_number);
    }

    for (agent ag = 0; ag < language->get_agents_number(); ++ag) {
        q[ag][1] = event_set{events_number, event_deque{1}};

        if (fo_ags[ag])
            q[ag][0] = event_set{events_number, event_deque{0}};
        else
            q[ag][0] = event_set{events_number, event_deque{1}};
    }

    preconditions pre(events_number);
    pre[0] = f_pre;
    pre[1] = std::make_shared<true_formula>();

    postconditions post = postconditions(events_number);
    post[0] = e_post;

    boost::dynamic_bitset<> is_ontic(events_number);
    is_ontic.set(0);

    event_deque designated_events = {0};

    return action{language, std::move(name), events_number, q, pre, post, is_ontic, designated_events};
}

action action_builder::build_private_sensing(std::string name, const language_ptr &language, const formula_ptr &f_pre,
                                             const agent_set &fo_ags) {
    const event_id events_number = 2;
    action_relations q(language->get_agents_number());

    for (agent ag = 0; ag < language->get_agents_number(); ++ag) {
        q[ag] = action_agent_relations(events_number);

        for (event_id e = 0; e < events_number; ++e)
            q[ag][e] = event_set(events_number);
    }

    for (agent ag = 0; ag < language->get_agents_number(); ++ag) {
        q[ag][1] = event_set{events_number, event_deque{1}};

        if (fo_ags[ag])
            q[ag][0] = event_set{events_number, event_deque{0}};
        else
            q[ag][0] = event_set{events_number, event_deque{1}};
    }

    preconditions pre(events_number);
    pre[0] = f_pre;
    pre[1] = std::make_shared<true_formula>();

    postconditions post = postconditions(events_number);

    boost::dynamic_bitset<> is_ontic(events_number);

    event_deque designated_events = {0};

    return action{language, std::move(name), events_number, q, pre, post, is_ontic, designated_events};
}

action action_builder::build_semi_private_sensing(std::string name, const language_ptr &language, const formula_ptr &f_pre,
                                                  const formula_ptr &sensed, const agent_set &fo_ags,
                                                  const agent_set &po_ags) {
    const event_id events_number = 3;
    action_relations q(language->get_agents_number());

    for (agent ag = 0; ag < language->get_agents_number(); ++ag) {
        q[ag] = action_agent_relations(events_number);

        for (event_id e = 0; e < events_number; ++e)
            q[ag][e] = event_set(events_number);
    }

    for (agent ag = 0; ag < language->get_agents_number(); ++ag) {
        q[ag][2] = event_set{events_number, event_deque({2})};

        if (fo_ags[ag]) {
            q[ag][0] = event_set{events_number, event_deque({0})};
            q[ag][1] = event_set{events_number, event_deque({1})};
        } else if (po_ags[ag]) {
            q[ag][0] = event_set{events_number, event_deque({0, 1})};
            q[ag][1] = event_set{events_number, event_deque({0, 1})};
        } else {
            q[ag][0] = event_set{events_number, event_deque({2})};
            q[ag][1] = event_set{events_number, event_deque({2})};
        }
    }

    preconditions pre(events_number);
    formula_ptr not_sensed = std::make_shared<not_formula>(sensed);

    pre[0] = std::make_shared<and_formula>(formula_deque{f_pre, sensed});
    pre[1] = std::make_shared<and_formula>(formula_deque{f_pre, not_sensed});
    pre[2] = std::make_shared<true_formula>();

    postconditions post;

    boost::dynamic_bitset<> is_ontic(events_number);
    event_deque designated_events = {0};    // {0, 1};

    return action{language, std::move(name), events_number, q, pre, post, is_ontic, designated_events};
}

action action_builder::build_semi_private_announcement(std::string name, const language_ptr &language, const formula_ptr &f_pre,
                                                       const formula_ptr &announced, const agent_set &fo_ags,
                                                       const agent_set &po_ags) {
    const event_id events_number = 3;
    action_relations q(language->get_agents_number());

    for (agent ag = 0; ag < language->get_agents_number(); ++ag) {
        q[ag] = action_agent_relations(events_number);

        for (event_id e = 0; e < events_number; ++e)
            q[ag][e] = event_set(events_number);
    }

    for (agent ag = 0; ag < language->get_agents_number(); ++ag) {
        q[ag][2] = event_set{events_number, event_deque({2})};

        if (fo_ags[ag]) {
            q[ag][0] = event_set{events_number, event_deque({0})};
            q[ag][1] = event_set{events_number, event_deque({1})};
        } else if (po_ags[ag]) {
            q[ag][0] = event_set{events_number, event_deque({0, 1})};
            q[ag][1] = event_set{events_number, event_deque({0, 1})};
        } else {
            q[ag][0] = event_set{events_number, event_deque({2})};
            q[ag][1] = event_set{events_number, event_deque({2})};
        }
    }

    preconditions pre(events_number);
    formula_ptr not_announced = std::make_shared<not_formula>(announced);

    pre[0] = std::make_shared<and_formula>(formula_deque{f_pre, announced});
    pre[1] = std::make_shared<and_formula>(formula_deque{f_pre, not_announced});
    pre[2] = std::make_shared<true_formula>();

    postconditions post;

    boost::dynamic_bitset<> is_ontic(events_number);
    event_deque designated_events = {0};

    return action{language, std::move(name), events_number, q, pre, post, is_ontic, designated_events};
}

action action_builder::build_private_announcement(std::string name, const language_ptr &language, const formula_ptr &f_pre, const agent_set &fo_ags) {
    const event_id events_number = 2;
    action_relations q(language->get_agents_number());

    for (agent ag = 0; ag < language->get_agents_number(); ++ag) {
        q[ag] = action_agent_relations(events_number);

        for (event_id e = 0; e < events_number; ++e)
            q[ag][e] = event_set(events_number);
    }

    for (agent ag = 0; ag < language->get_agents_number(); ++ag) {
        q[ag][1] = event_set{events_number, event_deque{1}};

        if (fo_ags[ag])
            q[ag][0] = event_set{events_number, event_deque{0}};
        else
            q[ag][0] = event_set{events_number, event_deque{1}};
    }

    preconditions pre(events_number);
    pre[0] = f_pre;
    pre[1] = std::make_shared<true_formula>();

    postconditions post = postconditions(events_number);

    boost::dynamic_bitset<> is_ontic(events_number);

    event_deque designated_events = {0};

    return action{language, std::move(name), events_number, q, pre, post, is_ontic, designated_events};
}

action action_builder::build_quasi_private_announcement(std::string name, const del::language_ptr &language,
                                                        const del::formula_ptr &f_pre_1, const del::formula_ptr &f_pre_2,
                                                        const del::agent_set &fo_ags_1, const del::agent_set &fo_ags_2) {
    const event_id events_number = 3;
    action_relations q(language->get_agents_number());

    for (agent ag = 0; ag < language->get_agents_number(); ++ag) {
        q[ag] = action_agent_relations(events_number);

        for (event_id e = 0; e < events_number; ++e)
            q[ag][e] = event_set(events_number);
    }

    for (agent ag = 0; ag < language->get_agents_number(); ++ag) {
        q[ag][2] = event_set{events_number, event_deque{2}};

        if (fo_ags_1[ag]) {
            q[ag][0] = event_set{events_number, event_deque{0}};
            q[ag][1] = event_set{events_number, event_deque{1}};
        }

        if (fo_ags_2[ag]) {
            q[ag][0] = event_set{events_number, event_deque{1}};
            q[ag][1] = event_set{events_number, event_deque{1}};
        }

        if (not fo_ags_1[ag] and not fo_ags_2[ag]) {
            q[ag][0] = event_set{events_number, event_deque{2}};
            q[ag][1] = event_set{events_number, event_deque{2}};
        }
    }

    preconditions pre(events_number);
    pre[0] = f_pre_1;
    pre[1] = f_pre_2;
    pre[2] = std::make_shared<true_formula>();

    postconditions post = postconditions(events_number);

    boost::dynamic_bitset<> is_ontic(events_number);

    event_deque designated_events = {0};

    return action{language, std::move(name), events_number, q, pre, post, is_ontic, designated_events};
}
