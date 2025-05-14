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

#ifndef DAEDALUS_MODEL_CHECKER_DELPHIC_H
#define DAEDALUS_MODEL_CHECKER_DELPHIC_H

#include "states/possibility.h"
#include "../../formulas/all_formulas.h"
#include "../../../utils/storage.h"

namespace delphic {
    class model_checker {
    public:
        static bool holds_in(const possibility &w, const del::formula &f, del::storages_handler_ptr handler);

    private:
        static bool holds_in(const possibility &w, const del::atom_formula &f, del::storages_handler_ptr handler);
        static bool holds_in(const possibility &w, const del::not_formula &f, del::storages_handler_ptr handler);
        static bool holds_in(const possibility &w, const del::and_formula &f, del::storages_handler_ptr handler);
        static bool holds_in(const possibility &w, const del::or_formula &f, del::storages_handler_ptr handler);
        static bool holds_in(const possibility &w, const del::imply_formula &f, del::storages_handler_ptr handler);
        static bool holds_in(const possibility &w, const del::box_formula &f, del::storages_handler_ptr handler);
        static bool holds_in(const possibility &w, const del::diamond_formula &f, del::storages_handler_ptr handler);
    };
}

#endif //DAEDALUS_MODEL_CHECKER_DELPHIC_H
