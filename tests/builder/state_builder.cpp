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

#include "state_builder.h"
#include "language_builder.h"
#include <memory>

using namespace daedalus::tester;
using namespace del;
using namespace kripke;

state state_builder::build_test_state1() {
    language_ptr language = language_builder::build_language1();
    const agent a = language->get_agent_id("a");
    unsigned long atoms_number = language->get_atoms_number();

    boost::dynamic_bitset<> bs0(atoms_number), bs1(atoms_number), bs2(atoms_number);

    // Valuation of w0, w1 and w2
    bs0[0] = true;    //  p
    bs0[1] = false;   // ~q

    // Valuation of w3 ,w4 and w5
    bs1[0] = false;   // ~p
    bs1[1] = true;    //  q

    // Valuation of w6
    bs2[0] = true;    //  p
    bs2[1] = true;    //  q

    const world_id w0 = 0, w1 = 1, w2 = 2, w3 = 3, w4 = 4, w5 = 5, w6 = 6;
    const world_id worlds_number = 7;

    relations r = relations{1};
    r[0] = agent_relation(worlds_number);

    for (world_id w = 0; w < worlds_number; ++w)    // Initializing world sets
        r[0][w] = world_set(worlds_number);

    r[a][w0] = world_set(worlds_number, world_deque{w1, w2});
    r[a][w1] = world_set(worlds_number, world_deque{w1, w3});
    r[a][w2] = world_set(worlds_number, world_deque{w2, w4});
    r[a][w3] = world_set(worlds_number, world_deque{w5});
    r[a][w4] = world_set(worlds_number, world_deque{w6});
    r[a][w5] = world_set(worlds_number);
    r[a][w6] = world_set(worlds_number);

    label_vector v = label_vector{worlds_number};
    v[w0] = label{bs0};
    v[w1] = label{bs0};
    v[w2] = label{bs0};
    v[w3] = label{bs1};
    v[w4] = label{bs1};
    v[w5] = label{bs1};
    v[w6] = label{bs2};

    return state{language, worlds_number, std::move(r), std::move(v), world_set{worlds_number, world_deque{w0}}};
}

state state_builder::build_singleton(bool has_loop) {
    language_ptr language = language_builder::build_language3();
    const agent a = language->get_agent_id("a");

    agent ag_number = language->get_agents_number();
    atom atom_number = language->get_atoms_number();

    boost::dynamic_bitset<> bs0(atom_number);

    // Valuation of all worlds
    bs0[0] = true;    //  p

    world_id worlds_number = 1;
    relations r = relations{ag_number};
    r[a] = agent_relation(worlds_number);

    for (world_id w = 0; w < worlds_number; ++w)    // Initializing world sets
        r[a][w] = world_set(worlds_number);

    if (has_loop)
        r[a][0] = world_set{worlds_number, world_deque{0}};

    label_vector v = label_vector{worlds_number};
    v[0] = label{std::move(bs0)};

    return state{language, worlds_number, std::move(r), std::move(v), world_set{worlds_number, world_deque{0}}};
}

state state_builder::build_chain(unsigned long length, bool has_final_loop, bool all_designated) {
    language_ptr language = language_builder::build_language2();
    agent ag_number = language->get_agents_number();
    atom atom_number = language->get_atoms_number();

    boost::dynamic_bitset<> bs0(atom_number);

    // Valuation of all worlds
    bs0[0] = true;    //  p

    world_id worlds_number = length + 1;

    relations r = relations{ag_number};

    for (agent ag = 0; ag < ag_number; ++ag) {
        r[ag] = agent_relation(worlds_number);

        for (world_id w = 0; w < worlds_number; ++w)    // Initializing world sets
            r[ag][w] = world_set(worlds_number);
    }

    label_vector v = label_vector{worlds_number};

    for (world_id w = 0; w < worlds_number; ++w) {
        if (has_final_loop)
            r[w % ag_number][w].push_back((w+1) % worlds_number);   // = block{(w + 1) % worlds_number};   //block{std::min(w + 1, worlds_number - 1)};
        else if (w < worlds_number - 1)
            r[w % ag_number][w].push_back(w+1);                     // = block{w + 1};

        v[w] = label{bs0};
    }

    world_deque designated_worlds = {0};

    if (all_designated)
        for (world_id w = 1; w < worlds_number; ++w)
            designated_worlds.push_back(w);

    return state{language, worlds_number, std::move(r), std::move(v), world_set{worlds_number, std::move(designated_worlds)}};
}

state state_builder::build_k_tree(const unsigned long k) {
    language_ptr language = language_builder::build_language3();
    const agent a = language->get_agent_id("a");

    agent ag_number = language->get_agents_number();
    atom atom_number = language->get_atoms_number();

    boost::dynamic_bitset<> bs0(atom_number);

    // Valuation of all worlds
    bs0[0] = true;    //  p

    auto worlds_number = static_cast<world_id>(std::exp2(k));
    relations r = relations{ag_number};
    label_vector v = label_vector{worlds_number};

    for (agent ag = 0; ag < ag_number; ++ag) {
        r[ag] = agent_relation(worlds_number);

        for (world_id w = 0; w < worlds_number; ++w)    // Initializing world sets
            r[ag][w] = world_set(worlds_number);
    }

    struct tree {
        world_id w;
        std::list<std::unique_ptr<tree>> children;
    };

    auto build_tree = [&](unsigned long k_) {
        world_id current = 0;

        std::function<tree(unsigned long)> build_tree_helper = [&](unsigned long k_) {
            tree t = {current++, {}};

            for (unsigned long h = 0; h < k_; ++h)
                t.children.emplace_back(std::move(std::make_unique<tree>(build_tree_helper(h))));

            for (const auto& t_ : t.children)
                r[0][t.w].push_back(t_->w);

            v[t.w] = label{bs0};
            return t;
        };
        build_tree_helper(k_);
    };

    build_tree(k);
    return state{language, worlds_number, std::move(r), std::move(v), world_set{worlds_number, world_deque{0}}};

//    std::vector<world_id>
//            pascal_triangle_row = std::vector<world_id>(k + 1),
//            fact                = std::vector<world_id>(k + 1),
//            first_world_row     = std::vector<world_id>(k + 1);
//
//    fact[0] = 1;
//    for (unsigned long h = 1; h <= k; ++h)
//        fact[h] = h * fact[h - 1];
//
//    for (unsigned long h = 0; h < static_cast<unsigned long>(std::ceil(k / 2)); ++h)
//        pascal_triangle_row[h] = pascal_triangle_row[k - h] = fact[k] / (fact[h] * fact[k - h]);
//
//    first_world_row[0] = 0;
//    for (unsigned long h = 1; h <= k; ++h)
//        first_world_row[h] = first_world_row[h - 1] + pascal_triangle_row[h] + 1 - (h-1);
//
//    for (unsigned long r = 1; r <= k; ++r) {
//        for (unsigned long k_ = 0; w)
//    }
}