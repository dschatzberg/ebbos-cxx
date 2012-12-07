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

#include <cstdint>

#include <arch/io.h>
#include <arch/x86_64/acpi.h>
#include <arch/x86_64/apic.h>
#include <lrt/assert.h>
#include <lrt/mem.h>
#include <sync/compiler.h>

ebbos::Apic::Apic()
{
  //Check that we actually have a lapic
  uint32_t features, dummy;
  cpuid(CPUID_FEATURES, &dummy, &dummy, &dummy, &features);
  lrtAssert(features & CPUID_EDX_HAS_LAPIC);

  //disable 8259 by masking IRQs on master and slave
  outb(0x21, 0xff);
  outb(0xa1, 0xff);

  nIoApics = Acpi::getInstance().getNIoApics();
  ioApics = reinterpret_cast<IoApic**>
    (lrt::mem::malloc(sizeof(IoApic) * nIoApics, 0));
  for (int i = 0; i < nIoApics; i++) {
    const Acpi::MadtIoApic* madtIoApic = Acpi::getInstance().findIoApic(i);
    ioApics[i] = reinterpret_cast<IoApic*>(madtIoApic->addr);
  }
}

void
ebbos::Apic::disableIrqs()
{
  IoApic::IoApicRedEntry entry;
  entry.disable();

  for (int i = 0; i < nIoApics; i++) {
    //how many entries?
    int entries = ioApics[i]->maxRedEntry() + 1;

    //disable all entries
    for (int j = 0; j < entries; j++) {
      ioApics[i]->writeEntry(j, entry);
    }
  }
}

void
ebbos::Apic::enableLapic()
{
  Lapic::enable();
}

uint32_t
ebbos::Apic::IoApic::read(int index)
{
  ioRegSel = index;
  reorder_barrier();
  return access_once(ioWin);
}

void
ebbos::Apic::IoApic::write(int index, uint32_t val)
{
  ioRegSel = index;
  reorder_barrier();
  ioWin = val;
}

ebbos::Apic::IoApic::IoApicVersionRegister
ebbos::Apic::IoApic::readVersion()
{
  return read(IO_APIC_VER);
}

int
ebbos::Apic::IoApic::maxRedEntry()
{
  return readVersion().maxRedirEntry;
}

void
ebbos::Apic::IoApic::writeEntry(int i, IoApicRedEntry entry)
{
  write(IO_APIC_REDTBL_START + (i * 2), entry & 0xFFFFFFFFF);
  write(IO_APIC_REDTBL_START + (i * 2) + 1, (entry >> 32) & 0xFFFFFFFFF);
}

ebbos::Apic::IoApic::IoApicRedEntry::IoApicRedEntry() : raw(0)
{
}

void
ebbos::Apic::IoApic::IoApicRedEntry::disable()
{
  interruptMask = 1;
}

void
ebbos::Apic::Lapic::enable()
{
  uint64_t base = rdmsr(MSR_APIC_BASE);
  base |= 1 << 11;
  wrmsr(MSR_APIC_BASE, base);

  Lapic* lapic = reinterpret_cast<Lapic*>
    (base & ((1 << 12) - 1));

  lapic->swEnable();
}

void
ebbos::Apic::Lapic::swEnable()
{
  access_once(sivr).swEnable = 1;
}
