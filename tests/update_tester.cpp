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

#include "update_tester.h"
#include "builder/action_builder.h"
#include "builder/state_builder.h"
#include "../include/del/formulas/propositional/atom_formula.h"
#include "../include/del/semantics/kripke/update/updater.h"
#include "../include/del/formulas/propositional/not_formula.h"
#include "../include/del/formulas/modal/box_formula.h"
#include "../include/del/formulas/propositional/and_formula.h"
#include "printer.h"
#include "../include/del/formulas/propositional/true_formula.h"
#include "builder/domains/coin_in_the_box.h"

using namespace daedalus::tester;
using namespace kripke;

state update_tester::test_CB_1(const std::string &out_path, const del::label_storage_ptr &l_storage, bool print) {
    std::string cb_dir = "coin_in_the_box/";

    state s0 = coin_in_the_box::build_initial_state(l_storage);
    del::language_ptr l = s0.get_language();

    del::formula_ptr heads = std::make_shared<del::atom_formula>(l->get_atom_id("heads"));
    action act = action_builder::build_public_announcement("shout_heads", l, heads);

    state s_cb_heads = updater::product_update(s0, act, l_storage);
    printer::print_state(s_cb_heads, l_storage, out_path + cb_dir, "cb_heads");

    return s_cb_heads;
}

state update_tester::test_CB_2(const std::string &out_path, const del::label_storage_ptr &l_storage, bool print) {
    std::string cb_dir = "coin_in_the_box/";

    state s0 = coin_in_the_box::build_initial_state(l_storage);
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

    action act = coin_in_the_box::build_open(a, fo_ags);

    state s_cb_open = updater::product_update(s0, act, l_storage);
    printer::print_state(s_cb_open, l_storage, out_path + cb_dir, "cb_open");

    return s_cb_open;
}

state update_tester::test_CB_3(const std::string &out_path, const del::label_storage_ptr &l_storage, bool print) {
    std::string cb_dir = "coin_in_the_box/";

    state s_open = daedalus::tester::update_tester::test_CB_2(out_path, l_storage, false);
    del::language_ptr l = s_open.get_language();

    const del::agent a = l->get_agent_id("a"), b = l->get_agent_id("b");
    del::agent_set fo_ags(l->get_agents_number()), po_ags(l->get_agents_number());
    fo_ags.set(a);
    fo_ags.set(b);

    del::formula_ptr heads      = std::make_shared<del::atom_formula>(l->get_atom_id("heads"));
    del::formula_ptr opened     = std::make_shared<del::atom_formula>(l->get_atom_id("opened"));
    del::formula_ptr K_a_opened = std::make_shared<del::box_formula>(a, opened);

    action act = action_builder::build_semi_private_sensing("peek_a", l, K_a_opened, heads, fo_ags, po_ags);

    state s_cb_open_peek = kripke::updater::product_update(s_open, act, l_storage);
    printer::print_state(s_cb_open_peek, l_storage, out_path + cb_dir, "cb_open_peek");

    return s_cb_open_peek;
}
