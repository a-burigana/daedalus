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

namespace delphic {
    class union_updater {
    public:
        static bool is_applicable(const possibility_ptr &w, const eventuality_ptr &e);
        static bool is_applicable(const possibility_spectrum_ptr &W, const eventuality_spectrum_ptr &E);

//        static possibility_ptr update(const possibility_ptr &w, const eventuality_ptr &e, storage<possibility> &storage);
        static possibility_spectrum_ptr bounded_update(const possibility_spectrum_ptr &W, const eventuality_spectrum_ptr &E, unsigned long b, storage<possibility> &storage);

        static possibility_ptr bounded_update(const possibility_ptr &w, const eventuality_ptr &e, unsigned long b,
                                              storage<possibility> &possibility_storage);
//        static possibility bounded_update(const possibility_spectrum &W, const eventuality_spectrum &E, storage<possibility> &storage);

    private:
        static void bounded_update_helper(const possibility_ptr &w, const eventuality_ptr &e, unsigned long b,
                                          std::vector<possibility_map> &updated_possibilities, storage<possibility> &possibility_storage);

        static kripke::label update_label(const possibility_ptr &w, const eventuality_ptr &e);
    };
}

#endif //DAEDALUS_UNION_UPDATER_H
