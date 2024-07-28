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

#include "update_tester.h"
#include "builder/action_builder.h"
#include "builder/state_builder.h"
#include "../include/del/formulae/propositional/atom_formula.h"
#include "../include/del/update/updater.h"
#include "../include/del/formulae/propositional/not_formula.h"
#include "../include/del/formulae/modal/box_formula.h"
#include "../include/del/formulae/propositional/and_formula.h"
#include "printer.h"
#include "../include/del/formulae/propositional/true_formula.h"
#include "builder/domains/coin_in_the_box.h"

using namespace bep::tester;

del::state update_tester::test_CB_1(const std::string &out_path, bool print) {
    std::string cb_dir = "coin_in_the_box/";

    del::state s0 = coin_in_the_box::build_initial_state();
    del::language_ptr l = s0.get_language();

    del::formula_ptr heads = std::make_shared<del::atom_formula>(l->get_atom_id("heads"));
    del::action act = action_builder::build_public_announcement("shout_heads", l, heads);

    del::state s_cb_heads = del::updater::product_update(s0, act);
    printer::print_state(s_cb_heads, out_path + cb_dir, "cb_heads");

    return s_cb_heads;
}

del::state update_tester::test_CB_2(const std::string &out_path, bool print) {
    std::string cb_dir = "coin_in_the_box/";

    del::state s0 = coin_in_the_box::build_initial_state();
    del::language_ptr l = s0.get_language();

    const del::agent a = l->get_agent_id("a"), b = l->get_agent_id("b");
    del::agent_set fo_ags(l->get_agents_number());
    fo_ags.set(a);
    fo_ags.set(b);

    del::formula_ptr opened         = std::make_shared<del::atom_formula>(l->get_atom_id("opened"));
    del::formula_ptr not_opened     = std::make_shared<del::not_formula>(opened);
    del::formula_ptr K_a_not_opened = std::make_shared<del::box_formula>(a, not_opened);
    del::formula_ptr f_post         = std::make_shared<del::true_formula>();
//    del::formula_ptr has_key_a      = std::make_shared<del::atom_formula>(l->get_atom_id("has_key_a"));

    del::action act = coin_in_the_box::build_open(a, fo_ags);

    del::state s_cb_open = del::updater::product_update(s0, act);
    printer::print_state(s_cb_open, out_path + cb_dir, "cb_open");

    return s_cb_open;
}

del::state update_tester::test_CB_3(const std::string &out_path, bool print) {
    std::string cb_dir = "coin_in_the_box/";

    del::state s_open = bep::tester::update_tester::test_CB_2(out_path, false);
    del::language_ptr l = s_open.get_language();

    const del::agent a = l->get_agent_id("a"), b = l->get_agent_id("b");
    del::agent_set fo_ags(l->get_agents_number()), po_ags(l->get_agents_number());
    fo_ags.set(a);
    fo_ags.set(b);

    del::formula_ptr heads      = std::make_shared<del::atom_formula>(l->get_atom_id("heads"));
    del::formula_ptr opened     = std::make_shared<del::atom_formula>(l->get_atom_id("opened"));
    del::formula_ptr K_a_opened = std::make_shared<del::box_formula>(a, opened);

    del::action act = action_builder::build_semi_private_sensing("peek_a", l, K_a_opened, heads, fo_ags, po_ags);

    del::state s_cb_open_peek = del::updater::product_update(s_open, act);
    printer::print_state(s_cb_open_peek, out_path + cb_dir, "cb_open_peek");

    return s_cb_open_peek;
}
