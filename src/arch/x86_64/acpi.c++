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

#include <arch/x86_64/acpi.h>
#include <lrt/assert.h>
#include <lrt/boot.h>
#include <lrt/mem.h>

ebbos::Acpi::Acpi()
{
  rsdp = Rsdp::locate();

  if (rsdp != nullptr) {
    xsdt = rsdp->getXsdt();
    if (xsdt != nullptr &&
        reinterpret_cast<uintptr_t>(xsdt) <= 0xFFFFFFFF) {
        madt = reinterpret_cast<const Madt*>(xsdt->find("APIC"));
        srat = reinterpret_cast<const Srat*>(xsdt->find("SRAT"));
    } else {
      rsdt = rsdp->getRsdt();
      if (rsdt != nullptr &&
          reinterpret_cast<uintptr_t>(rsdt) <= 0xFFFFFFFF) {
        madt = reinterpret_cast<const Madt*>(rsdt->find("APIC"));
        srat = reinterpret_cast<const Srat*>(rsdt->find("SRAT"));
      }
    }
  }


  // lrtAssert(madt && reinterpret_cast<uintptr_t>(madt) <= 0xFFFFFFFF);
  // const MadtLapic* madtLapic;
  // coreInfos = reinterpret_cast<CoreInfo*>(lrt::mem::memstart);
  // int skip = 0;
  // for (int i = 0; (madtLapic = madt->findLapic(i)) != nullptr; i++) {
  //   if (madtLapic->enabled) {
  //     nCores++;
  //     int index = i - skip;
  //     coreInfos[index].proximityDomain = 0; //no NUMA information
  //     coreInfos[index].apicId = madtLapic->apicId;
  //   } else {
  //     skip++;
  //   }
  // }
  // lrt::mem::memstart += nCores * sizeof(CoreInfo);

  // if (srat != nullptr) {
  //   lrtAssert(reinterpret_cast<uintptr_t>(srat) <= 0xFFFFFFFF);

  //   const SratMemAffinity* sratMemAffinity;
  //   proximityDomains = reinterpret_cast<ProximityDomain*>(lrt::mem::memstart);
  //   int skip = 0;
  //   nProximityDomains = 0;
  //   for (int i = 0; (sratMemAffinity = srat->findMemAffinity(i)) != nullptr; i++) {
  //     if (sratMemAffinity) {
  //       nProximityDomains++;
  //       int index = i - skip;
  //       proximityDomains[index].baseAddress = sratMemAffinity->baseAddressLow |
  //         (uint64_t)sratMemAffinity->baseAddressHigh << 32;
  //       proximityDomains[index].length = sratMemAffinity->lengthLow |
  //         (uint64_t)sratMemAffinity->lengthHigh << 32;
  //       proximityDomains[index].proximityDomain = sratMemAffinity->proximityDomain;
  //       proximityDomains[index].nCores = 0;
  //     } else {
  //       skip ++;
  //     }
  //   }
  //   lrt::mem::memstart += nProximityDomains * sizeof(ProximityDomain);
  //   const SratLapic* sratLapic;
  //   skip = 0;
  //   for (int i = 0; (sratLapic = srat->findLapic(i)) != nullptr; i++) {
  //     if (sratLapic->enabled) {
  //       int index = i - skip;
  //       coreInfos[index].proximityDomain = sratLapic->proximityDomain_0_7 |
  //         sratLapic->proximityDomain_8_31[0] << 8 |
  //         sratLapic->proximityDomain_8_31[1] << 16 |
  //         sratLapic->proximityDomain_8_31[2] << 24;
  //       for (int j = 0; j < nProximityDomains; j++) {
  //         if (proximityDomains[j].proximityDomain ==
  //             coreInfos[index].proximityDomain) {
  //           proximityDomains[j].nCores++;
  //         }
  //       }
  //     } else {
  //       skip++;
  //     }
  //   }
  //   lrt::mem::memstart += nCores * sizeof(CoreInfo);
  // } else {
  //   proximityDomains = reinterpret_cast<ProximityDomain*>(lrt::mem::memstart);
  //   proximityDomains[0].baseAddress = 0;
  //   proximityDomains[0].length = 0xFFFFFFFFFFFFFFFF;
  //   proximityDomains[0].proximityDomain = 0;
  //   proximityDomains[0].nCores = nCores;
  //   nProximityDomains = 1;
  //   lrt::mem::memstart += sizeof(proximityDomains);
  // }
  // lrtAssert(lrt::boot::multibootInformation->hasMemMap);
  // for (int i = 0; i < nProximityDomains; i++) {
  //   ProximityDomain* pd = &proximityDomains[i];
  //   pd->trueLength = pd->length;
  //   char* baseAddress =
  //     reinterpret_cast<char*>(pd->baseAddress);
  //   pd->trueEnd = baseAddress + pd->length;
  //   if (lrt::mem::memstart > baseAddress) {
  //     pd->trueStart = lrt::mem::memstart;
  //     pd->trueLength -= lrt::mem::memstart - baseAddress;
  //   }
  //   else {
  //     pd->trueStart = baseAddress;
  //   }
  //   uint32_t memoryMapAddress =
  //     lrt::boot::multibootInformation->memoryMapAddress;
  //   for (unsigned int len = 0;
  //        len < lrt::boot::multibootInformation->memoryMapLength;) {
  //     MultibootMemoryRegion* r = reinterpret_cast<MultibootMemoryRegion*>
  //       (memoryMapAddress + len);
  //     if (r->type != MultibootMemoryRegion::RAM_TYPE) {
  //       uint64_t trueStart = reinterpret_cast<uint64_t>(pd->trueStart);
  //       uint64_t trueEnd = reinterpret_cast<uint64_t>(pd->trueEnd);
  //       if (r->baseAddress > trueStart &&
  //           r->baseAddress < trueEnd) {
  //         //We have a hole starting in this pd
  //         if ((r->baseAddress + r->length) < trueEnd) {
  //           //the hole in completely contained
  //           pd->trueLength -= r->length;
  //         } else {
  //           //the hole contains the end of our pd
  //           pd->trueLength -= trueEnd - r->baseAddress;
  //           pd->trueEnd = reinterpret_cast<char*>(r->baseAddress);
  //         }
  //       }
  //       else if (r->baseAddress <= trueStart &&
  //                r->baseAddress + r->length > trueStart) {
  //         //the hole starts at the beginning or before and end in our
  //         //pd
  //         //the hole shouldn't consume the entire pd
  //         lrtAssert(r->baseAddress + r->length < trueEnd);
  //         pd->trueLength -= r->baseAddress + r->length - trueStart;
  //         pd->trueStart = reinterpret_cast<char*>(r->baseAddress + r->length);
  //       }
  //     }
  //     len += r->length;
  //   }
  // }
}

