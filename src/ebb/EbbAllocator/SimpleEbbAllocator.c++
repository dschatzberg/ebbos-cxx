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
#include <ebb/EbbAllocator/SimpleEbbAllocator.h>
#include <lrt/transImpl.h>

namespace ebbos {
  DistributedManager<SimpleEbbAllocator> allocatorManagerRep
  __attribute__((init_priority(201)));

  Ebb<DistributedManager<SimpleEbbAllocator> > allocatorManager =
    Ebb<DistributedManager<SimpleEbbAllocator> >
    (&lrt::trans::localTable[2], &allocatorManagerRep);

  Ebb<EbbAllocator> allocator = Ebb<EbbAllocator>
    (&lrt::trans::localTable[3], allocatorManager);

  using lrt::trans::N_LOCAL_ENTRIES;
  using lrt::event::getNCores;

  SimpleEbbAllocator::SimpleEbbAllocator() :
    bv(static_cast<uintptr_t*>
       (memAllocator->malloc(N_LOCAL_ENTRIES / getNCores())))
  {
    for (unsigned i = 0;
         i < ((N_LOCAL_ENTRIES / getNCores()) +
              (sizeof(uintptr_t) - 1) / sizeof(uintptr_t));
         i++) {
      bv[i] = 0;
    }
    if(getLocation() == 0) {
      bv[0] |= 1 << 0;
      bv[0] |= 1 << 1;
      bv[0] |= 1 << 2;
      bv[0] |= 1 << 3;
    }
  }

  using lrt::trans::LocalEntry;
  using lrt::trans::localTable;

  LocalEntry*
  SimpleEbbAllocator::alloc()
  {
    for(unsigned i = 0;
        i < ((N_LOCAL_ENTRIES / getNCores()) +
             (sizeof(uintptr_t) - 1) / sizeof(uintptr_t));
        i++) {
      int index = __builtin_ffsl(~bv[i]);
      if (index != 0) {
        return &localTable[N_LOCAL_ENTRIES / getNCores() * getLocation()
                           + index - 1];
      }
    }
    return nullptr;
  }

  void
  SimpleEbbAllocator::free(LocalEntry* le)
  {
    int index = le - &localTable[N_LOCAL_ENTRIES / getNCores() * getLocation()];
    if (index < 0) {
      while (1)
        ;
    }
    bv[index / (sizeof(uintptr_t) * 8)] |= index % (sizeof(uintptr_t) * 8);
  }

  void*
  SimpleEbbAllocator::operator new(size_t size) {
    return memAllocator->malloc(size);
  }
}
