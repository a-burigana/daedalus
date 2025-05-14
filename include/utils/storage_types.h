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

#ifndef DAEDALUS_STORAGE_TYPES_H
#define DAEDALUS_STORAGE_TYPES_H

#include <memory>
#include "storage.h"
#include "../del/semantics/delphic/states/possibility_types.h"

namespace del {
    class label;

    using label_storage             = storage<label>;
    using possibility_storage       = storage<delphic::possibility>;
    using signature_storage         = possibility_storage;
    using information_state_storage = storage<delphic::information_state>;

    class storages_handler;
    using storages_handler_ptr = std::shared_ptr<storages_handler>;

//    struct storages {
//        label_storage l_storage;
//        signature_storage_ptr s_storage;
//        information_state_storage_ptr is_storage;
//    };
//
//    using storages_ptr = std::shared_ptr<storages>;
}

#endif //DAEDALUS_STORAGE_TYPES_H
