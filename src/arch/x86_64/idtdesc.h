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

#ifndef EBBOS_ARCH_X86_64_IDTDESC_HPP
#define EBBOS_ARCH_X86_64_IDTDESC_HPP

#include <cstdint>

namespace ebbos {
  class IdtDesc {
  public:
    enum SegType {
      INTERRUPT = 0xe,
      TRAP = 0xf
    };
    void set(uint16_t sel, void *addr,
             SegType t=INTERRUPT, int pl=0, int st=0) {
      offset_low = reinterpret_cast<uint64_t>(addr) & 0xFFFF;
      offset_high = reinterpret_cast<uint64_t>(addr) >> 16;
      selector = sel;
      ist = st;
      type = t;
      dpl = pl;
      p = 1;
    }
  private:
    union {
      uint64_t raw[2];
      struct {
        uint64_t offset_low :16;
        uint64_t selector :16;
        uint64_t ist :3;
        uint64_t :5;
        uint64_t type :4;
        uint64_t :1;
        uint64_t dpl :2;
        uint64_t p :1;
        uint64_t offset_high :48;
        uint64_t :32;
      } __attribute__((packed));
    };
  };
  static_assert(sizeof(IdtDesc) == 16, "packing issue");
}


#endif
