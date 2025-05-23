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

#include "tiger.h"

#include "domain_utils.h"
#include "../action_builder.h"
#include "../../../include/del/formulas/propositional/atom_formula.h"
#include "../../../include/del/formulas/propositional/not_formula.h"
#include "../../../include/del/formulas/propositional/false_formula.h"
#include "../../../include/del/formulas/propositional/true_formula.h"
#include "../../../include/del/formulas/modal/box_formula.h"
#include "../../../include/del/formulas/propositional/and_formula.h"
#include "ma_star_utils.h"
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>

using namespace daedalus::tester;
using namespace del;

std::string tiger::get_name() {
    return "tiger";
}

del::language_ptr tiger::build_language(unsigned long doors_no, unsigned long tigers_no) {
    name_vector atom_names(4 * doors_no + 1), agent_names = {"knight"};

    for (unsigned long i = 0; i < doors_no; ++i) {
        atom_names[               i] = "knight_"   + std::to_string(i+1);
        atom_names[    doors_no + i] = "princess_" + std::to_string(i+1);
        atom_names[2 * doors_no + i] = "tiger_"    + std::to_string(i+1);
        atom_names[3 * doors_no + i] = "open_"     + std::to_string(i+1);
    }

    atom_names[4 * doors_no] = "saved_princess";

    return std::make_shared<language>(std::move(language{atom_names, agent_names}));
}

kripke::state tiger::build_initial_state(unsigned long doors_no, unsigned long tigers_no, label_storage &l_storage) {
    language_ptr language = tiger::build_language(doors_no, tigers_no);

    unsigned long worlds_number = doors_no * domain_utils::binom(doors_no - 1, tigers_no), count = 0;
    relations r(language->get_agents_number());
    label_vector ls = label_vector(worlds_number);

    for (unsigned long i = 0; i < doors_no; ++i) {
        boost::dynamic_bitset<> filter(doors_no);
        filter[i].flip();

        auto room_combinations = domain_utils::all_sized_combinations_without(doors_no, tigers_no, filter);
        atom knight_1 = language->get_atom_id("knight_1");
        atom princess_i = language->get_atom_id("princess_" + std::to_string(i + 1));

        for (const auto &combination: room_combinations) {
            boost::dynamic_bitset<> bw(language->get_atoms_number());

            bw[knight_1] = true;
            bw[princess_i] = true;

            for (unsigned long j = 0; j < doors_no; ++j)
                if (combination[j]) bw[language->get_atom_id("tiger_" + std::to_string(j + 1))] = true;

            label lw = label{std::move(bw)};
            ls[count++] = l_storage.emplace(std::move(lw));
        }
    }

    boost::dynamic_bitset<> all_worlds(worlds_number);
    all_worlds.set();

    for (agent ag = 0; ag < language->get_agents_number(); ++ag) {
        r[ag] = agent_relation(worlds_number);

        for (world_id w = 0; w < worlds_number; ++w)
            r[ag][w] = world_bitset(all_worlds);
    }

    world_bitset designated_worlds = world_bitset{worlds_number, world_set{worlds_number - 1}};

    return state{language, worlds_number, std::move(r), std::move(ls), std::move(designated_worlds)};
}

kripke::action_deque tiger::build_actions(unsigned long doors_no, unsigned long tigers_no) {
    language_ptr language = tiger::build_language(doors_no, tigers_no);
    action_deque actions;

    for (unsigned long door = 0; door < doors_no; ++door) {
        actions.push_back(std::make_shared<action>(build_listen(doors_no, tigers_no, door+1)));
        actions.push_back(std::make_shared<action>(build_open(doors_no, tigers_no, door+1)));
        actions.push_back(std::make_shared<action>(build_look(doors_no, tigers_no, door+1)));
        actions.push_back(std::make_shared<action>(build_save_princess(doors_no, tigers_no, door+1)));
    }

    actions.push_back(std::make_shared<action>(build_left(doors_no, tigers_no)));
    actions.push_back(std::make_shared<action>(build_right(doors_no, tigers_no)));

    return actions;
}

