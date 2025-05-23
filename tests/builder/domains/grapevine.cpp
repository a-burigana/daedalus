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

#include "grapevine.h"
#include "domain_utils.h"
#include "../action_builder.h"
#include "../../../include/del/formulas/propositional/atom_formula.h"
#include "../../../include/del/formulas/modal/box_formula.h"
#include "../../../include/del/formulas/propositional/not_formula.h"
#include "../../../include/del/formulas/propositional/and_formula.h"
#include "../../../include/del/formulas/propositional/or_formula.h"
#include "../../../include/del/formulas/propositional/true_formula.h"
#include "../../../include/del/formulas/propositional/false_formula.h"
#include "../../../include/del/formulas/propositional/imply_formula.h"
#include "ma_star_utils.h"
#include <filesystem>
#include <fstream>
#include <memory>

using namespace daedalus::tester;
using namespace del;

std::string grapevine::get_name() {
    return "grapevine";
}

del::language_ptr grapevine::build_language(unsigned long agents_no, unsigned long secrets_no) {
    name_vector atom_names(secrets_no + 2 * agents_no), agent_names(agents_no);

    for (unsigned long i = 0; i < secrets_no; ++i)
        atom_names[i] = "s_" + std::to_string(i);

    for (unsigned long i = 0; i < agents_no; ++i) {
        atom_names[secrets_no + i] = "in_room_1_ag_" + std::to_string(i);
        atom_names[secrets_no + agents_no + i] = "in_room_2_ag_" + std::to_string(i);

        agent_names[i] = "ag_" + std::to_string(i);
    }

    return std::make_shared<language>(std::move(language{atom_names, agent_names}));
}

kripke::state grapevine::build_initial_state(unsigned long agents_no, unsigned long secrets_no, label_storage &l_storage) {
    language_ptr language = grapevine::build_language(agents_no, secrets_no);

    const auto worlds_number = static_cast<const world_id>(std::exp2(secrets_no));
    world_id count = 0;

    label_vector ls = label_vector(worlds_number);
    auto secrets_combinations = domain_utils::all_combinations(secrets_no);

    for (auto &combination: secrets_combinations) {
        for (unsigned long bit = 0; bit < agents_no; ++bit)     // All agents are initially in room 1
            combination.push_back(true);

//        for (unsigned long bit = 0; bit < agents_no; ++bit)     // All agents are initially NOT in room 2
//            combination.push_back(false);

        label l = label{combination};
        ls[count++] = l_storage.emplace(std::move(l));
    }
    boost::dynamic_bitset<> all_worlds(worlds_number);
    all_worlds.set();

    relations r(language->get_agents_number());

    for (agent ag = 0; ag < language->get_agents_number(); ++ag) {
        r[ag] = agent_relation(worlds_number);

        for (world_id w = 0; w < worlds_number; ++w)
            r[ag][w] = world_bitset(all_worlds);
    }

    for (agent ag = 0; ag < secrets_no; ++ag)
        for (world_id w = 0; w < worlds_number; ++w)
            for (world_id v = 0; v < worlds_number; ++v) {
                label &lw = *l_storage.get(ls[w]), &lv = *l_storage.get(ls[v]);

                if (lw[ag] != lv[ag])
                    r[ag][w].remove(v);
            }

    world_id designated;

    for (world_id w = 0; w < worlds_number; ++w) {
        label &lw = *l_storage.get(ls[w]);
        if (lw.get_bitset().all()) designated = w;
    }

    world_bitset designated_worlds = world_bitset{worlds_number, world_set{designated}};

    return state{language, worlds_number, std::move(r), std::move(ls), std::move(designated_worlds)};
}

kripke::action_deque grapevine::build_actions(unsigned long agents_no, unsigned long secrets_no) {
    action_deque actions;

    for (unsigned long ag_1 = 0; ag_1 < secrets_no; ++ag_1) {
        boost::dynamic_bitset<> filter(agents_no);
        filter[ag_1] = true;

        auto ag_combinations = domain_utils::all_combinations_without(agents_no, filter);

        for (const auto &fo_ags : ag_combinations)
            actions.push_back(std::make_shared<action>(build_tell(agents_no, secrets_no, ag_1, fo_ags)));
    }

    for (unsigned long ag = 0; ag < agents_no; ++ag) {
        actions.push_back(std::make_shared<action>(build_left(agents_no, secrets_no, ag)));
        actions.push_back(std::make_shared<action>(build_right(agents_no, secrets_no, ag)));
    }

    return actions;
}

