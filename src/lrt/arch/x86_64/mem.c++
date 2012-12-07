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

#include <arch/x86_64/multiboot.h>
#include <lrt/boot.h>
#include <lrt/memImpl.h>
#include <lrt/assert.h>

void
ebbos::lrt::mem::init(int numCores)
{
  lrtAssert(boot::multibootInformation->hasMem);
  regions = reinterpret_cast<Region*>(memstart);
  char* ptr = memstart + (numCores * sizeof(Region));
  uint64_t numBytes = static_cast<uint64_t>
    (boot::multibootInformation->memoryHigher) << 10;
  numBytes -= reinterpret_cast<uint64_t>(memstart) - 0x100000;
  for (int i = 0; i < numCores; i++) {
    regions[i].start = regions[i].current = ptr;
    ptr += numBytes / numCores;
    regions[i].end = ptr;
  }
}

void
ebbos::lrt::mem::initCpu()
{
  //nop
}