search::planning_task tiger::build_task(unsigned long doors_no, unsigned long tigers_no, label_storage &l_storage) {
    std::string name = tiger::get_name();
    std::string id = std::to_string(doors_no) + "_" + std::to_string(tigers_no);

    language_ptr language = tiger::build_language(doors_no, tigers_no);
    state s0 = tiger::build_initial_state(doors_no, tigers_no, l_storage);

    action_deque actions = tiger::build_actions(doors_no, tigers_no);
    formula_ptr saved_princess = std::make_shared<atom_formula>(language->get_atom_id("saved_princess"));

    formula_deque fs;
    const label &l = *l_storage.get(s0.get_label_id(s0.get_worlds_number() - 1));

    for (unsigned long door = 0; door < doors_no; ++door) {
        atom tiger_d = language->get_atom_id("tiger_" + std::to_string(door+1));

        if (l[tiger_d])
            fs.push_back(std::make_shared<box_formula>(0, std::make_shared<atom_formula>(tiger_d)));
    }

    fs.push_back(std::move(saved_princess));
    formula_ptr goal = std::make_shared<and_formula>(std::move(fs));

    return search::planning_task{std::move(name), std::move(id), language, std::move(s0), std::move(actions), std::move(goal)};
}

std::vector<search::planning_task> tiger::build_tasks(label_storage &l_storage) {
    const unsigned long N_MIN_DOORS = 3, N_MAX_DOORS = 4, N_MIN_TIGERS = 1;
    std::vector<search::planning_task> tasks;

    for (unsigned long doors_no = N_MIN_DOORS; doors_no <= N_MAX_DOORS; ++doors_no)
        for (unsigned long tigers_no = N_MIN_TIGERS; tigers_no < doors_no; ++tigers_no)
            tasks.push_back(build_task(doors_no, tigers_no, l_storage));

    return tasks;
}

kripke::action tiger::build_left(const unsigned long doors_no, const unsigned long tigers_no) {
    language_ptr language = tiger::build_language(doors_no, tigers_no);

    formula_ptr knight_1 = std::make_shared<atom_formula>(language->get_atom_id("knight_1"));
    formula_ptr f_pre = std::make_shared<not_formula>(knight_1);
//    formula_ptr f_pre = std::make_shared<box_formula>(0, std::make_shared<not_formula>(knight_1));

    event_post e_post;

    atom knight_n = language->get_atom_id("knight_" + std::to_string(doors_no));
    e_post[knight_n] = std::make_shared<false_formula>();

    for (unsigned long r = 1; r < doors_no; ++r) {
        atom knight_to = language->get_atom_id("knight_" + std::to_string(r));
        atom knight_from = language->get_atom_id("knight_" + std::to_string(r + 1));
        formula_ptr f_post = std::make_shared<atom_formula>(knight_from);
        e_post[knight_to] = std::move(f_post);
    }

    return action_builder::build_public_ontic("left", language, f_pre, e_post);
}

kripke::action tiger::build_right(const unsigned long doors_no, const unsigned long tigers_no) {
    language_ptr language = tiger::build_language(doors_no, tigers_no);

    formula_ptr knight_d = std::make_shared<atom_formula>(language->get_atom_id("knight_" + std::to_string(doors_no)));
    formula_ptr f_pre = std::make_shared<not_formula>(knight_d);
//    formula_ptr f_pre = std::make_shared<box_formula>(0, std::make_shared<not_formula>(knight_d));

    event_post e_post;

    atom knight_1 = language->get_atom_id("knight_1");
    e_post[knight_1] = std::make_shared<false_formula>();

    for (unsigned long r = 1; r < doors_no; ++r) {
        atom knight_to = language->get_atom_id("knight_" + std::to_string(r + 1));
        atom knight_from = language->get_atom_id("knight_" + std::to_string(r));
        formula_ptr f_post = std::make_shared<atom_formula>(knight_from);
        e_post[knight_to] = std::move(f_post);
    }

    return action_builder::build_public_ontic("right", language, f_pre, e_post);
}