search::planning_task grapevine::build_task(unsigned long agents_no, unsigned long secrets_no, unsigned long learning_ags_no, label_storage &l_storage) {
    std::string name = grapevine::get_name();
    std::string id   = std::to_string(agents_no) + "_" + std::to_string(secrets_no) + "_" + std::to_string(learning_ags_no);
    language_ptr language = grapevine::build_language(agents_no, secrets_no);

    state s0 = grapevine::build_initial_state(agents_no, secrets_no, l_storage);
    action_deque actions = grapevine::build_actions(agents_no, secrets_no);
    formula_ptr goal = grapevine::build_goal(agents_no, secrets_no, learning_ags_no);

    return search::planning_task{std::move(name), std::move(id), language, std::move(s0), std::move(actions), std::move(goal)};
}

std::vector<search::planning_task> grapevine::build_tasks(label_storage &l_storage) {
    const unsigned long N_MIN_AGS = 3, N_MAX_AGS = 7, N_MIN_SECRETS = 1, N_MAX_SECRETS = 4;

    std::vector<search::planning_task> tasks;

    for (unsigned long agents_no = N_MIN_AGS; agents_no <= N_MAX_AGS; ++agents_no)
        for (unsigned long secrets_no = N_MIN_SECRETS; secrets_no <= N_MAX_SECRETS; ++secrets_no)
            for (unsigned long learning_ags_no = 1; learning_ags_no <= agents_no; ++learning_ags_no)
                tasks.push_back(build_task(agents_no, secrets_no, learning_ags_no, l_storage));

    return tasks;
}

kripke::action grapevine::build_tell(unsigned long agents_no, unsigned long secrets_no, del::agent ag_1, const agent_set &fo_ags) {
    language_ptr language = grapevine::build_language(agents_no, secrets_no);

    agent_set all_fo = fo_ags;
    all_fo[ag_1] = true;

    formula_ptr s_1 = std::make_shared<atom_formula>(ag_1);     // index of ag_1 is the same as index of s_1

    formula_deque fs;

    for (agent ag = 0; ag < agents_no; ++ag)
        fs.push_back(std::make_shared<not_formula>(std::make_shared<box_formula>(ag, s_1)));

    formula_ptr some_agent_not_K_s_1 = std::make_shared<or_formula>(std::move(fs));

//    formula_ptr K_ag_2_s_1 = std::make_shared<box_formula>(ag_2, s_1);
//    formula_ptr not_K_ag_2_s_1 = std::make_shared<not_formula>(K_ag_2_s_1);

    formula_deque ag_room_1, ag_room_2;

    for (agent ag = 0; ag < language->get_agents_number(); ++ag) {
        formula_ptr in_room_1_ag = std::make_shared<atom_formula>(language->get_atom_id("in_room_1_ag_" + std::to_string(ag)));
        formula_ptr not_in_room_1_ag = std::make_shared<not_formula>(in_room_1_ag);

        if (all_fo[ag]) {
            ag_room_1.push_back(in_room_1_ag);
            ag_room_2.push_back(not_in_room_1_ag);
        } else {
            ag_room_1.push_back(not_in_room_1_ag);
            ag_room_2.push_back(in_room_1_ag);
        }
    }

    formula_ptr all_fo_ag_room_1 = std::make_shared<and_formula>(formula_deque{ag_room_1});
    formula_ptr all_fo_ag_room_2 = std::make_shared<and_formula>(formula_deque{ag_room_2});

    formula_ptr in_room_1_ag_1 = std::make_shared<atom_formula>(language->get_atom_id("in_room_1_ag_" + std::to_string(ag_1)));
    formula_ptr not_in_room_1_ag_1 = std::make_shared<not_formula>(in_room_1_ag_1);

    formula_ptr if_ag_room_1 = std::make_shared<imply_formula>(in_room_1_ag_1, all_fo_ag_room_1);
    formula_ptr if_ag_room_2 = std::make_shared<imply_formula>(not_in_room_1_ag_1, all_fo_ag_room_2);

    formula_ptr same_room_fo_ag = std::make_shared<and_formula>(formula_deque{if_ag_room_1, if_ag_room_2});

    formula_deque fs_pre = {s_1, some_agent_not_K_s_1, same_room_fo_ag};
    formula_ptr f_pre = std::make_shared<and_formula>(fs_pre);

    std::string fo_str;
    to_string(all_fo, fo_str);
    std::reverse(fo_str.begin(), fo_str.end());
    std::string name = "tell_" + std::to_string(ag_1) + "_" + fo_str;

    return action_builder::build_private_announcement(std::move(name), language, f_pre, all_fo);
}

