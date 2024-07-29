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

#include "../../../../include/del/semantics/delphic/model_checker.h"
#include "../../../utils/storage.cpp"

using namespace delphic;

bool model_checker::holds_in(const possibility &w, const del::formula &f, const storage<possibility> &storage) {
    switch (f.get_type()) {
        case del::formula_type::true_formula:
            return true;
        case del::formula_type::false_formula:
            return false;
        case del::formula_type::atom_formula:
            return model_checker::holds_in(w, dynamic_cast<const del::atom_formula &>(f), storage);
        case del::formula_type::not_formula:
            return model_checker::holds_in(w, dynamic_cast<const del::not_formula &>(f), storage);
        case del::formula_type::and_formula:
            return model_checker::holds_in(w, dynamic_cast<const del::and_formula &>(f), storage);
        case del::formula_type::or_formula:
            return model_checker::holds_in(w, dynamic_cast<const del::or_formula &>(f), storage);
        case del::formula_type::imply_formula:
            return model_checker::holds_in(w, dynamic_cast<const del::imply_formula &>(f), storage);
        case del::formula_type::box_formula:
            return model_checker::holds_in(w, dynamic_cast<const del::box_formula &>(f), storage);
        case del::formula_type::diamond_formula:
            return model_checker::holds_in(w, dynamic_cast<const del::diamond_formula &>(f), storage);
    }
}

bool model_checker::holds_in(const possibility &w, const del::atom_formula &f, const storage<possibility> &storage) {
    return w.get_label()[f.get_atom()];
}

bool model_checker::holds_in(const possibility &w, const del::not_formula &f, const storage<possibility> &storage) {
    return not model_checker::holds_in(w, *f.get_f(), storage);
}

bool model_checker::holds_in(const possibility &w, const del::and_formula &f, const storage<possibility> &storage) {
    auto check = [&](const del::formula_ptr &f) { return model_checker::holds_in(w, *f, storage); };
    return std::all_of(f.get_fs().begin(), f.get_fs().end(), check);
}

bool model_checker::holds_in(const possibility &w, const del::or_formula &f, const storage<possibility> &storage) {
    auto check = [&](const del::formula_ptr &f) { return model_checker::holds_in(w, *f, storage); };
    return std::any_of(f.get_fs().begin(), f.get_fs().end(), check);
}

bool model_checker::holds_in(const possibility &w, const del::imply_formula &f, const storage<possibility> &storage) {
    return not model_checker::holds_in(w, *f.get_f1(), storage) or model_checker::holds_in(w, *f.get_f2(), storage);
}

bool model_checker::holds_in(const possibility &w, const del::box_formula &f, const storage<possibility> &storage) {
    const auto &w_ag = w.get_information_state(f.get_ag());
    return std::all_of(w_ag.begin(), w_ag.end(),
        [&](const possibility_id v) { return model_checker::holds_in(*storage.get(v), *f.get_f(), storage); });
}

bool model_checker::holds_in(const possibility &w, const del::diamond_formula &f, const storage<possibility> &storage) {
    const auto &w_ag = w.get_information_state(f.get_ag());
    return std::any_of(w_ag.begin(), w_ag.end(),
        [&](const possibility_id v) { return model_checker::holds_in(*storage.get(v), *f.get_f(), storage); });
}