kripke::action tiger::build_listen(unsigned long doors_no, unsigned long tigers_no, unsigned long door) {
    language_ptr language = tiger::build_language(doors_no, tigers_no);

    formula_ptr knight_d = std::make_shared<atom_formula>(language->get_atom_id("knight_" + std::to_string(door)));
    formula_ptr tiger_d  = std::make_shared<atom_formula>(language->get_atom_id("tiger_"  + std::to_string(door)));

    formula_ptr K_tiger_d     = std::make_shared<box_formula>(0, tiger_d);
    formula_ptr not_K_tiger_d = std::make_shared<not_formula>(K_tiger_d);

    formula_deque fs = {knight_d, not_K_tiger_d};
    formula_ptr f_pre = std::make_shared<and_formula>(std::move(fs));

    return action_builder::build_public_sensing("listen_" + std::to_string(door), language, f_pre, tiger_d);
}

kripke::action tiger::build_open(unsigned long doors_no, unsigned long tigers_no, unsigned long door) {
    language_ptr language = tiger::build_language(doors_no, tigers_no);

    formula_ptr knight_d = std::make_shared<atom_formula>(language->get_atom_id("knight_" + std::to_string(door)));
    formula_ptr tiger_d  = std::make_shared<atom_formula>(language->get_atom_id("tiger_"  + std::to_string(door)));
    formula_ptr open_d   = std::make_shared<atom_formula>(language->get_atom_id("open_"   + std::to_string(door)));

    formula_ptr not_tiger_d = std::make_shared<not_formula>(tiger_d);
    formula_ptr not_open_d  = std::make_shared<not_formula>(open_d);
    formula_ptr K_not_tiger_d = std::make_shared<box_formula>(0, not_tiger_d);

    formula_ptr f_pre = std::make_shared<and_formula>(formula_deque{knight_d, not_open_d, K_not_tiger_d});

    event_post e_post;
    e_post[language->get_atom_id("open_" + std::to_string(door))] = std::make_shared<true_formula>();

    return action_builder::build_public_ontic("open_" + std::to_string(door), language, f_pre, e_post);
}

kripke::action tiger::build_look(unsigned long doors_no, unsigned long tigers_no, unsigned long door) {
    language_ptr language = tiger::build_language(doors_no, tigers_no);

    formula_ptr knight_d   = std::make_shared<atom_formula>(language->get_atom_id("knight_"   + std::to_string(door)));
    formula_ptr princess_d = std::make_shared<atom_formula>(language->get_atom_id("princess_" + std::to_string(door)));

    formula_ptr K_princess_d     = std::make_shared<box_formula>(0, princess_d);
    formula_ptr not_K_princess_d = std::make_shared<not_formula>(K_princess_d);

    formula_deque fs = {knight_d, not_K_princess_d};
    formula_ptr f_pre = std::make_shared<and_formula>(std::move(fs));

    return action_builder::build_public_sensing("look_" + std::to_string(door), language, f_pre, princess_d);
}

kripke::action tiger::build_save_princess(unsigned long doors_no, unsigned long tigers_no, unsigned long door) {
    language_ptr language = tiger::build_language(doors_no, tigers_no);

    formula_ptr knight_d   = std::make_shared<atom_formula>(language->get_atom_id("knight_"   + std::to_string(door)));
    formula_ptr open_d     = std::make_shared<atom_formula>(language->get_atom_id("open_"     + std::to_string(door)));
    formula_ptr princess_d = std::make_shared<atom_formula>(language->get_atom_id("princess_" + std::to_string(door)));

    formula_ptr K_princess_d = std::make_shared<box_formula>(0, princess_d);
    formula_ptr f_pre = std::make_shared<and_formula>(formula_deque{knight_d, open_d, K_princess_d});

    event_post e_post;
    atom saved_princess  = language->get_atom_id("saved_princess");

    e_post[saved_princess] = std::make_shared<true_formula>();

    return action_builder::build_public_ontic("save_princess_" + std::to_string(door), language, f_pre, e_post);
}

