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

AM_CXXFLAGS = -std=c++0x -Wall -Werror
AM_CPPFLAGS = -I $(top_srcdir)/src \
	--include $(top_builddir)/config.h

bin_PROGRAMS = ebbos
ebbos_LDFLAGS = $(AM_LDFLAGS)
ebbos_LDADD = libebblrt.a $(LDADD)
ebbos_DEPENDENCIES = libebblrt.a

ebbos_SOURCES = \
	$(app_sources) \
	$(ebb_sources)

EXTRA_DIST =

noinst_LIBRARIES =

lib_LIBRARIES = libebblrt.a

libebblrt_a_SOURCES = $(arch_sources) $(lrt_sources)
libebblrt_a_CXXFLAGS = $(AM_CXXFLAGS) -fno-exceptions -fno-rtti \
	-fno-threadsafe-statics
AM_LDFLAGS = -u _start

if ARCH_X86
AM_LDFLAGS += -static -n -z max-page-size=0x1000 \
	-T $(top_srcdir)/src/lrt/arch/x86_64/link.ld

%.iso: % util/grub.cfg
	strip -d $< -o $<.stripped
	grub-mkrescue -o $@ -graft-points boot/ebbos=$<.stripped \
	boot/grub/grub.cfg=$(top_srcdir)/util/grub.cfg

endif

include src/arch/Makefile.am
include src/app/Makefile.am
include src/ebb/Makefile.am
include src/lrt/Makefile.am
include src/lib/Makefile.am
include src/sync/Makefile.am
