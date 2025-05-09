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

#include "../../../../../include/del/semantics/delphic/states/possibility.h"
#include "../../../../../include/del/semantics/delphic/model_checker.h"
#include "../../../../../include/utils/storage.h"

using namespace delphic;

possibility::possibility(del::language_ptr language, del::storages_ptr storages, label_id label,
                         agents_information_state state, unsigned long bound, std::optional<unsigned long> world) :
    m_language{std::move(language)},
    m_storages{std::move(storages)},
    m_label{std::move(label)},
    m_information_state{std::move(state)},
    m_bound{bound},
    m_world{world} {}

del::language_ptr possibility::get_language() const {
    return m_language;
}

//possibility_storage_ptr possibility::get_possibility_storage() const {
//    return m_p_storage;
//}
//
//information_state_storage_ptr possibility::get_information_state_storage() const {
//    return m_is_storage;
//}

//possibility_ptr possibility::get(delphic::possibility_id w) const {
//    return m_storages->s_storage->get(w);
//}

const label_id &possibility::get_label_id() const {
    return m_label;
}

unsigned long possibility::get_bound() const {
    return m_bound;
}

// todo:: use ids everywhere and do not use storages_ptr in states/actions. Use storages_ptr only where we need to extract information
//const information_state &possibility::get_information_state(del::agent ag) const {
//    return *m_storages->is_storage->get(m_information_state[ag]);
//}

const information_state_id possibility::get_information_state_id(del::agent ag) const {
    return m_information_state[ag];
}

void possibility::set_information_state(del::agent ag, delphic::information_state_id &is) {
    m_information_state[ag] = is;
}

bool possibility::satisfies(const del::formula_ptr &f, const del::storages_ptr &storages) const {
    return model_checker::holds_in(*this, *f, storages);
}

bool possibility::operator<(const possibility &rhs) const {
    if (m_bound != rhs.m_bound) return m_bound < rhs.m_bound;
    if (m_label != rhs.m_label) return m_label < rhs.m_label;
    return m_information_state < rhs.m_information_state;
}

bool possibility::operator>(const possibility &rhs) const {
    if (m_bound != rhs.m_bound) return m_bound > rhs.m_bound;
    if (m_label != rhs.m_label) return m_label > rhs.m_label;
    return m_information_state > rhs.m_information_state;
}

bool possibility::operator<=(const possibility &rhs) const {
    return *this < rhs or *this == rhs;
}

bool possibility::operator>=(const possibility &rhs) const {
    return *this > rhs or *this == rhs;
}

bool possibility::operator==(const possibility &rhs) const {
    return m_bound == rhs.m_bound and
           (m_bound == 0 ?
            m_label == rhs.m_label :
            m_label == rhs.m_label and m_information_state == rhs.m_information_state);
}

bool possibility::operator!=(const possibility &rhs) const {
    return not (rhs == *this);
}
