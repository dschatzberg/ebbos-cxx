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

#ifndef EBBOS_ARCH_X86_64_MULTIBOOT_H
#define EBBOS_ARCH_X86_64_MULTIBOOT_H

#include <cstdint>

namespace ebbos {
  class MultibootInformation {
  public:
    struct {
      uint32_t hasMem :1;
      uint32_t hasBootDevice :1;
      uint32_t hasCommandLine :1;
      uint32_t hasBootModules :1;
      uint32_t hasAOutSymbolTable :1;
      uint32_t hasElfSectionTable :1;
      uint32_t hasMemMap :1;
      uint32_t hasDrives :1;
      uint32_t hasRomConfigurationTable :1;
      uint32_t hasBootLoaderName :1;
      uint32_t hasApmTable :1;
      uint32_t hasGraphicsTable :1;
    uint32_t :20;
    };
    uint32_t memoryLower;
    uint32_t memoryHigher;
    uint32_t bootDevice;
    uint32_t commandLine;
    uint32_t modulesCount;
    uint32_t modulesAddress;
    uint32_t symbols[3];
    uint32_t memoryMapLength;
    uint32_t memoryMapAddress;
    uint32_t drivesLength;
    uint32_t drivesAddress;
    uint32_t configurationTable;
    uint32_t bootLoaderName;
    uint32_t apmTable;
    uint32_t vbeControlInfo;
    uint32_t vbeModeInfo;
    uint32_t vbeMode;
    uint32_t vbeInterfaceSegment;
    uint32_t vbeInterfaceOffset;
    uint32_t vbeInterfaceLength;
  };
  class MultibootMemoryRegion {
  public:
    uint32_t size;
    uint64_t baseAddress;
    uint64_t length;
    uint32_t type;
    static const uint32_t RAM_TYPE = 1;
  } __attribute__((packed));
};

#endif
