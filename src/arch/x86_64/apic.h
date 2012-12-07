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

#ifndef EBBOS_ARCH_X86_64_APIC_H
#define EBBOS_ARCH_X86_64_APIC_H

#include <arch/cpu.h>

namespace ebbos {
  class Apic {
  public:
    static Apic& getInstance()
    {
      static Apic instance;
      return instance;
    }
    void disableIrqs();
    void enableLapic();
  private:
    Apic();
    Apic(Apic const&);
    void operator=(Apic const&);
    class IoApic {
    public:
      uint32_t read(int index);
      void write(int index, uint32_t val);
      int id();
      int maxRedEntry();
      int apicVersion();
      class IoApicVersionRegister {
      public:
        IoApicVersionRegister(uint32_t i) : raw(i) {
        }
        union {
          uint32_t raw;
          struct {
            uint32_t version :8;
            uint32_t :8;
            uint32_t maxRedirEntry :8;
            uint32_t :8;
          } __attribute__((packed));
        };
      };
      IoApicVersionRegister readVersion();
      class IoApicRedEntry {
      public:
        IoApicRedEntry();
        void disable();
        operator uint64_t() {
          return raw;
        }
      private:
        union {
          uint64_t raw;
          struct {
            uint64_t intvec :8;
            uint64_t delmod :3;
            uint64_t destmod :1;
            uint64_t delivs :1;
            uint64_t intpol :1;
            uint64_t remoteIrr :1;
            uint64_t triggerMode :1;
            uint64_t interruptMask :1;
            uint64_t :39;
            uint64_t destination :8;
          };
        };
      } __attribute__((packed));
      IoApicRedEntry readEntry(int i);
      void writeEntry(int i, IoApicRedEntry entry);
      static const int IO_APIC_ID = 0;
      static const int IO_APIC_VER = 1;
      static const int IO_APIC_ARB = 2;
      static const int IO_APIC_REDTBL_START = 0x10;
      static const int IO_APIC_REDTBL_END = 0x3f;
    private:
      uint32_t ioRegSel;
      uint32_t ioWin __attribute__((aligned(16)));
    };
    IoApic** ioApics;
    int nIoApics;

    class Lapic {
    public:
      static void enable();
      void swEnable();
    private:
      class Sivr {
      public:
        union {
          uint32_t raw;
          struct {
            uint32_t vector :8;
            uint32_t swEnable :1;
            uint32_t focusCheck :1;
            uint32_t :2;
            uint32_t eoiSuppression :1;
            uint32_t :19;
          };
        };
      };
      union {
        uint32_t raw[256];
        struct {
          uint32_t reserved0 __attribute__((aligned(16)));
          uint32_t reserved1 __attribute__((aligned(16)));
          uint32_t lir __attribute__((aligned(16)));
          uint32_t lvr __attribute__((aligned(16)));
          uint32_t reserved2 __attribute__((aligned(16)));
          uint32_t reserved3 __attribute__((aligned(16)));
          uint32_t reserved4 __attribute__((aligned(16)));
          uint32_t reserved5 __attribute__((aligned(16)));
          uint32_t tpr __attribute__((aligned(16)));
          uint32_t apr __attribute__((aligned(16)));
          uint32_t ppr __attribute__((aligned(16)));
          uint32_t ler __attribute__((aligned(16)));
          uint32_t rrd __attribute__((aligned(16)));
          uint32_t ldr __attribute__((aligned(16)));
          uint32_t dfr __attribute__((aligned(16)));
          Sivr sivr __attribute__((aligned(16)));
          uint32_t isr_31_0 __attribute__((aligned(16)));
          uint32_t isr_63_32 __attribute__((aligned(16)));
          uint32_t isr_95_64 __attribute__((aligned(16)));
          uint32_t isr_127_96 __attribute__((aligned(16)));
          uint32_t isr_159_128 __attribute__((aligned(16)));
          uint32_t isr_191_160 __attribute__((aligned(16)));
          uint32_t isr_223_192 __attribute__((aligned(16)));
          uint32_t isr_255_224 __attribute__((aligned(16)));
          uint32_t tmr_31_0 __attribute__((aligned(16)));
          uint32_t tmr_63_32 __attribute__((aligned(16)));
          uint32_t tmr_95_64 __attribute__((aligned(16)));
          uint32_t tmr_127_96 __attribute__((aligned(16)));
          uint32_t tmr_159_128 __attribute__((aligned(16)));
          uint32_t tmr_191_160 __attribute__((aligned(16)));
          uint32_t tmr_223_192 __attribute__((aligned(16)));
          uint32_t tmr_255_224 __attribute__((aligned(16)));
          uint32_t irr_31_0 __attribute__((aligned(16)));
          uint32_t irr_63_32 __attribute__((aligned(16)));
          uint32_t irr_95_64 __attribute__((aligned(16)));
          uint32_t irr_127_96 __attribute__((aligned(16)));
          uint32_t irr_159_128 __attribute__((aligned(16)));
          uint32_t irr_191_160 __attribute__((aligned(16)));
          uint32_t irr_223_192 __attribute__((aligned(16)));
          uint32_t irr_255_224 __attribute__((aligned(16)));
          uint32_t esr __attribute__((aligned(16)));
          uint32_t reserved6 __attribute__((aligned(16)));
          uint32_t reserved7 __attribute__((aligned(16)));
          uint32_t reserved8 __attribute__((aligned(16)));
          uint32_t reserved9 __attribute__((aligned(16)));
          uint32_t reserved10 __attribute__((aligned(16)));
          uint32_t reserved11 __attribute__((aligned(16)));
          uint32_t lvt_cmci __attribute__((aligned(16)));
          uint32_t lil __attribute__((aligned(16)));
          uint32_t lih __attribute__((aligned(16)));
          uint32_t lvt_timer __attribute__((aligned(16)));
          uint32_t lvt_thermal __attribute__((aligned(16)));
          uint32_t lvt_pmc __attribute__((aligned(16)));
          uint32_t lvt_lint0 __attribute__((aligned(16)));
          uint32_t lvt_lint1 __attribute__((aligned(16)));
          uint32_t lvt_error __attribute__((aligned(16)));
          uint32_t init_count __attribute__((aligned(16)));
          uint32_t current_count __attribute__((aligned(16)));
          uint32_t reserved12 __attribute__((aligned(16)));
          uint32_t reserved13 __attribute__((aligned(16)));
          uint32_t reserved14 __attribute__((aligned(16)));
          uint32_t reserved15 __attribute__((aligned(16)));
          uint32_t dcr __attribute__((aligned(16)));
          uint32_t reserved16 __attribute__((aligned(16)));
        };
      };
    };
  };
}

#endif