kripke::action grapevine::build_left(unsigned long agents_no, unsigned long secrets_no, del::agent ag) {
    language_ptr language = grapevine::build_language(agents_no, secrets_no);
    std::string ag_name = language->get_agent_name(ag);

    formula_ptr in_room_1_ag = std::make_shared<atom_formula>(language->get_atom_id("in_room_1_" + ag_name));
    formula_ptr f_pre = std::make_shared<not_formula>(in_room_1_ag);
//    formula_ptr f_pre = std::make_shared<box_formula>(ag, not_in_room_1_ag);

    event_post e_post;
    e_post[language->get_atom_id("in_room_1_" + ag_name)] = std::make_shared<true_formula>();
//    e_post[language->get_atom_id("in_room_2_" + ag_name)] = std::make_shared<false_formula>();

    std::string name = "left_" + std::to_string(ag);

    return action_builder::build_public_ontic(std::move(name), language, f_pre, e_post);
}

kripke::action grapevine::build_right(unsigned long agents_no, unsigned long secrets_no, del::agent ag) {
    language_ptr language = grapevine::build_language(agents_no, secrets_no);
    std::string ag_name = language->get_agent_name(ag);

    formula_ptr f_pre = std::make_shared<atom_formula>(language->get_atom_id("in_room_1_" + ag_name));
//    formula_ptr f_pre = std::make_shared<box_formula>(ag, in_room_1_ag);

    event_post e_post;
    e_post[language->get_atom_id("in_room_1_" + ag_name)] = std::make_shared<false_formula>();
//    e_post[language->get_atom_id("in_room_2_" + ag_name)] = std::make_shared<true_formula>();

    std::string name = "right_" + std::to_string(ag);

    return action_builder::build_public_ontic(std::move(name), language, f_pre, e_post);
}

del::formula_ptr grapevine::build_goal(unsigned long agents_no, unsigned long secrets_no, unsigned long learning_ags_no) {
    language_ptr language = grapevine::build_language(agents_no, secrets_no);
    formula_ptr goal;

    formula_deque fs;

    for (agent ag = 0; ag < agents_no; ++ag) {
        if (ag < learning_ags_no) {
            formula_deque fs_secrets;

            for (unsigned long secret = 0; secret < secrets_no; ++secret)
                if (secret != ag)
                    fs_secrets.push_back(std::make_shared<atom_formula>(secret));

            fs.push_back(std::make_shared<box_formula>(ag, std::make_shared<and_formula>(std::move(fs_secrets))));
        } else {
            for (agent ag_1 = 0; ag_1 < agents_no; ++ag_1)
                if (ag_1 < learning_ags_no) {
                    formula_deque fs_secrets;

                    for (unsigned long secret = 0; secret < secrets_no; ++secret)
                        if (secret != ag_1 and secret != ag)
                            fs_secrets.push_back(std::make_shared<atom_formula>(secret));

                    if (not fs_secrets.empty()) {
                        formula_ptr f = std::make_shared<box_formula>(ag_1, std::make_shared<and_formula>(std::move(fs_secrets)));
                        fs.push_back(std::make_shared<not_formula>(std::make_shared<box_formula>(ag, std::move(f))));
                    }
                }
        }
    }

    return std::make_shared<and_formula>(std::move(fs));
}

