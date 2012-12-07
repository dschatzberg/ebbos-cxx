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

#ifndef EBBOS_ARCH_X86_64_RTC_H
#define EBBOS_ARCH_X86_64_RTC_H

#include <arch/io.h>

namespace {
  const uint16_t CMOS_SELECT = 0x70;
  const uint16_t CMOS_REGISTER = 0x71;

  /* CMOS 0Bh - RTC - STATUS REGISTER B (read/write) */
  /* Bitfields for Real-Time Clock status register B: */
  /* Bit(s)  Description     (Table C002) */
  /*  7      enable cycle update */
  /*  6      enable periodic interrupt */
  /*  5      enable alarm interrupt */
  /*  4      enable update-ended interrupt */
  /*  3      enable square wave output */
  /*  2      Data Mode - 0: BCD, 1: Binary */
  /*  1      24/12 hour selection - 1 enables 24 hour mode */
  /*  0      Daylight Savings Enable - 1 enables */
  const uint8_t CMOS_STATUS_B = 0xb;
  const uint8_t CMOS_STATUS_B_INT_UPDATE_ENDED = 1 << 4;
  const uint8_t CMOS_STATUS_B_INT_ALARM = 1 << 5;
  const uint8_t CMOS_STATUS_B_INT_PERIODIC = 1 << 6;

  const uint8_t CMOS_STATUS_C = 0xc;
};

namespace ebbos {
  inline void
  disableRtc() {
    outb(CMOS_SELECT, CMOS_STATUS_B);
    uint8_t status_b = inb(CMOS_REGISTER);
    //Mask off the three interrupts
    status_b &= ~(CMOS_STATUS_B_INT_UPDATE_ENDED |
                  CMOS_STATUS_B_INT_ALARM |
                  CMOS_STATUS_B_INT_PERIODIC);
    outb(CMOS_SELECT, CMOS_STATUS_B);
    outb(CMOS_REGISTER, status_b);

    //We read register C to clear out pending interrupts
    outb(CMOS_SELECT, CMOS_STATUS_C);
  }
}
#endif
