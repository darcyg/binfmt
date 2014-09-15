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

#include "binfmt\Common.hpp"
#include "binfmt\Buffer.hpp"
#include <iostream>

using namespace binfmt;

Buffer::Buffer(size_t capacity) : capacity_(capacity), limit_(0), position_(0) {
    buffer_ = new char[capacity];
    assert(buffer_);
}

Buffer::~Buffer() {
    delete[] buffer_;
}

void Buffer::limitIs(size_t limit) { 
    assert(limit <= capacity_);
    limit_ = limit; 
}

void Buffer::limitInc(size_t value) {
    limitIs(limit_+value);
}

void Buffer::positionIs(size_t position) { 
    assert(position <= limit_); 
    position_ = position; 
}

void Buffer::positionInc(size_t value) {
    positionIs(position_+value);
}

Buffer::ErrorCode Buffer::write(char const* buf, size_t len) {
    if (limit_+len > capacity_) {
        errors_++;
        return OVERFLOW;
    }
    memcpy(buffer_+limit_, buf, len);
    limitInc(len);
    return OK;
}

Buffer::ErrorCode Buffer::read(char* buf, size_t len) {
    if (position_+len > limit_) {
        errors_++;
        return UNDERFLOW;
    }
    memcpy(buf, buffer_+position_, len);
    positionInc(len);
    return OK;
}

