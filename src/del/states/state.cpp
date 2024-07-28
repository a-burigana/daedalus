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

#include "../../../include/del/states/state.h"
#include <queue>
#include <string>
#include <utility>
#include <vector>

using namespace del;

state::state(language_ptr language, unsigned long long worlds_number, relations relations,
             label_vector valuation, world_set designated_worlds) :
        m_language{std::move(language)},
        m_worlds_number{worlds_number},
        m_relations{std::move(relations)},
        m_labels{std::move(valuation)},
        m_designated_worlds{std::move(designated_worlds)} {
    calculate_worlds_depth();
}

unsigned long long state::get_worlds_number() const {
    return m_worlds_number;
}

const world_set &state::get_agent_possible_worlds(const agent ag, const world_id w) const {
    return m_relations[ag].at(w);
}

bool state::has_edge(const agent ag, const world_id w, const world_id v) const {
//    return std::find(m_relations[ag].at(w).begin(), m_relations[ag].at(w).end(), v) != m_relations[ag].at(w).end();
    return (*m_relations[ag][w])[v];
}

const label &state::get_label(const world_id w) const {
    return m_labels[w];
}

const world_set &state::get_designated_worlds() const {
    return m_designated_worlds;
}

bool state::is_designated(const world_id w) const {
    return std::find(m_designated_worlds.begin(), m_designated_worlds.end(), w) != m_designated_worlds.end();
}

language_ptr state::get_language() const {
    return m_language;
}

unsigned long long state::get_depth(const world_id w) const {
    return m_worlds_depth[w];
}

unsigned long long state::get_max_depth() const {
    return m_max_depth;
}

bool state::satisfies(const formula_ptr &f) const {
    const auto check = [&](const world_id wd) { return f->holds_in(*this, wd); };
    return std::all_of(m_designated_worlds.begin(), m_designated_worlds.end(), check);
}

void state::calculate_worlds_depth() {
    m_worlds_depth = std::vector<unsigned long long>(m_worlds_number);
    m_max_depth = 0;

    std::queue<world_id> to_visit;
    boost::dynamic_bitset<> assigned(m_worlds_number);

    for (const world_id wd : m_designated_worlds) {
        m_worlds_depth[wd] = 0;     // The designated worlds have depth 0
        assigned[wd] = true;
        to_visit.push(wd);
    }

    while (not to_visit.empty()) {
        const world_id current = to_visit.front();
        to_visit.pop();

        if (m_worlds_depth[current] > m_max_depth)
            m_max_depth = m_worlds_depth[current];

        for (agent ag = 0; ag < m_language->get_agents_number(); ++ag) {
            for (const world_id v : m_relations[ag][current]) {
//            for (world_id v = 0; v < m_worlds_number; ++v) {
                if (not assigned[v]) {      // has_edge(ag, current, v) and
                    m_worlds_depth[v] = m_worlds_depth[current] + 1;
                    assigned[v] = true;
                    to_visit.push(v);
                }
            }
        }
    }
}

