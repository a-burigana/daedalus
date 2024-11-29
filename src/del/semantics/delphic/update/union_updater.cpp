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

#include "../../../../../include/del/semantics/delphic/update/union_updater.h"
#include "../../../../../include/del/semantics/delphic/states/possibility_types.h"
#include "../../../../../include/del/semantics/delphic/states/possibility.h"
#include "../../../../../include/del/semantics/delphic/actions/eventuality.h"
#include "../../../../../include/del/semantics/delphic/model_checker.h"
#include "../../../../utils/storage.cpp"
#include <iterator>
#include <memory>
#include <unordered_map>
#include <vector>

using namespace delphic;

bool union_updater::is_applicable(const possibility_ptr &w, const eventuality_ptr &e, const del::storages_ptr &storages) {
    return model_checker::holds_in(*w, *e->get_pre(), storages);
}

bool union_updater::is_applicable(const possibility_spectrum_ptr &W, const eventuality_spectrum_ptr &E, const del::storages_ptr &storages) {
    // TODO: UNCOMMENT AND FIX
//    return std::all_of(W->get_designated_possibilities().begin(), W->get_designated_possibilities().end(),
//                       [&](const possibility_id &w) {
//                           return std::any_of(E->get_designated_eventualities().begin(),
//                                              E->get_designated_eventualities().end(),
//                                              [&](const eventuality_ptr &e) {
//                                                  return model_checker::holds_in(*W->get(w), *e->get_pre());
//                                              });
//                       });
}

possibility_spectrum_ptr
union_updater::update(const delphic::possibility_spectrum_ptr &W, const delphic::eventuality_spectrum_ptr &E) {
    switch (E->get_type()) {
        case action_type::public_announcement:
            return update_public_announcement(W, E);
        case action_type::public_sensing:
            return update_public_sensing(W, E);
        case action_type::public_ontic:
            return update_public_ontic(W, E);
        case action_type::private_announcement:
            return update_private_announcement(W, E);
        case action_type::private_ontic:
            return update_private_ontic(W, E);
        case action_type::semi_private_announcement:
            return update_semi_private_announcement(W, E);
        case action_type::semi_private_sensing:
            return update_semi_private_sensing(W, E);
    }
}

possibility_spectrum_ptr
union_updater::update_public_announcement(const possibility_spectrum_ptr &W, const eventuality_spectrum_ptr &E) {
    del::agent_set o_ags(W->get_language()->get_agents_number());
    return update_public_private_action(W, E, o_ags);
}

possibility_spectrum_ptr
union_updater::update_public_sensing(const possibility_spectrum_ptr &W, const eventuality_spectrum_ptr &E) {
    del::agent_set o_ags(W->get_language()->get_agents_number());
    return update_public_private_action(W, E, o_ags);
}

possibility_spectrum_ptr
union_updater::update_public_ontic(const possibility_spectrum_ptr &W, const eventuality_spectrum_ptr &E) {
    del::agent_set o_ags(W->get_language()->get_agents_number());
    return update_public_private_action(W, E, o_ags);
}

possibility_spectrum_ptr
union_updater::update_private_announcement(const possibility_spectrum_ptr &W, const eventuality_spectrum_ptr &E) {
    del::agent_set o_ags(W->get_language()->get_agents_number());
    eventuality_ptr e = *E->get_designated_eventualities().begin();

    for (del::agent ag = 0; ag < W->get_language()->get_agents_number(); ++ag)
        if (e->get_information_state(ag).find(e) == e->get_information_state(ag).end())
            o_ags[ag] = true;

    return update_public_private_action(W, E, o_ags);
}

possibility_spectrum_ptr
union_updater::update_private_ontic(const possibility_spectrum_ptr &W, const eventuality_spectrum_ptr &E) {
    del::agent_set o_ags(W->get_language()->get_agents_number());
    eventuality_ptr e = *E->get_designated_eventualities().begin();

    for (del::agent ag = 0; ag < W->get_language()->get_agents_number(); ++ag)
        if (e->get_information_state(ag).find(e) == e->get_information_state(ag).end())
            o_ags[ag] = true;

    return update_public_private_action(W, E, o_ags);
}

possibility_spectrum_ptr
union_updater::update_semi_private_sensing(const possibility_spectrum_ptr &W, const eventuality_spectrum_ptr &E) {
    del::agent_set o_ags(W->get_language()->get_agents_number());
    eventuality_ptr e = *E->get_designated_eventualities().begin();

    for (del::agent ag = 0; ag < W->get_language()->get_agents_number(); ++ag)
        if (e->get_information_state(ag).find(e) == e->get_information_state(ag).end())
            o_ags[ag] = true;

    return update_semi_private_action(W, E, o_ags);
}

