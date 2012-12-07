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

#define DEFAULT_FUNC(n) \
              .text ;\
              .align 2 ;\
              .globl defaultFunc ## n ;\
defaultFunc ## n: ;\
              mov $n, %r11 ;\
              jmp defaultFuncCommon

defaultFuncCommon:
              .cfi_startproc
              .cfi_lsda 0x03, .Llsda
              .cfi_personality 0x03, __gxx_personality_v0
              sub   $520, %rsp //move stack pointer down (with alignment)

              .cfi_adjust_cfa_offset 520
              fwait
              fxsave (%rsp)
              pushq %rax
              .cfi_adjust_cfa_offset 8
              pushq %r10
              .cfi_adjust_cfa_offset 8
              pushq %r9
              .cfi_adjust_cfa_offset 8
              pushq %r8
              .cfi_adjust_cfa_offset 8
              pushq %rcx
              .cfi_adjust_cfa_offset 8
              pushq %rdx
              .cfi_adjust_cfa_offset 8
              pushq %rsi
              .cfi_adjust_cfa_offset 8
              pushq %rdi
              .cfi_adjust_cfa_offset 8
              mov   %rsp, %rdi
              mov   %r11, %rsi
              subq  $16, %rsp
              .cfi_adjust_cfa_offset 16
              mov   %rsp, %rdx
              
.Lpre_precall:
              call  precall
.Lpost_precall:
              testq %rax, %rax
              je .Lfail

              mov   (520 + 8*8 + 16)(%rsp), %rdi
              call  altstackPush

              popq  %r11
              .cfi_adjust_cfa_offset -8
              addq  $8, %rsp
              .cfi_adjust_cfa_offset -8
              
              popq  %rdi
              .cfi_adjust_cfa_offset -8
              popq  %rsi
              .cfi_adjust_cfa_offset -8
              popq  %rdx
              .cfi_adjust_cfa_offset -8
              popq  %rcx
              .cfi_adjust_cfa_offset -8
              popq  %r8
              .cfi_adjust_cfa_offset -8
              popq  %r9
              .cfi_adjust_cfa_offset -8
              popq  %r10
              .cfi_adjust_cfa_offset -8
              popq  %rax
              .cfi_adjust_cfa_offset -8
              fxrstor (%rsp)
              add   $528, %rsp
              .cfi_adjust_cfa_offset -528

.Lpre_foo_call:
              callq *%r11
.Lpost_foo_call:
              pushq %rax
              .cfi_adjust_cfa_offset 8
              call  altstackPop
              popq  %rdi
              .cfi_adjust_cfa_offset -8
              pushq %rax
              .cfi_adjust_cfa_offset 8
              jmp   postcall
.Lfail:
              popq  %rax
              .cfi_adjust_cfa_offset -8
              addq $(520 + 8*8 + 16), %rsp
              .cfi_adjust_cfa_offset -(520 + 8*8 + 16)
              ret
.Llp0:
              mov   %rax, %rdi
              call  __cxa_begin_catch
              call altstackPop
              pushq %rax
.Lpre_rethrow:
              call  __cxa_rethrow
.Lpost_rethrow:
              mov   %rax, 0x8(%rsp)
              callq __cxa_end_catch
              mov   0x8(%rsp), %rax
              mov   %rax, %rdi
.Lpre_unwind:
              callq _Unwind_Resume
.Lpost_unwind:
              .cfi_endproc

              .section .gcc_except_table
.Llsda:
              .byte 0xff //DW_EH_PE_omit (no encoding of landing pad base)
              .byte 0x03 //DW_EH_PE_udata4 encoding of entries in type
              //table
              .uleb128 (.Ltypes_table - . ) //types table distance from here
              .byte 0x01 //DW_EH_PE_uleb128 (encoding of call-site
              //table)
              .uleb128 (.Lcall_site_end - .Lcall_site_begin) //length of call-site table
.Lcall_site_begin:
              .uleb128 (.Lpre_precall - defaultFuncCommon)
              .uleb128 (.Lpost_precall - .Lpre_precall)
              .uleb128 0x00
              .uleb128 0x00

              .uleb128 (.Lpre_foo_call - defaultFuncCommon) //offset to call site
              .uleb128 (.Lpost_foo_call - .Lpre_foo_call) //length
              .uleb128 (.Llp0 - defaultFuncCommon) //offset to landing pad
              .uleb128 (1) //1 plus byte offset into action table

              .uleb128 (.Lpre_rethrow - defaultFuncCommon)
              .uleb128 (.Lpost_rethrow - .Lpre_rethrow)
              .uleb128 (.Lpost_rethrow - defaultFuncCommon)
              .uleb128 0x00 //no action

              .uleb128 (.Lpre_unwind - defaultFuncCommon)
              .uleb128 (.Lpost_unwind - .Lpre_unwind)
              .uleb128 0x00 //no landing pad, just resume
              .uleb128 0x00
