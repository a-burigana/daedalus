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

#include "grapevine.h"

using namespace daedalus::tester;
using namespace del;

std::string grapevine::get_name() {
    return "grapevine";
}

del::language_ptr grapevine::build_language(unsigned long n) {
    name_vector atom_names(3*n), agent_names(n);

    for (unsigned long i = 0; i < n; ++i) {
        atom_names[      i]  = "secret_" + std::to_string(i);
        atom_names[n   + i]  = "at_"     + std::to_string(i) + "_room_1";
        atom_names[2*n + i]  = "at_"     + std::to_string(i) + "_room_2";

        agent_names[i] = "ag_"     + std::to_string(i);
    }

    return std::make_shared<language>(std::move(language{atom_names, agent_names}));
}
