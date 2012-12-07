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

#include <algorithm>

#include <app/app.h>
#include <lrt/boot.h>
#include <lrt/console.h>
#include <lrt/event.h>
#include <lrt/mem.h>
#include <lrt/trans.h>
#include <arch/cpu.h>

namespace ebbos {
  namespace lrt {
    namespace boot {

      extern "C"
      void __attribute__((noreturn))
      init(void* archArg)
      {
        //I believe that multiboot will do this because we are an elf
        //kernel
        //clear bss
        // extern char sbss[];
        // extern char ebss[];
        // memset(sbss, 0, ebss-sbss);

        initArch(archArg);
        //call all c++ constructors
        // typedef void (*function_pointer) (void);
        // extern function_pointer start_ctors[];
        // Extern function_pointer end_ctors[];

        // for (int i = 0; i < (end_ctors - start_ctors); i++) {
        //   function_pointer cons = start_ctors[i];
        //   cons();
        // }

        //console should now be constructed
        Console::getInstance().write("Console Initialized\n");


        // uint32_t eax, ebx, ecx, edx;
        // cpuid(1, &eax, &ebx, &ecx, &edx);
        // printf("%#x %#x %#x %#x\n", eax, ebx, ecx, edx);
        // double pi = 3.1415926535898;
        // printf("pi is approximately %f\n", pi);

        // std::cout << "wrote using cout" << std::endl;

        int nCores = event::getNCores();
        mem::init(nCores);
        trans::init(nCores);
        event::init(nCores);

        initSmp(nCores);

        event::initCpu();
      }

      void __attribute__((noreturn))
      initCpu()
      {
        mem::initCpu();
        trans::initCpu();

        app::start();
      }

      //I hate this so much
      extern "C" void
      __cxa_pure_virtual (void)
      {
        while(1)
          ;
      }
    }
  }
}