.Lcall_site_end:
              //action table
              .sleb128 0x01 //offset into types table
              .sleb128 0x00 //no further actions

              //types table
              .long 0x00
.Ltypes_table:
              .byte 0x00
              

DEFAULT_FUNC(0) 
DEFAULT_FUNC(1)
DEFAULT_FUNC(2)
DEFAULT_FUNC(3)
DEFAULT_FUNC(4)
DEFAULT_FUNC(5)
DEFAULT_FUNC(6)
DEFAULT_FUNC(7)
DEFAULT_FUNC(8)
DEFAULT_FUNC(9)

DEFAULT_FUNC(10)
DEFAULT_FUNC(11)
DEFAULT_FUNC(12)
DEFAULT_FUNC(13)
DEFAULT_FUNC(14)
DEFAULT_FUNC(15)
DEFAULT_FUNC(16)
DEFAULT_FUNC(17)
DEFAULT_FUNC(18)
DEFAULT_FUNC(19)

DEFAULT_FUNC(20)
DEFAULT_FUNC(21)
DEFAULT_FUNC(22)
DEFAULT_FUNC(23)
DEFAULT_FUNC(24)
DEFAULT_FUNC(25)
DEFAULT_FUNC(26)
DEFAULT_FUNC(27)
DEFAULT_FUNC(28)
DEFAULT_FUNC(29)

DEFAULT_FUNC(30)
DEFAULT_FUNC(31)
DEFAULT_FUNC(32)
DEFAULT_FUNC(33)
DEFAULT_FUNC(34)
DEFAULT_FUNC(35)
DEFAULT_FUNC(36)
DEFAULT_FUNC(37)
DEFAULT_FUNC(38)
DEFAULT_FUNC(39)

DEFAULT_FUNC(40)
DEFAULT_FUNC(41)
DEFAULT_FUNC(42)
DEFAULT_FUNC(43)
DEFAULT_FUNC(44)
DEFAULT_FUNC(45)
DEFAULT_FUNC(46)
DEFAULT_FUNC(47)
DEFAULT_FUNC(48)
DEFAULT_FUNC(49)

DEFAULT_FUNC(50)
DEFAULT_FUNC(51)
DEFAULT_FUNC(52)
DEFAULT_FUNC(53)
DEFAULT_FUNC(54)
DEFAULT_FUNC(55)
DEFAULT_FUNC(56)
DEFAULT_FUNC(57)
DEFAULT_FUNC(58)
DEFAULT_FUNC(59)

DEFAULT_FUNC(60)
DEFAULT_FUNC(61)
DEFAULT_FUNC(62)
DEFAULT_FUNC(63)
DEFAULT_FUNC(64)
DEFAULT_FUNC(65)
DEFAULT_FUNC(66)
DEFAULT_FUNC(67)
DEFAULT_FUNC(68)
DEFAULT_FUNC(69)

DEFAULT_FUNC(70)
DEFAULT_FUNC(71)
DEFAULT_FUNC(72)
DEFAULT_FUNC(73)
DEFAULT_FUNC(74)
DEFAULT_FUNC(75)
DEFAULT_FUNC(76)
DEFAULT_FUNC(77)
DEFAULT_FUNC(78)
DEFAULT_FUNC(79)

DEFAULT_FUNC(80)
DEFAULT_FUNC(81)
DEFAULT_FUNC(82)
DEFAULT_FUNC(83)
DEFAULT_FUNC(84)
DEFAULT_FUNC(85)
DEFAULT_FUNC(86)
DEFAULT_FUNC(87)
DEFAULT_FUNC(88)
DEFAULT_FUNC(89)

DEFAULT_FUNC(90)
DEFAULT_FUNC(91)
DEFAULT_FUNC(92)
DEFAULT_FUNC(93)
DEFAULT_FUNC(94)
DEFAULT_FUNC(95)
DEFAULT_FUNC(96)
DEFAULT_FUNC(97)
DEFAULT_FUNC(98)
DEFAULT_FUNC(99)

DEFAULT_FUNC(100)
DEFAULT_FUNC(101)
DEFAULT_FUNC(102)
DEFAULT_FUNC(103)
DEFAULT_FUNC(104)
DEFAULT_FUNC(105)
DEFAULT_FUNC(106)
DEFAULT_FUNC(107)
DEFAULT_FUNC(108)
DEFAULT_FUNC(109)

DEFAULT_FUNC(110)
DEFAULT_FUNC(111)
DEFAULT_FUNC(112)
DEFAULT_FUNC(113)
DEFAULT_FUNC(114)
DEFAULT_FUNC(115)
DEFAULT_FUNC(116)
DEFAULT_FUNC(117)
DEFAULT_FUNC(118)
DEFAULT_FUNC(119)

DEFAULT_FUNC(120)
DEFAULT_FUNC(121)
DEFAULT_FUNC(122)
DEFAULT_FUNC(123)
DEFAULT_FUNC(124)
DEFAULT_FUNC(125)
DEFAULT_FUNC(126)
DEFAULT_FUNC(127)
DEFAULT_FUNC(128)
DEFAULT_FUNC(129)