possibility_spectrum_ptr
union_updater::update_semi_private_announcement(const possibility_spectrum_ptr &W, const eventuality_spectrum_ptr &E) {
    del::agent_set o_ags(W->get_language()->get_agents_number());
    eventuality_ptr e = *E->get_designated_eventualities().begin();

    for (del::agent ag = 0; ag < W->get_language()->get_agents_number(); ++ag)
        if (e->get_information_state(ag).find(e) == e->get_information_state(ag).end())
            o_ags[ag] = true;

    return update_semi_private_action(W, E, o_ags);
}

possibility_spectrum_ptr union_updater::update_public_private_action(const delphic::possibility_spectrum_ptr &W,
                                                                     const delphic::eventuality_spectrum_ptr &E, const del::agent_set &o_ags) {
    information_state designated_p;

    for (const eventuality_ptr &e : E->get_designated_eventualities()) {
        possibility_map update_map;

        update_possibilities(W, e, o_ags, update_map);
        update_information_states(W, o_ags, update_map);
        update_designated(W, designated_p, update_map);
    }

    // TODO: UNCOMMENT AND FIX
//    information_state_id id = W->emplace_information_state(std::move(designated_p));
//
//    return std::make_shared<possibility_spectrum>(W->get_language(), W->get_possibility_storage(), W->get_information_state_storage(), id);
}

possibility_spectrum_ptr union_updater::update_semi_private_action(const delphic::possibility_spectrum_ptr &W,
                                                                   const delphic::eventuality_spectrum_ptr &E, const del::agent_set &o_ags) {
    information_state designated;
    possibility_map update_map;
    std::vector<eventuality_ptr> es;

    for (const eventuality_ptr &e : E->get_designated_eventualities())
        es.push_back(e);

    del::agent actor;

    for (del::agent ag = 0; ag < W->get_language()->get_agents_number(); ++ag) {
        const auto &is = es[0]->get_information_state(ag);

        if (is.size() == 1 and is.find(es[0]) != is.end()) {
            actor = ag;
            break;
        }
    }

    del::agent_set po_ags(W->get_language()->get_agents_number());

    for (del::agent ag = 0; ag < W->get_language()->get_agents_number(); ++ag)
        if (ag != actor and not o_ags[ag])
            po_ags[ag] = true;

    update_possibilities(W, es[0], o_ags, update_map);
    update_possibilities(W, es[1], o_ags, update_map);

    update_information_states(W, po_ags, o_ags, es[0]->get_pre(), update_map);
    update_designated(W, designated, update_map);

    // TODO: UNCOMMENT AND FIX
//    information_state_id id = W->emplace_information_state(std::move(designated));

//    return std::make_shared<possibility_spectrum>(W->get_language(), W->get_possibility_storage(), W->get_information_state_storage(), id);
}

void union_updater::update_possibilities(const delphic::possibility_spectrum_ptr &W, const delphic::eventuality_ptr &e,
                                         const del::agent_set &o_ags, delphic::possibility_map &update_map, bool negated) {
    information_state to_visit, visited;

    for (const auto &w: W->get_designated_possibilities())
        to_visit.emplace(w);

    while (not to_visit.empty()) {
        auto first = to_visit.begin();
        const possibility_id current_id = *first;
        const possibility_ptr current; // TODO: UNCOMMENT AND FIX = W->get(current_id);

        // TODO: UNCOMMENT AND FIX
//        if (current->satisfies(e->get_pre()) != negated) {
//            agents_information_state is = agents_information_state(W->get_language()->get_agents_number());
//            del::label l = e->is_ontic() ? update_label(current, e) : del::label{current->get_label_id()};
//            possibility_id updated_id = W->emplace_possibility(possibility{W->get_language(), W->get_possibility_storage(), W->get_information_state_storage(), std::move(l), std::move(is)});
//            update_map[current_id] = updated_id;
//        }

        visited.emplace(current_id);
        // TODO: UNCOMMENT AND FIX
//        for (del::agent ag = 0; ag < W->get_language()->get_agents_number(); ++ag)
//            for (const possibility_id &w: current->get_information_state(ag))
//                if (not o_ags[ag]) {
//                    if (visited.find(w) == visited.end())
//                        to_visit.emplace(w);
//                } else {
//                    if (visited.find(w) == visited.end()) {
//                        update_map[w] = w;
//                        visited.emplace(w);
//                    }
//                }

        to_visit.erase(first);
    }
}

