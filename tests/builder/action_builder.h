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

#ifndef DAEDALUS_ACTION_BUILDER_H
#define DAEDALUS_ACTION_BUILDER_H

#include "../../include/del/semantics/kripke/actions/action.h"
#include "../../include/del/language/language.h"
#include "../../include/del/del_types.h"
#include "../../include/del/formulas/formula.h"

using namespace kripke;

namespace daedalus::tester {
    class action_builder {
    public:
        static action build_public_announcement(std::string name, const del::language_ptr &language,
                                                     const del::formula_ptr &f_pre);

        static action build_public_ontic(std::string name, const del::language_ptr &language,
                                              const del::formula_ptr &f_pre, const event_post &e_post);

        static action build_private_ontic(std::string name, const del::language_ptr &language,
                                               const del::formula_ptr &f_pre, const event_post &e_post,
                                               const del::agent_set &fo_ags);

        static action build_private_sensing(std::string name, const del::language_ptr &language,
                                                 const del::formula_ptr &f_pre, const del::agent_set &fo_ags);

        static action build_semi_private_sensing(std::string name, const del::language_ptr &language,
                                                      const del::formula_ptr &f_pre, const del::formula_ptr &sensed,
                                                      const del::agent_set &fo_ags, const del::agent_set &po_ags);

        static action build_semi_private_announcement(std::string name, const del::language_ptr &language,
                                                           const del::formula_ptr &f_pre, const del::formula_ptr &announced,
                                                           const del::agent_set &fo_ags, const del::agent_set &po_ags);

        static action build_quasi_private_announcement(std::string name, const del::language_ptr &language,
                                                            const del::formula_ptr &f_pre_1, const del::formula_ptr &f_pre_2,
                                                            const del::agent_set &fo_ags_1, const del::agent_set &fo_ags_2);

        static action build_private_announcement(std::string name, const del::language_ptr &language,
                                                      const del::formula_ptr &f_pre, const del::agent_set &fo_ags);
    };
}

#endif //DAEDALUS_ACTION_BUILDER_H