DEFAULT_FUNC(130)
DEFAULT_FUNC(131)
DEFAULT_FUNC(132)
DEFAULT_FUNC(133)
DEFAULT_FUNC(134)
DEFAULT_FUNC(135)
DEFAULT_FUNC(136)
DEFAULT_FUNC(137)
DEFAULT_FUNC(138)
DEFAULT_FUNC(139)

DEFAULT_FUNC(140)
DEFAULT_FUNC(141)
DEFAULT_FUNC(142)
DEFAULT_FUNC(143)
DEFAULT_FUNC(144)
DEFAULT_FUNC(145)
DEFAULT_FUNC(146)
DEFAULT_FUNC(147)
DEFAULT_FUNC(148)
DEFAULT_FUNC(149)

DEFAULT_FUNC(150)
DEFAULT_FUNC(151)
DEFAULT_FUNC(152)
DEFAULT_FUNC(153)
DEFAULT_FUNC(154)
DEFAULT_FUNC(155)
DEFAULT_FUNC(156)
DEFAULT_FUNC(157)
DEFAULT_FUNC(158)
DEFAULT_FUNC(159)

DEFAULT_FUNC(160)
DEFAULT_FUNC(161)
DEFAULT_FUNC(162)
DEFAULT_FUNC(163)
DEFAULT_FUNC(164)
DEFAULT_FUNC(165)
DEFAULT_FUNC(166)
DEFAULT_FUNC(167)
DEFAULT_FUNC(168)
DEFAULT_FUNC(169)

DEFAULT_FUNC(170)
DEFAULT_FUNC(171)
DEFAULT_FUNC(172)
DEFAULT_FUNC(173)
DEFAULT_FUNC(174)
DEFAULT_FUNC(175)
DEFAULT_FUNC(176)
DEFAULT_FUNC(177)
DEFAULT_FUNC(178)
DEFAULT_FUNC(179)

DEFAULT_FUNC(180)
DEFAULT_FUNC(181)
DEFAULT_FUNC(182)
DEFAULT_FUNC(183)
DEFAULT_FUNC(184)
DEFAULT_FUNC(185)
DEFAULT_FUNC(186)
DEFAULT_FUNC(187)
DEFAULT_FUNC(188)
DEFAULT_FUNC(189)

DEFAULT_FUNC(190)
DEFAULT_FUNC(191)
DEFAULT_FUNC(192)
DEFAULT_FUNC(193)
DEFAULT_FUNC(194)
DEFAULT_FUNC(195)
DEFAULT_FUNC(196)
DEFAULT_FUNC(197)
DEFAULT_FUNC(198)
DEFAULT_FUNC(199)

DEFAULT_FUNC(200)
DEFAULT_FUNC(201)
DEFAULT_FUNC(202)
DEFAULT_FUNC(203)
DEFAULT_FUNC(204)
DEFAULT_FUNC(205)
DEFAULT_FUNC(206)
DEFAULT_FUNC(207)
DEFAULT_FUNC(208)
DEFAULT_FUNC(209)

DEFAULT_FUNC(210)
DEFAULT_FUNC(211)
DEFAULT_FUNC(212)
DEFAULT_FUNC(213)
DEFAULT_FUNC(214)
DEFAULT_FUNC(215)
DEFAULT_FUNC(216)
DEFAULT_FUNC(217)
DEFAULT_FUNC(218)
DEFAULT_FUNC(219)

DEFAULT_FUNC(220)
DEFAULT_FUNC(221)
DEFAULT_FUNC(222)
DEFAULT_FUNC(223)
DEFAULT_FUNC(224)
DEFAULT_FUNC(225)
DEFAULT_FUNC(226)
DEFAULT_FUNC(227)
DEFAULT_FUNC(228)
DEFAULT_FUNC(229)

DEFAULT_FUNC(230)
DEFAULT_FUNC(231)
DEFAULT_FUNC(232)
DEFAULT_FUNC(233)
DEFAULT_FUNC(234)
DEFAULT_FUNC(235)
DEFAULT_FUNC(236)
DEFAULT_FUNC(237)
DEFAULT_FUNC(238)
DEFAULT_FUNC(239)

DEFAULT_FUNC(240)
DEFAULT_FUNC(241)
DEFAULT_FUNC(242)
DEFAULT_FUNC(243)
DEFAULT_FUNC(244)
DEFAULT_FUNC(245)
DEFAULT_FUNC(246)
DEFAULT_FUNC(247)
DEFAULT_FUNC(248)
DEFAULT_FUNC(249)

DEFAULT_FUNC(250)
DEFAULT_FUNC(251)
DEFAULT_FUNC(252)
DEFAULT_FUNC(253)
DEFAULT_FUNC(254)
DEFAULT_FUNC(255)
              