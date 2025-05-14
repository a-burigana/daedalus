// MIT License
//
// Copyright (c) 2022 Alessandro Burigana and Francesco Fabiano
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

#ifndef DAEDALUS_STORAGES_HANDLER_H
#define DAEDALUS_STORAGES_HANDLER_H

#include "storage.h"
#include "storage_types.h"
#include "../del/language/label.h"
#include "../del/semantics/kripke/states/states_types.h"
#include "../del/semantics/kripke/bisimulation/bounded_bisimulation_types.h"
#include <memory>

namespace del {
    class storages_handler {
    public:
        storages_handler(unsigned long b, label_storage storage) {
            l_storage   = std::move(storage);
            s_storages  = std::deque<signature_storage>(b+1);
            is_storages = std::deque<information_state_storage>(b+1);

            for (unsigned long i = 0; i <= b; ++i) {
                s_storages[i]  = signature_storage();
                is_storages[i] = information_state_storage();
            }
        }

        [[nodiscard]] auto &get_label_storage() { return l_storage; }
        [[nodiscard]] auto &get_signature_storage(unsigned long h) { return s_storages[h]; }
        [[nodiscard]] auto &get_information_state_storage(unsigned long h) { return is_storages[h]; }

        void expand_storages() {
            s_storages.emplace_back();
            is_storages.emplace_back();
        }

    private:
        label_storage l_storage;
        std::deque<signature_storage> s_storages;
        std::deque<information_state_storage> is_storages;
    };
}
#endif //DAEDALUS_STORAGES_HANDLER_H
