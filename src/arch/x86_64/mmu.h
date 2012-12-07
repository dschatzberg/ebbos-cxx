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

#ifndef EBBOS_ARCH_X86_64_H
#define EBBOS_ARCH_X86_64_H

#include <cstdint>

namespace ebbos {
  const int PAGE_SIZE = 4096;
  class Pml4Entry {
  public:
    union {
      uint64_t raw;
      struct {
        uint64_t present :1;
        uint64_t rw :1;
        uint64_t privilege :1;
        uint64_t write_through :1;
        uint64_t cache_disable :1;
        uint64_t accessed :1;
        uint64_t :6;
        uint64_t base :40;
        uint64_t :11;
        uint64_t nx :1;
      } __attribute__((packed));
    };
  };
  static_assert(sizeof(Pml4Entry) == 8, "Pml4Entry packing issue");

  class PdptEntry {
  public:
    union {
      uint64_t raw;
      struct {
        uint64_t present :1;
        uint64_t rw :1;
        uint64_t privilege :1;
        uint64_t write_through :1;
        uint64_t cache_disable :1;
        uint64_t accessed :1;
        uint64_t :1;
        uint64_t ps :1; // must be unset for standard entry
        uint64_t :4;
        uint64_t base :40;
        uint64_t :11;
        uint64_t nx :1;
      } __attribute__((packed));
    };
  };
  static_assert(sizeof(PdptEntry) == 8, "PdptEntry packing issue");

  class Pd2mEntry {
  public:
    union {
      uint64_t raw;
      struct {
        uint64_t present :1;
        uint64_t rw :1;
        uint64_t privilege :1;
        uint64_t write_through :1;
        uint64_t cache_disable :1;
        uint64_t accessed :1;
        uint64_t dirty :1;
        uint64_t ps :1; // must be set for 2m entry
        uint64_t global :1;
        uint64_t :3;
        uint64_t pat :1;
        uint64_t :8;
        uint64_t base :31;
        uint64_t :11;
        uint64_t nx :1;
      } __attribute__((packed));
    };
  };
  static_assert(sizeof(Pd2mEntry) == 8, "Pd2mEntry packing issue");

  class PdEntry {
  public:
    union {
      uint64_t raw;
      struct {
        uint64_t present :1;
        uint64_t rw :1;
        uint64_t privilege :1;
        uint64_t write_through :1;
        uint64_t cache_disable :1;
        uint64_t accessed :1;
        uint64_t :1;
        uint64_t ps :1; // must be unset for standard entry
        uint64_t :4;
        uint64_t base :40;
        uint64_t :11;
        uint64_t nx :1;
      } __attribute__((packed));
    };
  };
  static_assert(sizeof(PdEntry) == 8, "PdEntry packing issue");

  class PtabEntry {
  public:
    union {
      uint64_t raw;
      struct {
        uint64_t present :1;
        uint64_t rw :1;
        uint64_t privilege :1;
        uint64_t write_through :1;
        uint64_t cache_disable :1;
        uint64_t accessed :1;
        uint64_t dirty :1;
        uint64_t pat :1; // must be set for 2m entry
        uint64_t global :1;
        uint64_t :3;
        uint64_t base :40;
        uint64_t :11;
        uint64_t nx :1;
      } __attribute__((packed));
    };
  };
  static_assert(sizeof(PtabEntry) == 8, "PtabEntry packing issue");

  inline Pml4Entry*
  getPml4() {
    Pml4Entry* ret;
    asm volatile (
                  "mov %%cr3, %[pml4]"
                  : [pml4] "=r" (ret)
                  );
    return ret;
  }

  inline void
  setPml4(Pml4Entry* pml4)
  {
    asm volatile (
                  "mov %[pml4], %%cr3"
                  :
                  : [pml4] "r" (pml4)
                  );
  }

  const int PML4_INDEX_SHIFT = 39;
  const int PML4_INDEX_BITS = 9;

  const int PDPT_INDEX_SHIFT = 30;
  const int PDPT_INDEX_BITS = 9;

  const int PDIR_INDEX_SHIFT = 21;
  const int PDIR_INDEX_BITS = 9;

  const int PTAB_INDEX_SHIFT = 12;
  const int PTAB_INDEX_BITS = 9;

  const int PML4_SIZE = 4096;
  const int PDPT_SIZE = 4096;
  const int PDIR_SIZE = 4096;
  const int PTAB_SIZE = 4096;

  const int PML4_ALIGN = 4096;
  const int PDPT_ALIGN = 4096;
  const int PDIR_ALIGN = 4096;
  const int PTAB_ALIGN = 4096;

  const int PML4_NUM_ENTS = PML4_SIZE / sizeof(Pml4Entry);
  const int PDPT_NUM_ENTS = PDPT_SIZE / sizeof(PdptEntry);
  const int PDIR_NUM_ENTS = PDIR_SIZE / sizeof(PdEntry);
  const int PTAB_NUM_ENTS = PTAB_SIZE / sizeof(PtabEntry);
  int pml4Index(void* addr) {
    return (reinterpret_cast<uintptr_t>(addr) >> PML4_INDEX_SHIFT) &
      ((1 << PML4_INDEX_BITS) - 1);
  }
  int pdptIndex(void* addr) {
    return (reinterpret_cast<uintptr_t>(addr) >> PDPT_INDEX_SHIFT) &
      ((1 << PDPT_INDEX_BITS) - 1);
  }
  int pdirIndex(void* addr) {
    return (reinterpret_cast<uintptr_t>(addr) >> PDIR_INDEX_SHIFT) &
      ((1 << PDIR_INDEX_BITS) - 1);
  }
  int ptabIndex(void* addr) {
    return (reinterpret_cast<uintptr_t>(addr) >> PTAB_INDEX_SHIFT) &
      ((1 << PTAB_INDEX_BITS) - 1);
  }
}

#endif
