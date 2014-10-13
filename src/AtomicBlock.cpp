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

#include "binfmt/Common.hpp"
#include "binfmt/AtomicBlock.hpp"

using namespace binfmt;

AtomicBlock::AtomicBlock(Buffer* buffer, Mode mode) : buffer_(buffer), mode_(mode) {
    errors_ = buffer_->errors();
    limit_ = buffer_->limit();
    position_ = buffer_->position();
}

bool AtomicBlock::commit() {
    switch (mode_) {
    case READ:
        assert(buffer_->limit()==limit_); // FIXME: Add a better check for writes
        break;
    case WRITE:
        assert(buffer_->position()==position_); // FIXME: Add a better check for reads
        break;
    default:
        assert(!"unknown mode");
        break;
    }
    if (buffer_->errors() != errors_) {
        buffer_->positionIs(buffer_->position()); 
        buffer_->limitIs(buffer_->limit()); 
        return false;
    } else {
        return true;
    }
}


