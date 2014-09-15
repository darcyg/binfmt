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

#include "binfmt/Common.hpp"

#undef OVERFLOW
#undef UNDERFLOW

namespace binfmt {

class Buffer {
public:
    enum ErrorCode { OK, OVERFLOW, UNDERFLOW };

    Buffer(size_t capacity);
    ~Buffer();
    
    size_t capacity() const { return capacity_; }
    size_t limit() const { return limit_; }
    size_t position() const { return position_; }
    size_t errors() const { return errors_; }
    char const* buffer() const { return buffer_; }
    
    void limitIs(size_t limit);
    void limitInc(size_t value);
    void positionIs(size_t position);
    void positionInc(size_t value);

    ErrorCode write(char const* buf, size_t len);
    ErrorCode read(char* buf, size_t len);

    // These overloads are not allowed
    ErrorCode write(char* msg) = delete;
    ErrorCode read(char* msg) = delete;

    template <typename T> 
    typename std::enable_if<std::is_integral<T>::value, ErrorCode>::type
    write(T const& value) {
        // FixMe: Endianness 
        return write((char const*)&value, sizeof(value));
    }

    template <typename T>
    typename std::enable_if<std::is_floating_point<T>::value, ErrorCode>::type
    write(T const& value) {
        return write((char const*)&value, sizeof(value));
    }

    ErrorCode write(std::string const& value) {
        size_t const len = value.size();
        ErrorCode err;

        err = write((char const*)&len, sizeof(len));
        if (OK != err) {
            return err;
        }
        err = write(value.c_str(), len);
        if (OK != err) {
            return err;
        }
        return OK;
    }

    ErrorCode write(bool value) {
        return write((char const*)&value, 1);
    }

    template <typename T> 
    typename std::enable_if<std::is_integral<T>::value, ErrorCode>::type
    read(T& value) {
        return read((char*)&value, sizeof(value));
    }

    template <typename T>
    typename std::enable_if<std::is_floating_point<T>::value, ErrorCode>::type
    read(T& value) {
        return read((char*)&value, sizeof(value));
    }

    ErrorCode
    read(std::string& value) {
        size_t len = 0;
        ErrorCode err;

        err = read((char*)&len, sizeof(len));
        if (OK != err) {
            return err;
        }
        value.resize(len);
        read((char*)value.c_str(), len);
        if (OK != err) {
            return err;
        }
        return OK;
    } 

    ErrorCode write(bool& value) {
        value = 0;
        return read((char*)&value, 1);
    }
    
private:
    char* buffer_ = 0;
    size_t capacity_;
    size_t limit_;
    size_t position_;
    size_t errors_ = 0;
};

}
