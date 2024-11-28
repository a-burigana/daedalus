cmake_minimum_required(VERSION 3.25)
project(DAEDALUS)

set(CMAKE_CXX_STANDARD 17)

# BOOST
#set(Boost_USE_STATIC_LIBS OFF)
#set(Boost_USE_MULTITHREADED ON)
#set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.74.0 REQUIRED COMPONENTS filesystem)
message(STATUS "Boost version: ${Boost_VERSION}")

include_directories(${Boost_INCLUDE_DIR})

#
# Additional modules
#
set(CMAKE_MODULE_PATH
        "${CMAKE_SOURCE_DIR}/sanitizers-cmake/cmake"
        ${CMAKE_MODULE_PATH})

find_package(Sanitizers) # Sanitizers

add_executable(DAEDALUS
        src/main.cpp
        include/utils/clipp.h
        src/del/semantics/kripke/states/state.cpp
        include/del/semantics/kripke/states/state.h
        src/del/semantics/kripke/actions/action.cpp
        include/del/semantics/kripke/actions/action.h
        src/search/planner.cpp
        include/search/planner.h
        include/del/formulas/formula.h
        include/del/formulas/propositional/atom_formula.h
        include/del/formulas/propositional/not_formula.h
        include/del/formulas/propositional/and_formula.h
        include/del/formulas/propositional/or_formula.h
        include/del/formulas/propositional/imply_formula.h
        include/del/formulas/modal/box_formula.h
        include/del/formulas/modal/diamond_formula.h
        src/del/semantics/kripke/update/updater.cpp
        include/del/semantics/kripke/update/updater.h
        src/del/language/language.cpp
        include/del/language/language.h
        src/search/planning_task.cpp
        include/search/planning_task.h
        src/search/search_space.cpp
        include/search/search_space.h
        src/del/semantics/kripke/bisimulation/bisimulator.cpp
        include/del/semantics/kripke/bisimulation/bisimulator.h
        src/del/language/label.cpp
        include/del/language/label.h
        src/del/semantics/kripke/bisimulation/partition_refinement.cpp
        include/del/semantics/kripke/bisimulation/partition_refinement.h
        include/del/semantics/kripke/states/states_types.h
        include/del/semantics/kripke/bisimulation/bisimulation_types.h
        include/del/language/language_types.h
        include/del/semantics/kripke/actions/actions_types.h
        src/del/semantics/kripke/bisimulation/bounded_partition_refinement.cpp
        include/del/semantics/kripke/bisimulation/bounded_partition_refinement.h
        include/del/del_types.h
        tests/formula_tester.cpp
        tests/formula_tester.h
        tests/builder/language_builder.cpp
        tests/builder/language_builder.h
        tests/builder/state_builder.cpp
        tests/builder/state_builder.h
        tests/update_tester.cpp
        tests/update_tester.h
        tests/builder/action_builder.cpp
        tests/builder/action_builder.h
        include/del/formulas/propositional/true_formula.h
        include/del/formulas/propositional/false_formula.h
        tests/bisimulation/bisimulation_tester.cpp
        tests/bisimulation/bisimulation_tester.h
        tests/printer.cpp
        tests/printer.h
        tests/builder/domains/coin_in_the_box.cpp
        tests/builder/domains/coin_in_the_box.h
        include/utils/bit_deque.h
        tests/action_tester.cpp
        tests/action_tester.h
        include/search/strategies.h
        include/del/semantics/kripke/bisimulation/bounded_bisimulation_types.h
        include/search/search_types.h
        include/utils/time_utils.h
        tests/builder/domains/consecutive_numbers.cpp
        tests/builder/domains/consecutive_numbers.h
        tests/builder/domains/selective_communication.cpp
        tests/builder/domains/selective_communication.h
        tests/builder/domains/domain_utils.cpp
        tests/builder/domains/domain_utils.h
        include/utils/timer.h
        tests/builder/domains/collaboration_communication.cpp
        tests/builder/domains/collaboration_communication.h
        tests/search_tester.cpp
        tests/search_tester.h
        tests/builder/domains/switches.cpp
        tests/builder/domains/switches.h
        src/del/semantics/delphic/states/possibility.cpp include/del/semantics/delphic/states/possibility.h src/utils/storage.cpp include/utils/storage.h src/del/semantics/delphic/actions/eventuality.cpp include/del/semantics/delphic/actions/eventuality.h src/del/semantics/delphic/update/union_updater.cpp include/del/semantics/delphic/update/union_updater.h src/del/semantics/kripke/model_checker.cpp include/del/semantics/kripke/model_checker.h src/utils/printer/formula_printer.cpp include/utils/printer/formula_printer.h include/del/formulas/formula_types.h include/del/formulas/all_formulas.h src/del/semantics/delphic/model_checker.cpp include/del/semantics/delphic/model_checker.h src/del/semantics/kripke/bisimulation/bounded_contraction_builder.cpp include/del/semantics/kripke/bisimulation/bounded_contraction_builder.h src/del/semantics/kripke/bisimulation/bounded_identification.cpp include/del/semantics/kripke/bisimulation/bounded_identification.h src/del/semantics/delphic/states/possibility_spectrum.cpp include/del/semantics/delphic/states/possibility_spectrum.h include/del/semantics/delphic/states/possibility_types.h src/del/semantics/delphic/actions/eventuality_spectrum.cpp include/del/semantics/delphic/actions/eventuality_spectrum.h include/del/semantics/delphic/actions/eventuality_types.h tests/builder/domains/tiger.cpp tests/builder/domains/tiger.h tests/builder/domains/active_muddy_children.cpp tests/builder/domains/active_muddy_children.h tests/builder/domains/gossip.cpp tests/builder/domains/gossip.h tests/builder/domains/grapevine.cpp tests/builder/domains/grapevine.h src/del/semantics/delphic/delphic_utils.cpp include/del/semantics/delphic/delphic_utils.h include/del/semantics/delphic/delphic_utils.h src/search/delphic/delphic_planning_task.cpp include/search/delphic/delphic_planning_task.h include/search/delphic/delphic_planning_task.h src/search/delphic/delphic_search_space.cpp include/search/delphic/delphic_search_space.h src/search/delphic/delphic_planner.cpp include/search/delphic/delphic_planner.h)

add_sanitizers(DAEDALUS)
