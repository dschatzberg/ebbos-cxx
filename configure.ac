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

AC_PREREQ(2.61)
AC_INIT([EbbOS],[0.3],[sesa-developers@googlegroups.com])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([foreign subdir-objects -Wall -Werror -Wno-portability])
#some random file which we assert exists in source tree
AC_CONFIG_SRCDIR([src/lrt/boot.c++])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([Makefile])

AC_CANONICAL_HOST
AS_CASE([$host_cpu], [x86_64],
[AC_DEFINE([ARCH_X86], [1], [x86_64 architecture])],
[powerpc],
[AC_DEFINE([ARCH_PPC], [1], [powerpc architecture])],
[AC_MSG_ERROR([not on a supported architecture])])
AM_CONDITIONAL([ARCH_X86], [test x$host_cpu == xx86_64])
AM_CONDITIONAL([ARCH_POWERPC], [test x$host_cpu == xpowerpc])

# Checks for programs.
AM_PROG_AS
AC_PROG_CC
AC_PROG_CXX
AC_PROG_RANLIB

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.

AC_OUTPUT
