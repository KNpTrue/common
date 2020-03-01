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
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <common/assert.hpp>
#include <platform/args.hpp>
#include <platform/handle.hpp>

namespace platform {

class HandlePriv {
 public:
    int fd;
};

static int getOpenFlag(int mode) {
    int flag = 0;
    bool read = mode & Handle::MO_READ;
    bool write = mode & Handle::MO_WRITE;
    if (read && write) {
        flag |= O_RDWR;
    } else if (read) {
        flag |= O_RDONLY;
    } else if (write) {
        flag |= O_WRONLY;
    }
    if (mode & Handle::MO_CREAT) {
        flag |= O_CREAT;
    }
    if (mode & Handle::MO_NOBLOCK) {
        flag |= O_NONBLOCK;
    }
    if (mode & Handle::MO_TRUNC) {
        flag |= O_TRUNC;
    }
    return flag;
}

static FILE *getFileStream(Handle::FileNo fileNo) {
    switch (fileNo) {
    case Handle::STDIN:
        return stdin;
        break;
    case Handle::STDOUT:
        return stdout;
        break;
    case Handle::STDERR:
        return stderr;
        break;
    default:
        break;
    }
    return nullptr;
}

void Handle::printNo(FileNo fileNo, const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    vfprintf(getFileStream(fileNo), fmt, ap);
    va_end(ap);
}

void Handle::vprintNo(FileNo fileNo, const char *fmt, va_list args) {
    vfprintf(getFileStream(fileNo), fmt, args);
}

Handle::Handle(const char *path, int mode): priv(new HandlePriv) {
    ASSERT(path);
    priv->fd = open(path, getOpenFlag(mode), 0664);
    if (priv->fd < 0) {
        throw HandleException(this, common::ERR_ERR);
    }
}

Handle::~Handle() {
    close(priv->fd);
    delete priv;
}

int Handle::getFileNo() {
    return priv->fd;
}

size_t Handle::write(const void *buf, size_t len) {
    ssize_t wlen;
    wlen = ::write(priv->fd, buf, len);
    if (wlen <= 0) {
        throw HandleException(this, common::ERR_ERR);
        return 0;
    }
    return static_cast<size_t>(wlen);
}

size_t Handle::read(void *buf, size_t len) {
    ssize_t rlen;
    rlen = ::read(priv->fd, buf, len);
    if (rlen <= 0) {
        throw HandleException(this, common::ERR_ERR);
        return 0;
    }
    return static_cast<size_t>(rlen);
}

size_t Handle::seek(SeekMode mode, ssize_t len) {
    int whence;
    off_t ret;
    switch (mode) {
    case SEEK_MO_SET:
        whence = SEEK_SET;
        break;
    case SEEK_MO_CUR:
        whence = SEEK_MO_CUR;
        break;
    case SEEK_MO_END:
        whence = SEEK_MO_END;
        break;
    }
    ret = ::lseek(priv->fd, len, whence);
    if (ret < 0) {
        throw HandleException(this, common::ERR_ERR);
        return 0;
    }
    return static_cast<size_t>(ret);
}

}  // namespace platform
