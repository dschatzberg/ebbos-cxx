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

#ifndef EBBOS_LRT_EVENT_H
#define EBBOS_LRT_EVENT_H

#include <lrt/arch/event.h>

namespace ebbos {
  namespace lrt {
    namespace event {
      void init(int nCores);
      void initCpu() __attribute__((noreturn));
      inline int getNCores()
      {
        return getNCoresArch();
      }
      extern "C" void altstackPush(uintptr_t val);
      extern "C" uintptr_t altstackPop();
      class Location {
      public:
        Location() : val(0) {}
        Location(int x) : val(x) {}
        operator int() { return val; };
      private:
        int val;
      };
      inline Location getLocation()
      {
        return getLocationArch();
      }
    }
  }
}

#endif
