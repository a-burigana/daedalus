//
// BEP - Bounded Epistemic Planner (MIT License)
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

#include "formula_tester.h"
#include "builder/state_builder.h"
#include "../include/del/formulae/propositional/and_formula.h"
#include "../include/del/formulae/propositional/atom_formula.h"
#include "../include/del/formulae/modal/box_formula.h"
#include "../include/del/formulae/propositional/not_formula.h"
#include "../include/del/formulae/modal/diamond_formula.h"
#include "builder/domains/coin_in_the_box.h"
#include <memory>

using namespace bep::tester;

void formula_tester::test_CB_1() {
    const del::state s = coin_in_the_box::build_initial_state();
    const del::language_ptr l = s.get_language();

    del::formula_ptr heads = std::make_shared<del::atom_formula>(l->get_atom_id("heads"));
    del::formula_ptr looking_a = std::make_shared<del::atom_formula>(l->get_atom_id("looking_a"));
//    del::formula_ptr has_key_a = std::make_shared<del::atom_formula>(l->get_atom_id("has_key_a"));

    del::formula_deque fs = {heads, looking_a};     // , has_key_a};
    del::formula_ptr f = std::make_shared<del::and_formula>(std::move(fs));

    assert(f->holds_in(s, 0));
    assert(not f->holds_in(s, 1));
}

void formula_tester::test_CB_2() {
    const del::state s = coin_in_the_box::build_initial_state();
    const del::language_ptr l = s.get_language();
    const del::agent a = l->get_agent_id("a");

    del::formula_ptr opened         = std::make_shared<del::atom_formula>(l->get_atom_id("opened"));
    del::formula_ptr not_opened     = std::make_shared<del::not_formula>(opened);
    del::formula_ptr K_a_opened     = std::make_shared<del::box_formula>(a, opened);
    del::formula_ptr K_a_not_opened = std::make_shared<del::box_formula>(a, not_opened);

    assert(s.satisfies(K_a_not_opened));
    assert(not s.satisfies(K_a_opened));
    assert(K_a_not_opened->holds_in(s, 1));
}

void formula_tester::test_CB_3() {
    const del::state s = coin_in_the_box::build_initial_state();
    const del::language_ptr l = s.get_language();
    const del::agent a = l->get_agent_id("a"), b = l->get_agent_id("b"), c = l->get_agent_id("c");

    del::formula_ptr opened                 = std::make_shared<del::atom_formula>(l->get_atom_id("opened"));
    del::formula_ptr not_opened             = std::make_shared<del::not_formula>(opened);
    del::formula_ptr K_a_not_opened         = std::make_shared<del::box_formula>(a, not_opened);
    del::formula_ptr K_b_K_a_not_opened     = std::make_shared<del::box_formula>(b, K_a_not_opened);
    del::formula_ptr K_c_K_b_K_a_not_opened = std::make_shared<del::box_formula>(c, K_b_K_a_not_opened);

    assert(s.satisfies(K_c_K_b_K_a_not_opened));
    assert(K_c_K_b_K_a_not_opened->holds_in(s, 1));
}
