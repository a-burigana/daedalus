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

#ifndef DAEDALUS_UNION_UPDATER_H
#define DAEDALUS_UNION_UPDATER_H

#include "../../../../../include/del/semantics/delphic/states/possibility_spectrum.h"
#include "../../../../../include/del/semantics/delphic/actions/eventuality_spectrum.h"
#include "../../../../utils/storage.h"
#include "../../kripke/states/label.h"
#include "../../../del_types.h"

namespace delphic {
    class union_updater {
    public:
        static bool is_applicable(const possibility_ptr &w, const eventuality_ptr &e);
        static bool is_applicable(const possibility_spectrum_ptr &W, const eventuality_spectrum_ptr &E);

        static possibility_spectrum_ptr update(const possibility_spectrum_ptr &W, const eventuality_spectrum_ptr &E);

    private:
        static possibility_spectrum_ptr update_public_ontic(const possibility_spectrum_ptr &W, const eventuality_spectrum_ptr &E);
        static possibility_spectrum_ptr update_private_ontic(const possibility_spectrum_ptr &W, const eventuality_spectrum_ptr &E);
        static possibility_spectrum_ptr update_semi_private_sensing(const possibility_spectrum_ptr &W, const eventuality_spectrum_ptr &E);
        static possibility_spectrum_ptr update_public_sensing(const possibility_spectrum_ptr &W, const eventuality_spectrum_ptr &E);
        static possibility_spectrum_ptr update_private_announcement(const possibility_spectrum_ptr &W, const eventuality_spectrum_ptr &E);
        static possibility_spectrum_ptr update_semi_private_announcement(const possibility_spectrum_ptr &W, const eventuality_spectrum_ptr &E);
        static possibility_spectrum_ptr update_public_announcement(const possibility_spectrum_ptr &W, const eventuality_spectrum_ptr &E);

        static possibility_spectrum_ptr update_public_private_action(const possibility_spectrum_ptr &W, const eventuality_spectrum_ptr &E, const del::agent_set& o_ags);
        static possibility_spectrum_ptr update_semi_private_action(const possibility_spectrum_ptr &W, const eventuality_spectrum_ptr &E, const del::agent_set& o_ags);

        static void update_possibilities(const possibility_spectrum_ptr &W, const eventuality_ptr &e,
                                         const del::agent_set &o_ags, possibility_map &update_map, bool negated = false);

        static void update_information_states(const possibility_spectrum_ptr &W, const del::agent_set &o_ags, possibility_map &update_map);
        static void update_information_states(const possibility_spectrum_ptr &W, const del::agent_set &po_ags, const del::agent_set &o_ags, const del::formula_ptr &f_pre, possibility_map &update_map);

        static void update_designated(const possibility_spectrum_ptr &W, information_state &designated, possibility_map &update_map);

        static kripke::label update_label(const possibility_ptr &w, const eventuality_ptr &e);
    };
}

#endif //DAEDALUS_UNION_UPDATER_H