void grapevine::write_ma_star_problem(unsigned long agents_no, unsigned long secrets_no, unsigned long learning_ags_no,
                                      del::label_storage &l_storage) {
    auto task = build_task(agents_no, secrets_no, learning_ags_no, l_storage);
    std::string path = "../tests/builder/domains/ma_star/" + task.get_domain_name() + "/";
    std::string name = task.get_domain_name() + "_" + task.get_problem_id();
    std::string ext  = ".txt";

    if (not std::filesystem::exists(path))
        std::filesystem::create_directories(path);

    std::ofstream out = std::ofstream{path + name + ext};

    ma_star_utils::print_atoms(out, task);
    out << "action ";

    for (unsigned long teller = 0; teller < secrets_no; ++teller)
        out << "tell_" + std::to_string(teller) << ", ";

    for (unsigned long ag = 0; ag < agents_no; ++ag) {
        std::string ag_name = task.get_language()->get_agent_name(ag);
        std::string left_name = "left_" + std::to_string(ag);
        std::string right_name = "right_" + std::to_string(ag);

        // left
        out << "left_" << std::to_string(ag) << ", ";
        out << "right_" << std::to_string(ag) << (ag+1 == agents_no ? ";\n" : ", ");
    }

    ma_star_utils::print_agents(out, task);

    out << std::endl << std::endl;

    for (unsigned long teller = 0; teller < secrets_no; ++teller) {
        std::string act_name = "tell_" + std::to_string(teller);

        formula_ptr s_1 = std::make_shared<atom_formula>(teller);     // index of ag_1 is the same as index of s_1
        formula_deque fs;

        for (agent ag = 0; ag < agents_no; ++ag)
            fs.push_back(std::make_shared<not_formula>(std::make_shared<box_formula>(ag, s_1)));

        formula_ptr some_agent_not_K_s_1 = std::make_shared<or_formula>(std::move(fs));
        formula_deque fs_pre = {some_agent_not_K_s_1, s_1};
        formula_ptr f_pre = std::make_shared<and_formula>(fs_pre);

        out << "executable " << act_name << " if ";
        ma_star_utils::print_formula(out, task.get_language(), f_pre);
        out << " ;" << std::endl;

        out << act_name << " announces ";
        ma_star_utils::print_formula(out, task.get_language(), s_1);
        out << " ;" << std::endl;

        out << task.get_language()->get_agent_name(teller) << " observes " << act_name << ";" << std::endl;

        formula_ptr in_room_1_teller = std::make_shared<atom_formula>(task.get_language()->get_atom_id("in_room_1_ag_" + std::to_string(teller)));
        formula_ptr not_in_room_1_teller = std::make_shared<not_formula>(in_room_1_teller);

        for (agent ag = 0; ag < task.get_language()->get_agents_number(); ++ag) {
            if (ag != teller) {
                formula_ptr in_room_1_ag = std::make_shared<atom_formula>(task.get_language()->get_atom_id("in_room_1_ag_" + std::to_string(ag)));
                formula_ptr not_in_room_1_ag = std::make_shared<not_formula>(in_room_1_ag);

                formula_ptr both_ags_in_room_1 = std::make_shared<and_formula>(formula_deque{in_room_1_teller, in_room_1_ag});
                formula_ptr both_ags_in_room_2 = std::make_shared<and_formula>(formula_deque{not_in_room_1_teller, not_in_room_1_ag});

                formula_ptr obs_cond = std::make_shared<or_formula>(formula_deque{both_ags_in_room_1, both_ags_in_room_2});

                out << task.get_language()->get_agent_name(ag) << " observes " << act_name << " if ";
                ma_star_utils::print_formula(out, task.get_language(), obs_cond);
                out << ";" << std::endl;
            }
        }
        out << std::endl;
    }

    for (unsigned long ag = 0; ag < agents_no; ++ag) {
        std::string ag_name = task.get_language()->get_agent_name(ag);
        std::string left_name = "left_" + std::to_string(ag);
        std::string right_name = "right_" + std::to_string(ag);

        // left
        formula_ptr in_room_1_ag = std::make_shared<atom_formula>(task.get_language()->get_atom_id("in_room_1_" + ag_name));
        formula_ptr not_in_room_1_ag = std::make_shared<not_formula>(in_room_1_ag);

        out << "executable " << left_name << " if ";
        ma_star_utils::print_formula(out, task.get_language(), not_in_room_1_ag);
        out << " ;" << std::endl;

        out << left_name << " causes ";
        ma_star_utils::print_formula(out, task.get_language(), in_room_1_ag);
        out << " ;" << std::endl;

        for (agent ag2 = 0; ag2 < task.get_language()->get_agents_number(); ++ag2)
            out << task.get_language()->get_agent_name(ag2) << " observes " << left_name << ";" << std::endl;

        out << std::endl;
        // right
        out << "executable " << right_name << " if ";
        ma_star_utils::print_formula(out, task.get_language(), in_room_1_ag);
        out << " ;" << std::endl;

        out << right_name << " causes -";
        ma_star_utils::print_formula(out, task.get_language(), in_room_1_ag);
        out << " ;" << std::endl;

        for (agent ag2 = 0; ag2 < task.get_language()->get_agents_number(); ++ag2)
            out << task.get_language()->get_agent_name(ag2) << " observes " << right_name << ";" << std::endl;
        out << std::endl;
    }

    out << "\ninitially ";
    for (auto ag = 0; ag < agents_no; ++ag)
        out << "in_room_1_ag_" << ag << ", ";

    for (auto s = 0; s < secrets_no; ++s)
        out << task.get_language()->get_atom_name(s) << (s+1 < secrets_no ? ", " : ";\n");

    std::string all_agents = "[";
    for (auto ag = 0; ag < agents_no; ++ag)
        all_agents += task.get_language()->get_agent_name(ag) + (ag+1 < agents_no ? ", " : "]");

    for (auto s = 0; s < secrets_no; ++s) {
        out
            << "initially C(" << all_agents << ", "
            << "( ( B( " << task.get_language()->get_agent_name(s) << " , "  << task.get_language()->get_atom_name(s) << " ) ) | "
            << "( B( " << task.get_language()->get_agent_name(s) << " , (-" << task.get_language()->get_atom_name(s) << ") ) ) ) "
            << ");\n";
    }

    out << std::endl << "goal ";
    ma_star_utils::print_formula(out, task.get_language(), task.get_goal());
    out << ";" << std::endl;
}