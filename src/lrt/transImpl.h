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

#ifndef EBBOS_LRT_TRANSIMPL_H
#define EBBOS_LRT_TRANSIMPL_H

#include <lrt/trans.h>
#include <lrt/arch/transImpl.h>

namespace ebbos {
  namespace lrt {
    namespace trans {
      const int GTABLE_SIZE = 1 << 21; //2 mb
      extern void (*defaultVtable[256])(); //size 256 table to
                                           //pointers to functions
                                           //taking no arguments and
                                           //having no return

      extern LocalEntry** physLocalEntries;
      extern LocalEntry* ltable;
      const int N_GLOBAL_ENTRIES = GTABLE_SIZE / sizeof(GlobalEntry);
    }
  }
}


#endif
