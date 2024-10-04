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

#ifndef DAEDALUS_POSSIBILITY_TYPES_H
#define DAEDALUS_POSSIBILITY_TYPES_H

#include <memory>
#include <deque>
#include <unordered_set>

namespace delphic {
    class possibility;
    class possibility_spectrum;
    using possibility_ptr = std::shared_ptr<possibility>;
    using possibility_spectrum_ptr = std::shared_ptr<possibility_spectrum>;

    using information_state = std::unordered_set<possibility_ptr>;
    using agents_information_state = std::vector<information_state>;

    using possibility_map = std::unordered_map<possibility_ptr, possibility_ptr>;
    using information_state_map = std::unordered_map<possibility_ptr, information_state>;
    using possibility_deque = std::deque<possibility_ptr>;
}

#endif //DAEDALUS_POSSIBILITY_TYPES_H
