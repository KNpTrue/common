/**
 * Copyright (c) 2020 KNpTrue
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/
#pragma once

/**
 * @file error.hpp
 * @brief Common error code
*/

namespace common {

/**
 * @enum Error code.
*/
enum ErrorCode {
    ERR_OK,             ///< no error
    ERR_ERR,            ///< non-specific error
    ERR_MEM,            ///< memory error
    ERR_IDLE,           ///< not active or in use
    ERR_BUSY,           ///< another operation is in progress
    ERR_INVAL_ARG,      ///< invalid arg
    ERR_PERM,           ///< operation not permitted
    ERR_EXIST,          ///< something exists
    ERR_NOENT,          ///< no such something
    ERR_OVER_RANGE,     ///< over the range
    ERR_DQUOT,          ///< quota exceeded
    ERR_AGAIN,          ///< try again
    ERR_INTR,           ///< was interrupted
};

/**
 * @brief Lookup a human-readable definition of an error code.
 * 
 * @param err is an error code.
 * @return a pointer to a string describing the error.
*/
const char *getErrorString(ErrorCode err);

}  // namespace common
