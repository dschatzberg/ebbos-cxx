/*
 * Copyright (C) 2012 by Project SESA, Boston University
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef EBBOS_SYNC_SPINLOCK_H
#define EBBOS_SYNC_SPINLOCK_H

#include <sync/compiler.h>

namespace ebbos {
  class Spinlock {
  public:
    Spinlock() : lock_(0) {}

    inline void lock()
    {
      int rc = 0;
      while (!rc) {
        rc = __sync_bool_compare_and_swap(&lock_, UNLOCKED, LOCKED);
      }
    }

    inline void unlock()
    {
      mfence();
      access_once(lock_) = UNLOCKED;
    }
  private:
    int lock_;
    const int UNLOCKED = 0;
    const int LOCKED = 1;
  };
}

#endif
