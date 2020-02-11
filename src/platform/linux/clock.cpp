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
#include <sys/time.h>
#include <ctime>
#include <common/assert.hpp>
#include <platform/clock.hpp>
#include <platform/lock.hpp>

#define THOUSAND 1000

namespace platform {

class ClockPriv {
 public:
    ClockPriv(): mutex(Lock(Lock::LOCK_MUTEX)) {}
    Lock mutex;
};

Clock::Clock(): priv(new ClockPriv) {
    resetSource();
}

Clock::~Clock() {
    delete priv;
}

void Clock::set(time_t timestamp, Source src) {
    struct timeval now;

    priv->mutex.lock();
    if (src < this->src || src >= CS_LIMIT) {
        priv->mutex.unlock();
        throw common::Exception(common::ERR_INVAL_ARG,
            "clock source is too low or out of range");
    }

    now.tv_sec = timestamp;
    now.tv_usec = 0;
    if (settimeofday(&now, nullptr)) {
        priv->mutex.unlock();
        throw common::Exception(common::ERR_PERM,
            "insufficient privilege to call settimeofday();"
            "under Linux the CAP_SYS_TIME capability is required");
    }

    this->src = src;
    priv->mutex.unlock();
}

time_t Clock::get(Source *src) const {
    struct timeval now;
    time_t ct;

    priv->mutex.lock();
    gettimeofday(&now, nullptr);
    if (src) {
        *src = this->src;
    }
    ct = now.tv_sec + (now.tv_usec / THOUSAND / THOUSAND);
    priv->mutex.unlock();
    return ct;
}

u64 Clock::getUTCMs(Source *src) const {
    struct timeval now;
    u64 ct;

    priv->mutex.lock();
    gettimeofday(&now, nullptr);
    if (src) {
        *src = this->src;
    }
    ct = (u64)now.tv_sec * THOUSAND +
        (u64)now.tv_usec / THOUSAND;
    priv->mutex.unlock();
    return (u64)ct;
}

u64 Clock::getTotalMs() const {
    struct timespec ts;
    u64 ct;

    clock_gettime(CLOCK_MONOTONIC, &ts);
    ct = (u64)ts.tv_sec * THOUSAND +
        (u64)ts.tv_nsec / THOUSAND / THOUSAND;
    return ct;
}

const char *Clock::getFormat(char *buf, size_t len) {
    struct timeval tv;

    ASSERT(len >= CLOCK_FORMAT_STRING_LEN);

    gettimeofday(&tv, nullptr);
    if (!strftime(buf, len, "%m/%d/%Y %H:%M:%S", localtime(&tv.tv_sec))) {
        throw common::Exception(common::ERR_INVAL_ARG,
            "the length of buffer is too small");
    }
}

void Clock::resetSource() {
    priv->mutex.lock();
    src = CS_NONE;
    priv->mutex.unlock();
}

}  // namespace platform