int
ebbos::Acpi::getNCores() const
{
  lrtAssert(madt != nullptr &&
            reinterpret_cast<uintptr_t>(madt) <= 0xFFFFFFFF);
  const MadtLapic* madtLapic;
  int ret = 0;
  for (int i = 0; (madtLapic = madt->findLapic(i)) != nullptr; i++) {
    if (madtLapic->enabled) {
      ret++;
    }
  }
  return ret;
}

int
ebbos::Acpi::getNIoApics() const
{
  lrtAssert(madt != nullptr &&
            reinterpret_cast<uintptr_t>(madt) <= 0xFFFFFFFF);
  const MadtIoApic* madtIoApic;
  int ret = 0;
  for (int i = 0; (madtIoApic = madt->findIoApic(i)) != nullptr; i++) {
    ret++;
  }
  return ret;
}

namespace {
  const volatile uint16_t* BDA_EBDA =
    reinterpret_cast<volatile uint16_t*>(0x40e);
  const char* SECOND_SEARCH_LOCATION = reinterpret_cast<char*>(0xe0000);
  const int SECOND_SEARCH_SIZE = 0x20000;

}

const ebbos::Acpi::MadtIoApic*
ebbos::Acpi::findIoApic(int i) const {
  return madt->findIoApic(i);
}

const ebbos::Acpi::Rsdp*
ebbos::Acpi::Rsdp::locateInRange(const char* start, int length) {
  for (const char* p = start;
       p < (start + length);
       p += 16) {
    if (p[0] == 'R' &&
        p[1] == 'S' &&
        p[2] == 'D' &&
        p[3] == ' ' &&
        p[4] == 'P' &&
        p[5] == 'T' &&
        p[6] == 'R' &&
        p[7] == ' ') {
      return reinterpret_cast<const Rsdp*>(p);
    }
  }
  return nullptr;
}

const ebbos::Acpi::Rsdp*
ebbos::Acpi::Rsdp::locate() {
  char *ebda = reinterpret_cast<char *>
    (static_cast<uintptr_t>(*BDA_EBDA) << 4);
  const Rsdp* ret = nullptr;
  if((ret = locateInRange(ebda, 1 << 10)) == nullptr) {
    ret = locateInRange(SECOND_SEARCH_LOCATION, SECOND_SEARCH_SIZE);
  }
  return ret;
}

const ebbos::Acpi::Rsdt*
ebbos::Acpi::Rsdp::getRsdt() const
{
  return reinterpret_cast<Rsdt*>(rsdtAddress);
}

const ebbos::Acpi::Xsdt*
ebbos::Acpi::Rsdp::getXsdt() const
{
  return reinterpret_cast<Xsdt*>(xsdtAddress);
}

const ebbos::Acpi::MadtHeader*
ebbos::Acpi::Madt::find(uint8_t type, int index) const
{
  for (unsigned i = 0; i < (header.len-sizeof(Madt));) {
    const MadtHeader* h = reinterpret_cast<const MadtHeader*>(&data[i]);
    if (h->type == type) {
      if (index == 0) {
        return h;
      }
      index--;
    }
    i += h->len;
  }
  return nullptr;
}

const ebbos::Acpi::MadtLapic*
ebbos::Acpi::Madt::findLapic(int index) const
{
  return reinterpret_cast<const MadtLapic*>(find(MadtHeader::LAPIC_TYPE,
                                                 index));
}

const ebbos::Acpi::MadtIoApic*
ebbos::Acpi::Madt::findIoApic(int index) const
{
  return reinterpret_cast<const MadtIoApic*>(find(MadtHeader::IOAPIC_TYPE,
                                                  index));
}

const ebbos::Acpi::SratHeader*
ebbos::Acpi::Srat::find(uint8_t type, int index) const
{
  for (unsigned i = 0; i < (header.len-sizeof(Srat));) {
    const SratHeader* h = reinterpret_cast<const SratHeader*>(&data[i]);
    if (h->type == type) {
      if (index == 0) {
        return h;
      }
      index--;
    }
    i += h->len;
  }
  return nullptr;
}

const ebbos::Acpi::SratLapic*
ebbos::Acpi::Srat::findLapic(int index) const
{
  return reinterpret_cast<const SratLapic*>(find(SratHeader::LAPIC_TYPE,
                                                 index));
}

const ebbos::Acpi::SratMemAffinity*
ebbos::Acpi::Srat::findMemAffinity(int index) const
{
  return
    reinterpret_cast<const SratMemAffinity*>(find(SratHeader::MEM_AFFINITY_TYPE,
                                                  index));
}
