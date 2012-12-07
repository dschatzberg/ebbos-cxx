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

#ifndef EBBOS_ARCH_CPU_H
#error "Don't include this file directly"
#endif

#include <cstdint>

namespace ebbos {
  const uint32_t CPUID_FEATURES = 1;

  // CPUID FEATURE FLAGS
  const uint32_t CPUID_EDX_HAS_LAPIC = 1 << 9;

  static inline void
  cpuid(uint32_t index, uint32_t *eax, uint32_t *ebx, uint32_t *ecx,
        uint32_t *edx)
  {
    __asm__ volatile (
                      "cpuid"
                      : "=a" (*eax), "=b" (*ebx), "=c" (*ecx), "=d" (*edx)
                      : "a" (index)
                      );
  }

  const uint32_t MSR_APIC_BASE = 0x1b;
  const uint32_t MSR_FS_BASE = 0xC0000100;

  static inline void
  wrmsr(uint32_t msr, uint64_t val) {
    asm volatile ("wrmsr"
                  :
                  : "d" ((uint32_t)(val >> 32)),
                    "a" ((uint32_t)(val & 0xFFFFFFFF)),
                    "c" (msr)
                  );
  }

  static inline uint64_t
  rdmsr(uint32_t msr) {
    uint32_t low, high;
    asm volatile ("rdmsr"
                  : "=a" (low), "=d" (high)
                  : "c" (msr)
                  );
    return (static_cast<uint64_t>(high) << 32) | low;
  }
}
