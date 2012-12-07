#
# Copyright (C) 2012 by Project SESA, Boston University
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#

lrt_arch_x86_64_sources = \
	src/lrt/arch/x86_64/bootAsm.S \
	src/lrt/arch/x86_64/boot.c++ \
	src/lrt/arch/x86_64/boot.h \
	src/lrt/arch/x86_64/defaultFunc.S \
	src/lrt/arch/x86_64/event.c++ \
	src/lrt/arch/x86_64/event.h \
	src/lrt/arch/x86_64/exceptionTable.h \
	src/lrt/arch/x86_64/isr.S \
	src/lrt/arch/x86_64/mem.c++ \
	src/lrt/arch/x86_64/trans.c++ \
	src/lrt/arch/x86_64/transImpl.h

if ARCH_X86

ebbos_DEPENDENCIES += src/lrt/arch/x86_64/link.ld

endif

EXTRA_DIST += src/lrt/arch/amd64/link.ld