std::ostream &del::operator<<(std::ostream &os, const state &s) {
    using edges_map = std::map<std::pair<world_id, world_id>, std::vector<agent>>;

    const std::string font = std::string{"\"Helvetica,Arial,sans-serif\""};

    os
            << "digraph {" << std::endl
            << "\tfontname=" << font << ";" << std::endl
            << "\tnode [fontname=" << font << "];" << std::endl
            << "\tedge [fontname=" << font << "];" << std::endl
            << "\tlabeljust=l;" << std::endl
            << "\trankdir=BT;" << std::endl
            << "\tranksep=0.5;" << std::endl
            << "\tnewrank=true;" << std::endl
            << "\tcompound=true;" << std::endl;

    os << std::endl << "\t{ node[shape=doublecircle] ";

    for (const world_id wd : s.get_designated_worlds())
        os << "w" << wd << "; ";

    os << "}" << std::endl << std::endl;

    // Map: <accessible_worlds_hash> -> <list_of_worlds>
    try {
        std::map<unsigned long, std::deque<world_id>> ranks;

        for (world_id w = 0; w < s.get_worlds_number(); ++w) {
            boost::dynamic_bitset<> out(s.get_worlds_number());

            for (agent ag = 0; ag < s.get_language()->get_agents_number(); ++ag)
                out |= *s.get_agent_possible_worlds(ag, w);

            ranks[out.to_ulong()].emplace_back(w);
        }

        for (const auto &[hash, ws]: ranks) {
            os << "\t{ rank = same; ";

            for (const world_id w_id: ws)
                os << "w" << w_id << "; ";

            os << "}" << std::endl;
        }

        os << std::endl;
    } catch (boost::wrapexcept<std::overflow_error> &e) {}

    edges_map edges;

    for (agent ag = 0; ag < s.get_language()->get_agents_number(); ++ag)
        for (world_id w = 0; w < s.get_worlds_number(); ++w)
            for (const world_id v : s.get_agent_possible_worlds(ag, w))
//            for (world_id v = 0; v < state.get_worlds_number(); ++v)
                if (edges.find({w, v}) == edges.end())      // state.has_edge(ag, w, v) and
                    edges[{w, v}] = {ag};
                else
                    edges[{w, v}].emplace_back(ag);

    auto print_ags = [&s](const std::vector<agent> &ags) {
        std::string ags_string;
        for (const agent &ag: ags)
            ags_string += std::string{s.get_language()->get_agent_name(ag)} + ", ";
        return ags_string.substr(0, ags_string.size() - 2);
    };

    for (auto &[t, ags]: edges) {
        const world_id from = t.first, to = t.second;
        auto it = edges.find({to, from});
        if (it == edges.end())
            os << "\tw" << from << " -> w" << to << " [label=\"" << print_ags(ags) << "\"];" << std::endl;
        else {
            std::vector<agent> ags2 = it->second;
            auto size = std::max(ags.size(), ags2.size());
            std::vector<agent> ags_intersection = std::vector<agent>(size), ags_difference = std::vector<agent>(size);

            std::sort(ags.begin(), ags.end());
            std::sort(ags2.begin(), ags2.end());

            auto it1 = std::set_intersection(ags.begin(), ags.end(), ags2.begin(), ags2.end(), ags_intersection.begin());
            auto it2 = std::set_difference(ags.begin(), ags.end(), ags2.begin(), ags2.end(), ags_difference.begin());

            ags_intersection.resize(it1 - ags_intersection.begin());
            ags_difference.resize(it2 - ags_difference.begin());

            if (not ags_intersection.empty()) {
                if (from < to)
                    os << "\tw" << from << " -> w" << to << " [label=\"" << print_ags(ags_intersection) << "\" dir=both];" << std::endl;
                else if (from == to)
                    os << "\tw" << from << " -> w" << to << " [label=\"" << print_ags(ags_intersection) << "\"];" << std::endl;
            }

            if (not ags_difference.empty())
                os << "\tw" << from << " -> w" << to << " [label=\"" << print_ags(ags_difference) << "\"];" << std::endl;
        }
    }

    os
        << "\tnode [] val_table [shape=none label=<"                                       << std::endl
        << "\t\t<TABLE border=\"0\" cellspacing=\"0\" cellborder=\"1\" cellpadding=\"2\">" << std::endl;

    os
        << "\t\t\t<TR>"             << std::endl
        << "\t\t\t\t<TD>World</TD>" << std::endl
        << "\t\t\t\t<TD>Depth</TD>" << std::endl
        << "\t\t\t\t<TD>Label</TD>" << std::endl
        << "\t\t\t</TR>"            << std::endl;

    for (world_id w = 0; w < s.get_worlds_number(); ++w) {
        os
            << "\t\t\t<TR>"                          << std::endl
            << "\t\t\t\t<TD>" << "w" << w << "</TD>" << std::endl;

        os
                << "\t\t\t\t<TD>" << s.get_depth(w) << "</TD>" << std::endl;

        os << "\t\t\t\t<TD>" << std::endl;

        for (atom p = 0; p < s.get_language()->get_atoms_number(); ++p) {
            if (s.get_label(w)[p]) {
                std::string_view color = "blue";        // s.get_label(w)[p] ? "blue" : "red";
                std::string_view sep   = " ";           // p < s.get_language()->get_atoms_number() - 1 ? ", " : "";

                os << "\t\t\t\t\t<font color=\"" << color << "\">" << s.get_language()->get_atom_name(p) << "</font>"
                   << sep << std::endl;
            }
        }

        os
            << "\t\t\t\t</TD>" << std::endl
            << "\t\t\t</TR>"   << std::endl;
    }

    os
        << "\t\t</TABLE>" << std::endl
        << "\t>];"        << std::endl
        << "}";

    return os;
}
