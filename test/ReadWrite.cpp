/*
 * Copyright (c) 2014 Matt Fichman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, APEXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#pragma once

#include "binfmt\Common.hpp"
#include "binfmt\binfmt.hpp"
#include <cstdint>
#include <iostream>

int main() {
    binfmt::Buffer buffer(16);

    assert(binfmt::Buffer::OK==buffer.write(std::string("hello world")));
    assert(15==buffer.limit());
    assert(binfmt::Buffer::OK==buffer.write(true));
    assert(binfmt::Buffer::OVERFLOW==buffer.write(1));

    std::string msg;
    int32_t id = 0;
    bool boolean;
    assert(binfmt::Buffer::OK==buffer.read(msg));
    assert(binfmt::Buffer::OK==buffer.read(boolean));
    assert(binfmt::Buffer::UNDERFLOW==buffer.read(id));

    assert(id==0);
    assert(msg=="hello world");
    assert(boolean==true);

    buffer.limitIs(0);
    buffer.positionIs(0);

    binfmt::AtomicBlock atomic(&buffer, binfmt::AtomicBlock::WRITE);

    buffer.write(true);
    buffer.write(std::string("helllllllllllllllllllo world")); 

    assert(!atomic.commit());
    assert(buffer.limit()==0);
    assert(buffer.position()==0);

    return 0;
}
