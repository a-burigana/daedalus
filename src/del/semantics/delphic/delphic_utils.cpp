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

#include "../../../../include/del/semantics/delphic/delphic_utils.h"
#include "../../../../include/del/semantics/delphic/states/possibility.h"
#include "../../../../include/del/semantics/delphic/states/possibility_spectrum.h"
#include "../../../../include/del/semantics/delphic/states/possibility_types.h"
#include "../../../../include/del/semantics/delphic/actions/eventuality.h"
#include <memory>
#include <unordered_map>
#include <vector>

using namespace delphic;
using namespace kripke;

possibility_spectrum delphic::delphic_utils::convert(const state &s) {
    std::vector<possibility_ptr> possibilities(s.get_worlds_number());
    std::vector<std::vector<information_state>> information_states(s.get_worlds_number());
    information_state designated;

    for (world_id w = 0; w < s.get_worlds_number(); ++w) {
        possibilities[w] = std::make_shared<possibility>(s.get_language(), s.get_label(w), agents_information_state(s.get_language()->get_agents_number()));
        information_states[w] = std::vector<information_state>(s.get_language()->get_agents_number());

        if (s.is_designated(w))
            designated.emplace(possibilities[w]);
    }

    agents_information_state ag_is = agents_information_state(s.get_language()->get_agents_number());

    for (del::agent ag = 0; ag < s.get_language()->get_agents_number(); ++ag)
        for (world_id w = 0; w < s.get_worlds_number(); ++w)
            for (const world_id v : s.get_agent_possible_worlds(ag, w))
                information_states[w][ag].emplace(possibilities[v]);

    for (del::agent ag = 0; ag < s.get_language()->get_agents_number(); ++ag)
        for (world_id w = 0; w < s.get_worlds_number(); ++w)
            possibilities[w]->set_information_state(ag, information_states[w][ag]);

    return possibility_spectrum{s.get_language(), std::move(designated)};       // , s.get_worlds_number()
}

eventuality_spectrum delphic_utils::convert(const kripke::action &a) {
    std::vector<eventuality_ptr> eventualities(a.get_events_number());
    std::vector<std::vector<dynamic_information_state>> information_states(a.get_events_number());
    dynamic_information_state designated;

    for (event_id e = 0; e < a.get_events_number(); ++e) {
        postconditions post;

        if (a.is_ontic(e))
            post = postconditions(a.get_postconditions(e));

        eventualities[e] = std::make_shared<eventuality>(a.get_precondition(e), post, agents_dynamic_information_state(a.get_language()->get_agents_number()));
        information_states[e] = std::vector<dynamic_information_state>(a.get_language()->get_agents_number());

        if (a.is_designated(e))
            designated.emplace(eventualities[e]);
    }

    agents_information_state ag_is = agents_information_state(a.get_language()->get_agents_number());

    for (del::agent ag = 0; ag < a.get_language()->get_agents_number(); ++ag)
        for (event_id e = 0; e < a.get_events_number(); ++e)
            for (const event_id f : a.get_agent_possible_events(ag, e))
                information_states[e][ag].emplace(eventualities[f]);

    for (del::agent ag = 0; ag < a.get_language()->get_agents_number(); ++ag)
        for (event_id e = 0; e < a.get_events_number(); ++e)
            eventualities[e]->set_information_state(ag, information_states[e][ag]);

    return eventuality_spectrum{a.get_language(), a.get_type(), a.get_name(), std::move(designated)};
}

kripke::state delphic_utils::convert(const delphic::possibility_spectrum &W) {
    std::unordered_map<possibility_ptr, kripke::world_id> world_map;
    std::unordered_set<possibility_ptr> to_visit;
    std::unordered_set<possibility_ptr> visited;
    kripke::world_id worlds_number = 0;

    for (const auto &w : W.get_designated_possibilities())
        to_visit.emplace(w);

    while (not to_visit.empty()) {
        auto first = to_visit.begin();
        const possibility_ptr &current = *first;

        world_map[current] = worlds_number++;
        visited.emplace(current);

        for (del::agent ag = 0; ag < W.get_language()->get_agents_number(); ++ag)
            for (auto &w : current->get_information_state(ag))
                if (visited.find(w) == visited.end())
                    to_visit.emplace(w);

        to_visit.erase(first);
    }

    kripke::relations r(W.get_language()->get_agents_number());

    for (del::agent ag = 0; ag < W.get_language()->get_agents_number(); ++ag) {
        r[ag] = kripke::agent_relation(worlds_number);

        for (kripke::world_id w = 0; w < worlds_number; ++w)
            r[ag][w] = kripke::world_set(worlds_number);

        for (const auto &[w, w_id] : world_map)
            for (auto &v : w->get_information_state(ag))
                r[ag][w_id].push_back(world_map[v]);
    }

    kripke::label_vector ls(worlds_number);

    for (const auto &[w, w_id] : world_map)
        ls[w_id] = w->get_label();

    kripke::world_set designated_worlds(worlds_number);

    for (const auto &w : W.get_designated_possibilities())
        designated_worlds.push_back(world_map[w]);

    return kripke::state{W.get_language(), worlds_number, std::move(r), std::move(ls), std::move(designated_worlds)};
}

kripke::action delphic_utils::convert(const delphic::eventuality_spectrum &E) {
    std::unordered_map<eventuality_ptr, kripke::event_id> event_map;
    std::unordered_set<eventuality_ptr> to_visit;
    std::unordered_set<eventuality_ptr> visited;
    kripke::event_id events_number = 0;

    for (const auto &e : E.get_designated_eventualities())
        to_visit.emplace(e);

    while (not to_visit.empty()) {
        auto first = to_visit.begin();
        const eventuality_ptr &current = *first;

        event_map[current] = events_number++;

        for (del::agent ag = 0; ag < E.get_language()->get_agents_number(); ++ag)
            for (auto &e : current->get_information_state(ag))
                if (visited.find(e) == visited.end())
                    to_visit.emplace(e);

        to_visit.erase(first);
    }

    kripke::action_relations q(E.get_language()->get_agents_number());

    for (del::agent ag = 0; ag < E.get_language()->get_agents_number(); ++ag) {
        q[ag] = kripke::action_agent_relations(events_number);

        for (kripke::event_id e = 0; e < events_number; ++e)
            q[ag][e] = kripke::event_set(events_number);

        for (const auto &[e, e_id] : event_map)
            for (auto &f : e->get_information_state(ag))
                q[ag][e_id].push_back(event_map[f]);
    }

    kripke::preconditions pre(events_number);
    kripke::postconditions post(events_number);
    boost::dynamic_bitset<> is_ontic(events_number);

    for (const auto &[e, e_id] : event_map) {
        pre[e_id] = e->get_pre();
        post[e_id] = e->get_postconditions();
        is_ontic[e_id] = e->is_ontic();
    }

    kripke::event_deque designated_events(events_number);

    for (const auto &e : E.get_designated_eventualities())
        designated_events.push_back(event_map[e]);

    return kripke::action{E.get_language(), E.get_type(), E.get_name(), events_number, std::move(q), std::move(pre),
                          std::move(post), std::move(is_ontic), std::move(designated_events)};
}

search::delphic_planning_task delphic_utils::convert(const search::planning_task &task) {
    delphic::action_deque actions;

    for (const action_ptr &a : task.get_actions())
        actions.push_back(std::make_shared<eventuality_spectrum>(delphic_utils::convert(*a)));

    return search::delphic_planning_task{task.get_domain_name(), task.get_problem_id(), task.get_language(),
                                         delphic_utils::convert(*task.get_initial_state()), std::move(actions), task.get_goal()};
}
