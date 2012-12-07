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

#include <arch/x86_64/8259.h>
#include <arch/x86_64/apic.h>
#include <arch/x86_64/idtdesc.h>
#include <arch/x86_64/pit.h>
#include <arch/x86_64/rtc.h>
#include <lrt/assert.h>
#include <lrt/boot.h>
#include <lrt/eventImpl.h>
#include <lrt/mem.h>
#include <lrt/arch/x86_64/exceptionTable.h>

namespace {
  ebbos::IdtDesc idt[256];
}

void
ebbos::lrt::event::initArch(int nCores)
{
  for (int i = 0; i < 256; i++) {
    idt[i].set(0x8, exceptionTable[i]);
  }

  disablePit();
  disableRtc();

  Apic::getInstance().disableIrqs();

  altstack = reinterpret_cast<uintptr_t**>
    (mem::malloc(sizeof(uintptr_t*) * nCores, 0));
}

void __attribute__((noreturn))
ebbos::lrt::event::initCpuArch()
{
  //TODO: move these into headers
  class Idtr {
  public:
    uint16_t limit;
    uint64_t base;
  } __attribute__((packed));

  Idtr idtr;
  idtr.limit = (sizeof(IdtDesc) * 256) - 1;
  idtr.base = reinterpret_cast<uint64_t>(idt);
  asm volatile (
                "lidt %[idtr]"
                :
                : [idtr] "m" (idtr), "m" (idt[0])
                );

  Location loc = boot::smpLock + 1;
  Location* myLoc = reinterpret_cast<Location*>
    (mem::malloc(sizeof(Location), loc));
  *myLoc = loc;

  wrmsr(MSR_FS_BASE, uint64_t(myLoc));

  //We already enabled on the first processor to startup other cores
  if (getLocation() != 0) {
    Apic::getInstance().enableLapic();
  }

  char *stack = reinterpret_cast<char*>
    (mem::memalign(16, STACK_SIZE, getLocation()));

  altstack[getLocation()] = reinterpret_cast<uintptr_t*>
    (mem::malloc(STACK_SIZE, getLocation()));

  asm volatile (
                "mov %[stack], %%rsp;"
                "mov %[val], %[smpLock];"
                :
                : [stack] "r" (&stack[STACK_SIZE]),
                  [val] "r" (loc),
                  [smpLock] "m" (boot::smpLock)
                : "memory");

  boot::initCpu();
  dispatchLoop();
}

void __attribute__((noreturn))
ebbos::lrt::event::dispatchLoop()
{
  while(1)
    ;
}
