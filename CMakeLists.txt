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
        src/del/states/state.cpp
        include/del/states/state.h
        src/del/actions/action.cpp
        include/del/actions/action.h
        src/search/planner.cpp
        include/search/planner.h
        src/del/formulae/formula.cpp
        include/del/formulae/formula.h
        src/del/formulae/propositional/atom_formula.cpp
        include/del/formulae/propositional/atom_formula.h
        src/del/formulae/propositional/not_formula.cpp
        include/del/formulae/propositional/not_formula.h
        src/del/formulae/propositional/and_formula.cpp
        include/del/formulae/propositional/and_formula.h
        src/del/formulae/propositional/or_formula.cpp
        include/del/formulae/propositional/or_formula.h
        src/del/formulae/propositional/imply_formula.cpp
        include/del/formulae/propositional/imply_formula.h
        src/del/formulae/modal/box_formula.cpp
        include/del/formulae/modal/box_formula.h
        src/del/formulae/modal/diamond_formula.cpp
        include/del/formulae/modal/diamond_formula.h
        src/del/update/updater.cpp
        include/del/update/updater.h
        src/del/language/language.cpp
        include/del/language/language.h
        src/search/planning_task.cpp
        include/search/planning_task.h
        src/search/search_space.cpp
        include/search/search_space.h
        src/del/bisimulation/bisimulator.cpp
        include/del/bisimulation/bisimulator.h
        src/del/states/label.cpp
        include/del/states/label.h
        src/del/bisimulation/partition_refinement.cpp
        include/del/bisimulation/partition_refinement.h
        include/del/states/states_types.h
        include/del/bisimulation/bisimulation_types.h
        include/del/language/language_types.h
        include/del/actions/actions_types.h
        src/del/bisimulation/bounded_partition_refinement.cpp
        include/del/bisimulation/bounded_partition_refinement.h
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
        src/del/formulae/propositional/true_formula.cpp
        include/del/formulae/propositional/true_formula.h
        src/del/formulae/propositional/false_formula.cpp
        include/del/formulae/propositional/false_formula.h
        tests/bisimulation_tester.cpp
        tests/bisimulation_tester.h
        tests/printer.cpp
        tests/printer.h
        tests/builder/domains/coin_in_the_box.cpp
        tests/builder/domains/coin_in_the_box.h
        include/utils/bit_deque.h
        tests/action_tester.cpp
        tests/action_tester.h
        include/search/strategies.h
        include/del/bisimulation/bounded_bisimulation_types.h
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
        )

add_sanitizers(DAEDALUS)
