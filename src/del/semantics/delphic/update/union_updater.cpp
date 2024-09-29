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
#include "../../../../../include/del/semantics/delphic/states/possibility.h"
#include "../../../../../include/del/semantics/delphic/actions/eventuality.h"
#include "../../../../../include/del/semantics/delphic/model_checker.h"
#include "../../../../utils/storage.cpp"
#include <algorithm>
#include <unordered_map>

using namespace delphic;

bool union_updater::is_applicable(const possibility_ptr &w, const eventuality_ptr &e) {
    return model_checker::holds_in(*w, *e->get_pre());
}

bool union_updater::is_applicable(const possibility_spectrum_ptr &W, const eventuality_spectrum_ptr &E) {
    return std::all_of(W->get_designated_possibilities().begin(), W->get_designated_possibilities().end(),
        [&](const possibility_ptr &w) {
            return std::any_of(E->get_designated_eventualities().begin(), E->get_designated_eventualities().end(),
                [&](const eventuality_ptr &e) {
                    return model_checker::holds_in(*w, *e->get_pre());
            });
    });
}

/*possibility union_updater::update(const possibility &w, const eventuality &e, storage<possibility> &possibility_storage) {
}

possibility union_updater::update(const possibility_spectrum &w, const eventuality_spectrum &e, storage<possibility> &possibility_storage) {
}*/

possibility_spectrum_ptr union_updater::bounded_update(const delphic::possibility_spectrum_ptr &W, const delphic::eventuality_spectrum_ptr &E,
                                                       const unsigned long b, storage<delphic::possibility> &possibility_storage) {
    auto updated_possibilities = std::vector<possibility_map>(b+1);

    for (const possibility_ptr &w : W->get_designated_possibilities())
        for (const eventuality_ptr &e : E->get_designated_eventualities())
            if (is_applicable(w, e))
                bounded_update_helper(w, e, b, updated_possibilities, possibility_storage);

    information_state W_;

    for (auto &[_, w] : updated_possibilities[b])
        W_.emplace(std::move(w));

    return std::make_shared<possibility_spectrum>(W->get_language(), std::move(W_));
}

possibility_ptr union_updater::bounded_update(const possibility_ptr &w, const eventuality_ptr &e, const unsigned long b,
                                              storage<possibility> &possibility_storage) {


//    signature sign_x_h = signature{s.get_language(), std::make_shared<label>(s.get_label(x)), std::move(xs)};       // We create the h-signature of x (h being x's bound),
//    worlds_signatures[h][x] = signatures_storage.emplace(std::move(sign_x_h));
}

void union_updater::bounded_update_helper(const possibility_ptr &w, const eventuality_ptr &e, unsigned long b,
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
}

kripke::label union_updater::update_label(const delphic::possibility_ptr &w, const delphic::eventuality_ptr &e) {
    kripke::label l = w->get_label();

    for (const auto &[p, post] : e->get_postconditions())
        l.update(p, model_checker::holds_in(*w, *post));

    return std::move(l);
}

/*
possibility union_updater::bounded_update(const possibility_spectrum &w, const eventuality_spectrum &e, storage<possibility> &storage) {
}
*/
