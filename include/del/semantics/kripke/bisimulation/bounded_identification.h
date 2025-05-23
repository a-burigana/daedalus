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

#ifndef DAEDALUS_BOUNDED_IDENTIFICATION_H
#define DAEDALUS_BOUNDED_IDENTIFICATION_H

#include <memory>
#include <queue>
#include "bounded_bisimulation_types.h"
#include "../../../../utils/storages_handler.h"
#include "../states/states_types.h"

namespace kripke {
    class bounded_identification {
    public:
//        static std::pair<signature_matrix, signature_map>
//        calculate_signatures(const state &s, unsigned long k, del::storages_handler_ptr handler);

        static information_state_id calculate_state_id(const state &s, unsigned long k, del::storages_handler_ptr &handler);

        static signature_id calculate_world_signature(const state &s, world_id x, unsigned long h,
                                                      del::storages_handler_ptr &handler, signature_matrix &worlds_signatures);

    private:
//        static void calculate_world_signature(const state &s, unsigned long k, world_id x, unsigned long h,
//                                              del::storages_handler_ptr handler, signature_matrix &worlds_signatures,
//                                              signature_map &sign_map);
    };
}

#endif //DAEDALUS_BOUNDED_IDENTIFICATION_H
