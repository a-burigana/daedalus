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

#include "language_builder.h"

using namespace daedalus::tester;
using namespace del;

language_ptr language_builder::build_language1() {
    name_vector atom_names = {"p", "q"}, agent_names = {"a"};
    return std::make_shared<language>(std::move(language{atom_names, agent_names}));
}

language_ptr language_builder::build_language2() {
    name_vector atom_names = {"p"}, agent_names = {"a", "b"};
    return std::make_shared<language>(std::move(language{atom_names, agent_names}));
}

language_ptr language_builder::build_language3() {
    name_vector atom_names = {"p"}, agent_names = {"a"};
    return std::make_shared<language>(std::move(language{atom_names, agent_names}));
}
