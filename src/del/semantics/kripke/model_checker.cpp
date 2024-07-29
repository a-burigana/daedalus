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

#include "../../../../include/del/semantics/kripke/model_checker.h"

using namespace kripke;

bool model_checker::holds_in(const state &s, world_id w, const del::formula &f) {
    switch (f.get_type()) {
        case del::formula_type::true_formula:
            return true;
        case del::formula_type::false_formula:
            return false;
        case del::formula_type::atom_formula:
            return model_checker::holds_in(s, w, dynamic_cast<const del::atom_formula &>(f));
        case del::formula_type::not_formula:
            return model_checker::holds_in(s, w, dynamic_cast<const del::not_formula &>(f));
        case del::formula_type::and_formula:
            return model_checker::holds_in(s, w, dynamic_cast<const del::and_formula &>(f));
        case del::formula_type::or_formula:
            return model_checker::holds_in(s, w, dynamic_cast<const del::or_formula &>(f));
        case del::formula_type::imply_formula:
            return model_checker::holds_in(s, w, dynamic_cast<const del::imply_formula &>(f));
        case del::formula_type::box_formula:
            return model_checker::holds_in(s, w, dynamic_cast<const del::box_formula &>(f));
        case del::formula_type::diamond_formula:
            return model_checker::holds_in(s, w, dynamic_cast<const del::diamond_formula &>(f));
    }
}

bool model_checker::holds_in(const state &s, world_id w, const del::atom_formula &f) {
    return s.get_label(w)[f.get_atom()];
}

bool model_checker::holds_in(const state &s, world_id w, const del::not_formula &f) {
    return not model_checker::holds_in(s, w, *f.get_f());
}

bool model_checker::holds_in(const state &s, world_id w, const del::and_formula &f) {
    auto check = [&](const del::formula_ptr &f) { return model_checker::holds_in(s, w, *f); };
    return std::all_of(f.get_fs().begin(), f.get_fs().end(), check);
}

bool model_checker::holds_in(const state &s, world_id w, const del::or_formula &f) {
    auto check = [&](const del::formula_ptr &f) { return model_checker::holds_in(s, w, *f); };
    return std::any_of(f.get_fs().begin(), f.get_fs().end(), check);
}

bool model_checker::holds_in(const state &s, world_id w, const del::imply_formula &f) {
    return not model_checker::holds_in(s, w, *f.get_f1()) or model_checker::holds_in(s, w, *f.get_f2());
}

bool model_checker::holds_in(const state &s, world_id w, const del::box_formula &f) {
    const auto &worlds = s.get_agent_possible_worlds(f.get_ag(), w);
    return std::all_of(worlds.begin(), worlds.end(),
                       [&](const world_id &v) { return model_checker::holds_in(s, v, *f.get_f()); });
}

bool model_checker::holds_in(const state &s, world_id w, const del::diamond_formula &f) {
    const auto &worlds = s.get_agent_possible_worlds(f.get_ag(), w);
    return std::any_of(worlds.begin(), worlds.end(),
                       [&](const world_id &v) { return model_checker::holds_in(s, v, *f.get_f()); });
}
