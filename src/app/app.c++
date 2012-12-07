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

#include <app/app.h>
#include <ebb/EbbAllocator/EbbAllocator.h>

extern void (*start_ctors[])();
extern void (*end_ctors[])();

void *__dso_handle = 0;

extern char __eh_frame_start[];
extern "C" void __register_frame(void *);

namespace ebbos {
  namespace app {
    void
    start() {
      //register exception handling frame
      __register_frame(__eh_frame_start);
      for (int i = 0; i < (end_ctors - start_ctors); i++) {
        start_ctors[i]();
      }
      lrt::trans::LocalEntry* le __attribute__((le));
      unused = allocator->alloc();
      while (1)
        ;
    }
  }
}
