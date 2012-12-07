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

#include <cstdint>

#include <lrt/memImpl.h>
#include <lrt/assert.h>

extern char kend[]; //end of the kernel
char* ebbos::lrt::mem::memstart = kend; //early on memory can be
                                        //stolen from the end of the
                                        //kernel like in ACPI code
ebbos::lrt::mem::Region* ebbos::lrt::mem::regions;

void*
ebbos::lrt::mem::malloc(size_t size, event::Location loc)
{
  return memalign(8, size, loc);
}

void*
ebbos::lrt::mem::memalign(size_t boundary, size_t size, event::Location loc)
{
  Region* r = &regions[loc];
  char* p = r->current;
  p = reinterpret_cast<char*>(((reinterpret_cast<uintptr_t>(p) + boundary - 1)
                               / boundary) * boundary);
  lrtAssert((p + size) < r->end);
  r->current = p + size;
  return p;
}