void union_updater::update_information_states(const delphic::possibility_spectrum_ptr &W,
                                              const del::agent_set &o_ags, delphic::possibility_map &update_map) {
    std::vector<information_state_map> agents_is_map(W->get_language()->get_agents_number());
    std::vector<information_state_id_map> agents_is_id_map(W->get_language()->get_agents_number());

    // TODO: UNCOMMENT AND FIX
//    for (auto &[old_w, new_w] : update_map)
//        if (new_w and new_w != old_w) {
//            for (del::agent ag = 0; ag < W->get_language()->get_agents_number(); ++ag)
//                if (o_ags[ag])
//                    agents_is_id_map[ag][new_w] = W->get(old_w)->get_information_state_id(ag);
//                else {
//                    for (const possibility_id &old_v: W->get(old_w)->get_information_state(ag)) {
//                        auto new_v = update_map.find(old_v);
//
//                        if (new_v != update_map.end() and new_v->second != old_v) {
////                        if (auto new_v = update_map[old_v]; new_v and new_v != old_v) {
//                            if (agents_is_map[ag].find(new_w) == agents_is_map[ag].end())
//                                agents_is_map[ag][new_w] = information_state{new_v->second};
//                            else
//                                agents_is_map[ag][new_w].emplace(new_v->second);
//                        }
//                    }
//                }
//        }

//    for (auto &[old_w, new_w] : update_map)
//        if (new_w and new_w != old_w) {
//            for (del::agent ag = 0; ag < W->get_language()->get_agents_number(); ++ag)
//                if (o_ags[ag])
//                    W->get(new_w)->set_information_state(ag, agents_is_id_map[ag][new_w]);
//                else {
//                    information_state_id id = W->emplace_information_state(std::move(agents_is_map[ag][new_w]));
//                    W->get(new_w)->set_information_state(ag, id);
//                }
//        }
}

void union_updater::update_information_states(const delphic::possibility_spectrum_ptr &W, const del::agent_set &po_ags,
                                              const del::agent_set &o_ags, const del::formula_ptr &f_pre, delphic::possibility_map &update_map) {
    std::vector<information_state_map> agents_is_map(W->get_language()->get_agents_number());
    std::vector<information_state_id_map> agents_is_id_map(W->get_language()->get_agents_number());

    // TODO: UNCOMMENT AND FIX
//    for (auto &[old_w, new_w] : update_map)
//        if (new_w and new_w != old_w) {
//            for (del::agent ag = 0; ag < W->get_language()->get_agents_number(); ++ag)
//                if (o_ags[ag])
//                    agents_is_id_map[ag][new_w] = W->get(old_w)->get_information_state_id(ag);
//                else {        // if (obs_ags[ag])
//                    for (const possibility_id &old_v: W->get(old_w)->get_information_state(ag)) {
//                        bool good = po_ags[ag] or
//                                W->get(old_w)->satisfies(f_pre) == W->get(old_v)->satisfies(f_pre);
//
//                        if (auto new_v = update_map[old_v]; new_v and good) {
//                            if (agents_is_map[ag].find(new_w) == agents_is_map[ag].end())
//                                agents_is_map[ag][new_w] = information_state{new_v};
//                            else
//                                agents_is_map[ag][new_w].emplace(new_v);
//                        }
//                    }
//                }/* else {
//                    agents_is_map[ag][new_w] = information_state{new_w};
//                }*/
//        }
//
//    for (auto &[old_w, new_w] : update_map)
//        if (new_w and new_w != old_w) {
//            for (del::agent ag = 0; ag < W->get_language()->get_agents_number(); ++ag)
//                if (o_ags[ag])
//                    W->get(new_w)->set_information_state(ag, agents_is_id_map[ag][new_w]);
//                else {
//                    information_state_id id = W->emplace_information_state(std::move(agents_is_map[ag][new_w]));
//                    W->get(new_w)->set_information_state(ag, id);
//                }
//        }
}

void union_updater::update_designated(const delphic::possibility_spectrum_ptr &W, information_state &designated, delphic::possibility_map &update_map) {
    for (const possibility_id &w : W->get_designated_possibilities())
        if (update_map[w])
            designated.emplace(update_map[w]);
}

del::label union_updater::update_label(const delphic::possibility_ptr &w, const delphic::eventuality_ptr &e) {
    // TODO: UNCOMMENT AND FIX
//    auto bitset = w->get_label_id().get_bitset();
//
//    for (const auto &[p, post] : e->get_postconditions())
//        bitset[p] = model_checker::holds_in(*w, *post);
//
//    return del::label{std::move(bitset)};
}

/*void union_updater::bounded_update_helper(const possibility_ptr &w, const eventuality_ptr &e, unsigned long b,
                                          std::vector<possibility_map> &updated_possibilities, storage<possibility> &possibility_storage) {
    agents_information_state ws = agents_information_state(w->get_language()->get_agents_number());

    if (w->get_bound() > 0)
        for (del::agent ag = 0; ag < w->get_language()->get_agents_number(); ++ag)
            for (const possibility_ptr &v : w->get_information_state(ag))
                for (const eventuality_ptr &f : e->get_information_state(ag)) {
                if (not updated_possibilities[b].at(v))     // We check that we did not already calculate v*f
                    bounded_update_helper(v, f, b-1, updated_possibilities, possibility_storage);

                ws[ag].emplace(updated_possibilities[b].at(v));      // xs[ag] is the set of (b-1)-possibilities v*f
            }

    possibility w_b = possibility{w->get_language(), update_label(w, e), std::move(ws), b};     // We create the b-possibility w_b,
    updated_possibilities[b][w] = possibility_storage.emplace(std::move(w_b));                  // we add it to the storage, and we put it into updated_possibilities[b][w]
}*/
