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

#include "../include/utils/clipp.h"
#include "../tests/search_tester.h"
#include "../tests/action_tester.h"
#include "../tests/builder/domains/consecutive_numbers.h"
#include "../tests/printer.h"
#include "../tests/builder/domains/collaboration_communication.h"
#include "../tests/builder/domains/coin_in_the_box.h"
#include "../include/search/planner.h"
#include "../tests/bisimulation/bisimulation_tester.h"
#include "../tests/builder/state_builder.h"
#include "../include/del/semantics/kripke/bisimulation/bounded_identification.h"
#include "utils/storage.cpp"
#include "../tests/builder/domains/tiger.h"
#include "../tests/builder/domains/gossip.h"
#include "../include/utils/printer/formula_printer.h"
#include "../tests/builder/domains/grapevine.h"
#include "../tests/builder/domains/active_muddy_children.h"
#include "../include/search/delphic/delphic_planning_task.h"
#include "../include/del/semantics/delphic/delphic_utils.h"
#include "../include/del/semantics/delphic/update/union_updater.h"
#include "../include/search/delphic/delphic_planner.h"
#include "../tests/builder/domains/selective_communication.h"
#include "../tests/builder/domains/eavesdropping.h"
#include <memory>
#include <string>
#include <filesystem>
#include <iostream>

#define OUT_PATH std::string{"../tests/out/"}

using namespace daedalus::tester;
using namespace clipp;

//void storage_test();
void run(int argc, char *argv[]);

int main(int argc, char *argv[]) {
    run(argc, argv);
    return 0;
}

/*void storage_test() {
//    storage<int> int_storage;
//    int x = 5, y = 5;
//
//    auto x_id = int_storage.emplace(std::move(x));
//    auto y_id = int_storage.emplace(std::move(y));
//
//    assert(x_id == y_id);

    signature_storage_ptr s_storage = std::make_shared<storage<possibility>>();
    information_state_storage_ptr is_storage = std::make_shared<storage<information_state>>(information_state{});

    unsigned long k = 3, h = 3;
    state s1 = state_builder::build_test_state1(), s1_contr = bisimulator::contract(contraction_type::rooted, s1, k).second;
    auto worlds_signatures = signature_matrix(h+1), worlds_signatures_contr = signature_matrix(h+1);

    for (auto &h_signatures: worlds_signatures)
        h_signatures = signature_vector(s1.get_worlds_number());

    for (auto &h_signatures: worlds_signatures_contr)
        h_signatures = signature_vector(s1_contr.get_worlds_number());

    world_id x = 0, y = 0;

    bounded_identification::calculate_world_signature(s1,      k, x, h, s_storage, is_storage, worlds_signatures);
    bounded_identification::calculate_world_signature(s1_contr, k, y, h, s_storage, is_storage, worlds_signatures);

    signature_ptr sign_x_h = s_storage->get(worlds_signatures[h][x]);
    signature_ptr sign_y_h = s_storage->get(worlds_signatures[h][y]);

    assert(sign_x_h->get_label() == sign_y_h->get_label());
    assert(sign_x_h->get_information_state(0) == sign_y_h->get_information_state(0));
    assert(*sign_x_h == *sign_y_h);
}*/