void tiger::write_ma_star_problem(unsigned long doors_no, unsigned long tigers_no,
                                  del::label_storage &l_storage) {
    auto task = build_task(doors_no, tigers_no, l_storage);
    std::string path = "../tests/builder/domains/ma_star/" + task.get_domain_name() + "/";
    std::string name = task.get_domain_name() + "_" + task.get_problem_id();
    std::string ext  = ".txt";

    if (not std::filesystem::exists(path))
        std::filesystem::create_directories(path);

    std::ofstream out = std::ofstream{path + name + ext};

    ma_star_utils::print_atoms(out, task);
    out << "action ";

    for (unsigned long door = 1; door <= doors_no; ++door) {
        out << "listen_" + std::to_string(door) << ", ";
        out << "open_" + std::to_string(door) << ", ";
        out << "look_" + std::to_string(door) << ", ";
        out << "save_" + std::to_string(door) << ", ";
    }

    out << "left" << ", ";
    out << "right" << ";\n";

    ma_star_utils::print_agents(out, task);

    out << std::endl << std::endl;
    std::string ag_name = "knight";

    for (unsigned long door = 1; door <= doors_no; ++door) {
        // listen
        std::string listen_name = "listen_" + std::to_string(door);

        formula_ptr knight_d = std::make_shared<atom_formula>(task.get_language()->get_atom_id("knight_" + std::to_string(door)));
        formula_ptr tiger_d  = std::make_shared<atom_formula>(task.get_language()->get_atom_id("tiger_"  + std::to_string(door)));

        formula_ptr K_tiger_d     = std::make_shared<box_formula>(0, tiger_d);
        formula_ptr not_K_tiger_d = std::make_shared<not_formula>(K_tiger_d);

        formula_deque fs = {not_K_tiger_d, knight_d};
        formula_ptr f_pre_listen = std::make_shared<and_formula>(fs);

        out << "executable " << listen_name << " if ";
        ma_star_utils::print_formula(out, task.get_language(), f_pre_listen);
        out << " ;\n";

        out << listen_name << " determines ";
        ma_star_utils::print_formula(out, task.get_language(), tiger_d);
        out << " ;\n";

        out << ag_name << " observes " << listen_name << ";\n\n";

        // open
        std::string open_name = "open_" + std::to_string(door);

        formula_ptr open_d   = std::make_shared<atom_formula>(task.get_language()->get_atom_id("open_" + std::to_string(door)));

        formula_ptr not_tiger_d = std::make_shared<not_formula>(tiger_d);
        formula_ptr not_open_d  = std::make_shared<not_formula>(open_d);
        formula_ptr K_not_tiger_d = std::make_shared<box_formula>(0, not_tiger_d);

        formula_ptr f_pre_open = std::make_shared<and_formula>(formula_deque{K_not_tiger_d, knight_d, not_open_d});

        out << "executable " << open_name << " if ";
        ma_star_utils::print_formula(out, task.get_language(), f_pre_open);
        out << " ;\n";
        out << open_name << " causes open_" << door << " ;\n";
        out << ag_name << " observes " << open_name << ";\n\n";

        // look
        std::string look_name = "look_" + std::to_string(door);

        formula_ptr princess_d = std::make_shared<atom_formula>(task.get_language()->get_atom_id("princess_" + std::to_string(door)));

        formula_ptr K_princess_d     = std::make_shared<box_formula>(0, princess_d);
        formula_ptr not_K_princess_d = std::make_shared<not_formula>(K_princess_d);

        formula_deque fs2 = {not_K_princess_d, knight_d};
        formula_ptr f_pre_look = std::make_shared<and_formula>(fs2);

        out << "executable " << look_name << " if ";
        ma_star_utils::print_formula(out, task.get_language(), f_pre_look);
        out << " ;\n";

        out << look_name << " determines ";
        ma_star_utils::print_formula(out, task.get_language(), princess_d);
        out << " ;\n";

        out << ag_name << " observes " << look_name << ";\n\n";

        // save_princess
        std::string save_name = "save_" + std::to_string(door);

        formula_ptr f_pre_save = std::make_shared<and_formula>(formula_deque{K_princess_d, knight_d, open_d});

        out << "executable " << save_name << " if ";
        ma_star_utils::print_formula(out, task.get_language(), f_pre_save);
        out << " ;\n";

        out << save_name << " causes saved_princess;\n";
        out << ag_name << " observes " << save_name << ";\n\n";
    }

    // left
    std::string left_name = "left";

    formula_ptr knight_1 = std::make_shared<atom_formula>(task.get_language()->get_atom_id("knight_1"));
    formula_ptr f_pre_left = std::make_shared<not_formula>(knight_1);

    out << "executable " << left_name << " if ";
    ma_star_utils::print_formula(out, task.get_language(), f_pre_left);
    out << " ;" << std::endl;

    for (unsigned long r = 1; r < doors_no; ++r) {
        std::string knight_from = "knight_" + std::to_string(r+1);
        std::string knight_to   = "knight_" + std::to_string(r);

        out << left_name << " causes -" << knight_from << " if " << knight_from << " ;\n";
        out << left_name << " causes  " << knight_to   << " if " << knight_from << " ;\n";
    }

    out << ag_name << " observes " << left_name << ";\n\n";

    // right
    std::string right_name = "right";

    formula_ptr knight_n = std::make_shared<atom_formula>(task.get_language()->get_atom_id("knight_" + std::to_string(doors_no)));
    formula_ptr f_pre_right = std::make_shared<not_formula>(knight_n);

    out << "executable " << right_name << " if ";
    ma_star_utils::print_formula(out, task.get_language(), f_pre_right);
    out << " ;" << std::endl;

    for (unsigned long r = 1; r < doors_no; ++r) {
        std::string knight_from = "knight_" + std::to_string(r);
        std::string knight_to   = "knight_" + std::to_string(r+1);

        out << right_name << " causes -" << knight_from << " if " << knight_from << " ;\n";
        out << right_name << " causes  " << knight_to   << " if " << knight_from << " ;\n";
    }

    out << ag_name << " observes " << right_name << ";\n\n";

    out << "\ninitially knight_1, ";

    for (auto t = 1; t <= tigers_no; ++t)
        out << "tiger_" << t << ", ";

    out << "princess_" << doors_no << ";\n";
    out << "initially C( [knight] , knight_1 );\n";

    formula_deque fs;

    for (auto r = 1; r <= doors_no; ++r) {
        formula_deque fs2;

        auto princess_r = std::make_shared<atom_formula>(task.get_language()->get_atom_id("princess_" + std::to_string(r)));
        fs2.push_back(princess_r);

        for (auto r2 = 1; r2 <= doors_no; ++r2) {
            if (r != r2) {
                auto princess_r2 = std::make_shared<atom_formula>(task.get_language()->get_atom_id("princess_" + std::to_string(r2)));
                auto not_princess_r2 = std::make_shared<not_formula>(princess_r2);
                fs2.push_back(not_princess_r2);
            }
        }
        auto only_room_r_princess = std::make_shared<and_formula>(fs2);
        fs.push_back(only_room_r_princess);
    }

    auto only_one_room_princess = std::make_shared<or_formula>(fs);
    out << "initially C( [knight], ";
    ma_star_utils::print_formula(out, task.get_language(), only_one_room_princess);
    out << " );\n";

    out << std::endl << "goal ";
    ma_star_utils::print_formula(out, task.get_language(), task.get_goal());
    out << ";" << std::endl;
}
