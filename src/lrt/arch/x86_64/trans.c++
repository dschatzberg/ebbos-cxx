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

#include <new>

#include <arch/x86_64/mmu.h>
#include <lrt/mem.h>
#include <lrt/transImpl.h>

namespace {
  const int TRANS_PAGE_SIZE = 1 << 21; //2 mb
  ebbos::lrt::trans::GlobalEntry* globalEntries;
}

void
ebbos::lrt::trans::initArch(int nCores)
{
  void* globalMemPhys =
    (mem::memalign(TRANS_PAGE_SIZE, GTABLE_SIZE, 0));
  globalEntries = new (globalMemPhys) GlobalEntry[N_GLOBAL_ENTRIES];

  Pml4Entry* pml4 = getPml4();
  PdptEntry* pdpt = reinterpret_cast<PdptEntry*>
    (mem::memalign(PDPT_ALIGN, PDPT_SIZE, 0));
  Pd2mEntry* pdir = reinterpret_cast<Pd2mEntry*>
    (mem::memalign(PDIR_ALIGN, PDIR_SIZE, 0));

  for (int i = 0; i < PDPT_NUM_ENTS; i++) {
    pdpt[i].raw = 0;
  }

  for (int i = 0; i < PDIR_NUM_ENTS; i++) {
    pdir[i].raw = 0;
  }

  pdir[pdirIndex(reinterpret_cast<void*>(GLOBAL_MEM_VIRT))].present = 1;
  pdir[pdirIndex(reinterpret_cast<void*>(GLOBAL_MEM_VIRT))].rw = 1;
  pdir[pdirIndex(reinterpret_cast<void*>(GLOBAL_MEM_VIRT))].ps = 1;
  pdir[pdirIndex(reinterpret_cast<void*>(GLOBAL_MEM_VIRT))].base =
    reinterpret_cast<uintptr_t>(globalMemPhys) >> 21;

  pdpt[pdptIndex(reinterpret_cast<void*>(GLOBAL_MEM_VIRT))].present = 1;
  pdpt[pdptIndex(reinterpret_cast<void*>(GLOBAL_MEM_VIRT))].rw = 1;
  pdpt[pdptIndex(reinterpret_cast<void*>(GLOBAL_MEM_VIRT))].base =
    reinterpret_cast<uintptr_t>(pdir) >> 12;

  pml4[pml4Index(reinterpret_cast<void*>(GLOBAL_MEM_VIRT))].present = 1;
  pml4[pml4Index(reinterpret_cast<void*>(GLOBAL_MEM_VIRT))].rw = 1;
  pml4[pml4Index(reinterpret_cast<void*>(GLOBAL_MEM_VIRT))].base =
    reinterpret_cast<uintptr_t>(pdpt) >> 12;

}

void
ebbos::lrt::trans::initCpuArch()
{
  Pml4Entry* myPml4 = reinterpret_cast<Pml4Entry*>
    (mem::memalign(PML4_ALIGN, PML4_SIZE, event::getLocation()));
  PdptEntry* transPdpt = reinterpret_cast<PdptEntry*>
    (mem::memalign(PDPT_ALIGN, PDPT_SIZE, event::getLocation()));
  Pd2mEntry* transPdir = reinterpret_cast<Pd2mEntry*>
    (mem::memalign(PDIR_ALIGN, PDIR_SIZE, event::getLocation()));

  void* localMemPhys =
    mem::memalign(TRANS_PAGE_SIZE, LTABLE_SIZE, event::getLocation());
  physLocalEntries[event::getLocation()] =
    reinterpret_cast<LocalEntry*>(localMemPhys);

  Pml4Entry* pml4 = getPml4();
  PdptEntry* existingPdpt = reinterpret_cast<PdptEntry*>
    (pml4[pml4Index(reinterpret_cast<void*>(LOCAL_MEM_VIRT))].base << 12);

  for (int i = 0; i < PML4_NUM_ENTS; i++) {
    myPml4[i].raw = pml4[i].raw;
  }

  for (int i = 0; i < PDPT_NUM_ENTS; i++) {
    transPdpt[i].raw = existingPdpt[i].raw;
  }

  for (int i = 0; i < PDIR_NUM_ENTS; i++) {
    transPdir[i].raw = 0;
  }

  transPdir[pdirIndex(reinterpret_cast<void*>(LOCAL_MEM_VIRT))].present = 1;
  transPdir[pdirIndex(reinterpret_cast<void*>(LOCAL_MEM_VIRT))].rw = 1;
  transPdir[pdirIndex(reinterpret_cast<void*>(LOCAL_MEM_VIRT))].ps = 1;
  transPdir[pdirIndex(reinterpret_cast<void*>(LOCAL_MEM_VIRT))].base =
    reinterpret_cast<uint64_t>(localMemPhys) >> 21;

  transPdpt[pdptIndex(reinterpret_cast<void*>(LOCAL_MEM_VIRT))].present = 1;
  transPdpt[pdptIndex(reinterpret_cast<void*>(LOCAL_MEM_VIRT))].rw = 1;
  transPdpt[pdptIndex(reinterpret_cast<void*>(LOCAL_MEM_VIRT))].base =
    reinterpret_cast<uint64_t>(transPdir) >> 12;

  myPml4[pml4Index(reinterpret_cast<void*>(LOCAL_MEM_VIRT))].present = 1;
  myPml4[pml4Index(reinterpret_cast<void*>(LOCAL_MEM_VIRT))].rw = 1;
  myPml4[pml4Index(reinterpret_cast<void*>(LOCAL_MEM_VIRT))].base =
    reinterpret_cast<uint64_t>(transPdpt) >> 12;

  setPml4(myPml4);
}