void run(int argc, char *argv[]) {
    std::string semantics = "kripke", strategy = "unbounded", contraction_type = "full", bound;
    std::string domain;
    std::vector<std::string> parameters, actions;
    bool print_results = false, print_info = false, debug = false;

    auto cli = (
            required("-d", "--domain") & value("domain", domain),
            required("-p", "--parameters") & values("parameters", parameters),
            option("-s", "--semantics") & value("semantics", semantics).doc("Selects the preferred DEL semantics ('kripke' or 'delphic')"),
            option("-t", "--strategy" ) & value("strategy", strategy).doc("Selects the search strategy ('unbounded' or 'bounded')"),
            option("-c", "--contraction" ) & value("contraction type", contraction_type).doc("Selects the type of bisimulation contraction to perform ('full', 'rooted' or 'canonical')"),
            option("-a", "--actions" ) & values("actions", actions).doc("Actions to execute"),
            option("-b", "--bound" ) & values("bound", bound).doc("Initial bound"),
            option("--print").set(print_results).doc("Print time results"),
            option("--info").set(print_info),
            option("--debug").set(debug)
    );

    if (not parse(argc, argv, cli)) {
        std::cout << make_man_page(cli, argv[0]);
        return;
    }

    search::planning_task_ptr task;
    del::label_storage_ptr l_storage = std::make_shared<storage<label>>();
    del::signature_storage_ptr s_storage = std::make_shared<storage<signature>>();
    del::information_state_storage_ptr is_storage = std::make_shared<storage<information_state>>(information_state{});

    storages_ptr storages = std::make_shared<del::storages>(del::storages{std::move(l_storage), std::move(s_storage), std::move(is_storage)});

    if (domain == "active_muddy_children" or domain == "amc")
        task = std::make_unique<search::planning_task>(active_muddy_children::build_task(std::stoul(parameters[0]), std::stoul(parameters[1]), std::stoul(parameters[2]), storages->l_storage));
    else if (domain == "coin_in_the_box" or domain == "cb")
        task = std::make_unique<search::planning_task>(coin_in_the_box::build_task(std::stoul(parameters[0]), storages->l_storage));
    else if (domain == "collaboration_communication" or domain == "cc")
        task = std::make_unique<search::planning_task>(collaboration_communication::build_task(std::stoul(parameters[0]), std::stoul(parameters[1]), std::stoul(parameters[2]), std::stoul(parameters[3]), storages->l_storage));
    else if (domain == "consecutive_numbers" or domain == "cn")
        task = std::make_unique<search::planning_task>(consecutive_numbers::build_task(std::stoul(parameters[0]), storages->l_storage));
    else if (domain == "eavesdropping" or domain == "eav")
        task = std::make_unique<search::planning_task>(eavesdropping::build_task(std::stoul(parameters[0]), std::stoul(parameters[1]), storages->l_storage));
    else if (domain == "gossip" or domain == "gos")
        task = std::make_unique<search::planning_task>(gossip::build_task(std::stoul(parameters[0]), std::stoul(parameters[1]), std::stoul(parameters[2]), storages->l_storage));
    else if (domain == "grapevine" or domain == "gra")
        task = std::make_unique<search::planning_task>(grapevine::build_task(std::stoul(parameters[0]), std::stoul(parameters[1]), std::stoul(parameters[2]), storages->l_storage));
    else if (domain == "selective_communication" or domain == "sc")
        task = std::make_unique<search::planning_task>(selective_communication::build_task(std::stoul(parameters[0]), std::stoul(parameters[1]), std::stoul(parameters[2]), storages->l_storage));
    else if (domain == "tiger" or domain == "tig")
        task = std::make_unique<search::planning_task>(tiger::build_task(std::stoul(parameters[0]), std::stoul(parameters[1]), storages->l_storage));

//    search::delphic_planning_task task_ = delphic_utils::convert(*task);

    search::strategy t = strategy == "unbounded" ? search::strategy::unbounded_search : search::strategy::iterative_bounded_search;
    enum contraction_type type = contraction_type == "full" ? contraction_type::full : (contraction_type == "rooted" ? contraction_type::rooted : contraction_type::canonical);

    if (debug) {
        if (actions.empty()) {
            if (semantics == "kripke")
                daedalus::tester::printer::print_results(*task, t, type, storages, OUT_PATH);
            else if (semantics == "delphic") {}
//                daedalus::tester::printer::print_delphic_results(task_, t, OUT_PATH);
        } else {
            std::string path = task->get_domain_name() + "/" + task->get_problem_id() + "/updates/" + semantics + "/" + strategy + "/";

            if (semantics == "kripke") {
                kripke::action_deque as = task->get_actions(actions);
//                daedalus::tester::printer::print_state(*task->get_initial_state(), OUT_PATH + path, "s0");
                state_deque ss = {task->get_initial_state()};
                unsigned long b = bound.empty() ? task->get_goal()->get_modal_depth() : std::stoul(bound);
                daedalus::tester::printer::print_states(ss, as, storages, task->get_goal(), OUT_PATH + path, "s0", true, type, b);
            } else if (semantics == "delphic") {
//                delphic::action_deque as = task_.get_actions(actions);
//                daedalus::tester::printer::print_state(*task_.get_initial_state(), OUT_PATH + path, "W0");
//                daedalus::tester::printer::print_states(task_.get_initial_state(), as, task_.get_goal(), OUT_PATH + path, "W0");
            }
        }
    } else {
        std::string out_file_path = "ijcai/time_results/" + task->get_domain_name() + "/";

        if (not std::filesystem::exists(out_file_path))
            std::filesystem::create_directories(out_file_path);

        std::string out_file_name = "results_" + semantics + "_" + strategy + ".csv";
        std::ifstream read_out_file = std::ifstream{out_file_path + out_file_name};
        bool empty_file = read_out_file.peek() == std::ifstream::traits_type::eof();
        read_out_file.close();

        std::ofstream out_file = std::ofstream{out_file_path + out_file_name, std::ios_base::app};

        if (empty_file) {
            switch (t) {
                case search::strategy::unbounded_search:
                    out_file << "Domain;Problem ID;#Atoms;#Agents;|W0|;#Actions;Goal depth;Plan length;#Nodes;#Worlds;Time"
                             << std::endl;
                    break;
                case search::strategy::iterative_bounded_search:
                    out_file << "Domain;Problem ID;#Atoms;#Agents;|W0|;#Actions;Goal depth;Bound;Plan length;#Nodes;#Worlds;Time"
                             << std::endl;
                    break;
            }
        }

        if (print_info)
            daedalus::tester::printer::print_domain_info(*task, out_file);

        if (not print_info) {
            if (semantics == "kripke") {
                if (print_results) daedalus::tester::printer::print_time_results(*task, t, type, storages, out_file);
                else search::planner::search(*task, t, type, storages);
            } else if (semantics == "delphic") {
//                if (print_results) daedalus::tester::printer::print_delphic_time_results(task_, t, out_file);
//                else search::delphic_planner::search(task_, t);
            }
        }

        out_file.close();
    }
}
