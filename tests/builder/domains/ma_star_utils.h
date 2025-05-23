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

#ifndef DAEDALUS_MA_STAR_UTILS_H
#define DAEDALUS_MA_STAR_UTILS_H

#include <fstream>
#include "../../../include/search/planning_task.h"
#include "../../../include/del/formulas/propositional/atom_formula.h"
#include "../../../include/del/formulas/propositional/not_formula.h"
#include "../../../include/del/formulas/propositional/and_formula.h"
#include "../../../include/del/formulas/propositional/or_formula.h"
#include "../../../include/del/formulas/propositional/imply_formula.h"
#include "../../../include/del/formulas/modal/box_formula.h"
#include "../../../include/del/formulas/modal/diamond_formula.h"

class ma_star_utils {
public:
    static void print_atoms(std::ofstream &out, const search::planning_task &task);
    static void print_agents(std::ofstream &out, const search::planning_task &task);
    static void print_action_names(std::ofstream &out, const search::planning_task &task);

    static void print_formula(std::ofstream &out, const del::language_ptr &lang, const del::formula_ptr &f);

    static void print_atom_formula(std::ofstream &out, const del::language_ptr &lang, const del::atom_formula &f);
    static void print_not_formula(std::ofstream &out, const del::language_ptr &lang, const del::not_formula &f);
    static void print_and_formula(std::ofstream &out, const del::language_ptr &lang, const del::and_formula &f);
    static void print_or_formula(std::ofstream &out, const del::language_ptr &lang, const del::or_formula &f);
    static void print_imply_formula(std::ofstream &out, const del::language_ptr &lang, const del::imply_formula &f);
    static void print_box_formula(std::ofstream &out, const del::language_ptr &lang, const del::box_formula &f);
    static void print_diamond_formula(std::ofstream &out, const del::language_ptr &lang, const del::diamond_formula &f);

//    static void print_action(std::ofstream &out, const kripke::action_ptr &act);
//
//    static void print_public_ontic(std::ofstream &out, const kripke::action_ptr &act);
//    static void print_private_ontic(std::ofstream &out, const kripke::action_ptr &act);
//    static void print_semi_private_sensing(std::ofstream &out, const kripke::action_ptr &act);
//    static void print_public_sensing(std::ofstream &out, const kripke::action_ptr &act);
//    static void print_private_announcement(std::ofstream &out, const kripke::action_ptr &act);
//    static void print_semi_private_announcement(std::ofstream &out, const kripke::action_ptr &act);
//    static void print_public_announcement(std::ofstream &out, const kripke::action_ptr &act);
};


#endif //DAEDALUS_MA_STAR_UTILS_H
