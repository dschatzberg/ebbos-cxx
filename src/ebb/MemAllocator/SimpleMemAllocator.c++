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

#include <ebb/DistributedManager.h>
#include <ebb/MemAllocator/SimpleMemAllocator.h>
#include <lrt/memImpl.h>
#include <lrt/transImpl.h>

namespace ebbos {
  DistributedManager<SimpleMemAllocator> memAllocatorManager;

  Ebb<DistributedManager<SimpleMemAllocator> > memAllocatorManagerEbb =
    Ebb<DistributedManager<SimpleMemAllocator> >
    (&lrt::trans::localTable[0], &memAllocatorManager);

  Ebb<MemAllocator> memAllocator __attribute__((init_priority(200))) =
    Ebb<MemAllocator>(&lrt::trans::localTable[1], memAllocatorManagerEbb);

  SimpleMemAllocator::SimpleMemAllocator() {
    lrt::mem::Region* r = &lrt::mem::regions[getLocation()];
    start = r->start;
    current = r->current;
    end = r->end;
    r->start = r->end;
  }

  void*
  SimpleMemAllocator::malloc(size_t size) {
    return memalign(8, size);
  }

  void*
  SimpleMemAllocator::memalign(size_t boundary, size_t size) {
    char* p = current;
    p = reinterpret_cast<char*>(((reinterpret_cast<uintptr_t>(p) +
                                  boundary - 1) / boundary) * boundary);
    if ((p + size) > end) {
      return nullptr;
    }
    current = p + size;
    return p;
  }

  void
  SimpleMemAllocator::free(void* p) {
  }

  void*
  SimpleMemAllocator::operator new(size_t size) {
    return lrt::mem::malloc(size, getLocation());
  }

  void
  SimpleMemAllocator::operator delete(void* p) {
    //Cannot delete lrt memory!
  }
}
