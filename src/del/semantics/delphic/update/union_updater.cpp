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
#include "../../../../../include/del/semantics/delphic/model_checker.h"
#include "../../../../utils/storage.cpp"
#include <algorithm>

using namespace delphic;

bool union_updater::is_applicable(const possibility &w, const eventuality &e, const storage<possibility> &storage) {
    return model_checker::holds_in(w, *e.get_pre(), storage);
}

bool union_updater::is_applicable(const possibility_spectrum &W, const eventuality_spectrum &E,
                                  const storage <possibility> &storage) {
    return std::all_of(W.begin(), W.end(),
        [&](const possibility_id w) {
            return std::any_of(E.begin(), E.end(),
                [&](const eventuality_ptr &e) {
                    return model_checker::holds_in(*storage.get(w), *e->get_pre(), storage);
            });
    });
}

possibility union_updater::update(const possibility &w, const eventuality &e, const storage<possibility> &storage) {
}

possibility union_updater::update(const possibility_spectrum &w, const eventuality_spectrum &e, const storage<possibility> &storage) {
}

possibility union_updater::bounded_update(const possibility &w, const eventuality &e, const storage<possibility> &storage) {
}

possibility union_updater::bounded_update(const possibility_spectrum &w, const eventuality_spectrum &e, const storage<possibility> &storage) {
}
