// MIT License
//
// Copyright (c) 2022 Alessandro Burigana and Francesco Fabiano
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

#include "ma_star_utils.h"

void ma_star_utils::print_atoms(std::ofstream &out, const search::planning_task &task) {
    out << "fluent ";

    for (del::atom p = 0; p < task.get_language()->get_atoms_number(); ++p)
        out << task.get_language()->get_atom_name(p) << (p+1 < task.get_language()->get_atoms_number() ? ", " : ";\n");
}

void ma_star_utils::print_agents(std::ofstream &out, const search::planning_task &task) {
    out << "agent ";

    for (del::agent ag = 0; ag < task.get_language()->get_agents_number(); ++ag)
        out << task.get_language()->get_agent_name(ag) << (ag+1 < task.get_language()->get_agents_number() ? ", " : ";\n");
}

void ma_star_utils::print_action_names(std::ofstream &out, const search::planning_task &task) {
    out << "action ";

    for (const auto &act : task.get_actions())
        out << act->get_name() << (act != task.get_actions().back() ? ", " : ";\n");
}

void ma_star_utils::print_formula(std::ofstream &out, const del::language_ptr &lang, const del::formula_ptr &f) {
    switch (f->get_type()) {
        case del::formula_type::true_formula:
            out << "true";
            break;
        case del::formula_type::false_formula:
            out << "false";
            break;
        case del::formula_type::atom_formula:
            print_atom_formula(out, lang, dynamic_cast<const del::atom_formula &>(*f));
            break;
        case del::formula_type::not_formula:
            print_not_formula(out, lang, dynamic_cast<const del::not_formula &>(*f));
            break;
        case del::formula_type::and_formula:
            print_and_formula(out, lang, dynamic_cast<const del::and_formula &>(*f));
            break;
        case del::formula_type::or_formula:
            print_or_formula(out, lang, dynamic_cast<const del::or_formula &>(*f));
            break;
        case del::formula_type::imply_formula:
            print_imply_formula(out, lang, dynamic_cast<const del::imply_formula &>(*f));
            break;
        case del::formula_type::box_formula:
            print_box_formula(out, lang, dynamic_cast<const del::box_formula &>(*f));
            break;
        case del::formula_type::diamond_formula:
            print_diamond_formula(out, lang, dynamic_cast<const del::diamond_formula &>(*f));
            break;
    }
}

void ma_star_utils::print_atom_formula(std::ofstream &out, const del::language_ptr &lang, const del::atom_formula &f) {
    out << lang->get_atom_name(f.get_atom());
}

void ma_star_utils::print_not_formula(std::ofstream &out, const del::language_ptr &lang, const del::not_formula &f) {
    out << " ( - ( ";
    print_formula(out, lang, f.get_f());
    out << " ) ) ";
}

void ma_star_utils::print_and_formula(std::ofstream &out, const del::language_ptr &lang, const del::and_formula &f) {
    out << " ( ";

    for (const auto &f_ : f.get_fs()) {
        print_formula(out, lang, f_);

        if (f_ != f.get_fs().back())
            out << " , ";
    }

    out << " ) ";
}

void ma_star_utils::print_or_formula(std::ofstream &out, const del::language_ptr &lang, const del::or_formula &f) {
    out << " ( ";

    for (const auto &f_ : f.get_fs()) {
        print_formula(out, lang, f_);

        if (f_ != f.get_fs().back())
            out << " | ";
    }

    out << " ) ";
}

void ma_star_utils::print_imply_formula(std::ofstream &out, const del::language_ptr &lang, const del::imply_formula &f) {
    out << " ( - ( ";
    print_formula(out, lang, f.get_f1());
    out << " ) | ( ";
    print_formula(out, lang, f.get_f2());
    out << " ) ) ";
}

void ma_star_utils::print_box_formula(std::ofstream &out, const del::language_ptr &lang, const del::box_formula &f) {
    out << " ( B( " << lang->get_agent_name(f.get_ag()) << " , ";
    print_formula(out, lang, f.get_f());
    out << " ) ) ";
}

void ma_star_utils::print_diamond_formula(std::ofstream &out, const del::language_ptr &lang, const del::diamond_formula &f) {
    out << " ( - ( B( " << lang->get_agent_name(f.get_ag()) << " , ";
    out << " ( - ( ";
    print_formula(out, lang, f.get_f());
    out << " ) ) ) ) ) ";
}

//void ma_star_utils::print_action(std::ofstream &out, const kripke::action_ptr &act) {
//    switch (act->get_type()) {
//        case kripke::action_type::public_ontic:
//            print_public_ontic(out, act);
//            break;
//        case kripke::action_type::private_ontic:
//            print_private_ontic(out, act);
//            break;
//        case kripke::action_type::semi_private_sensing:
//            print_semi_private_sensing(out, act);
//            break;
//        case kripke::action_type::public_sensing:
//            print_public_sensing(out, act);
//            break;
//        case kripke::action_type::private_announcement:
//            print_private_announcement(out, act);
//            break;
//        case kripke::action_type::semi_private_announcement:
//            print_semi_private_announcement(out, act);
//            break;
//        case kripke::action_type::public_announcement:
//            print_public_announcement(out, act);
//            break;
//    }
//}
//
//void ma_star_utils::print_public_announcement(std::ofstream &out, const kripke::action_ptr &act) {
//    out << "executable " << act->get_name();
//
////    if (act->get_precondition(0))
//}
