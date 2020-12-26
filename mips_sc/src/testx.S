#include "trap.h"
   .set noat
   .globl main
   .text
main:
   addiu $t0, $zero, 0xfffe    # t0 = 0xffe -2
   sw $t0, 0x1008($zero)       # [0x1008] = 0xfffffffe -2
   lui $t0, 0xffff
   addiu $t0, $zero, 0xffb5	# $t0 = 0xffffffb5 -75
   sub $t1, $zero, $t0         # $t1 = 0x4b 75
   subu $t2, $t1, $t0          # $t2 = 0x96 150
   lh $t3, 0x1008($zero)       # $t3 = 0xfffffffe -2
   mult $t3, $t0               # $hi = 0x0, $lo = 0x96 150
   mflo $t3                    # $t3 = 0x96 150
   nor $t4, $t3, $t3           # $t4 = 0xffffff69 -151
   and $t5, $t4, $t3           # $t5 = 0
   sub $t6, $t3, $t2           # $t6 = 0
   lh $t7, 0x1008($zero)       # $t7 = -2
   sub $t7, $zero, $t7         # $t7 = 2
   srav $t7, $t3, $t7          # $t7 = 0x25 37
   sra $t8, $t7, 2             # $t8 = 9
   srav $t9, $t4, $t8          # $t9 = 0xffffffff -1
   lhu $s0, 0x1008($zero)      # $s0 = 0xfffe 65534
   sra $s1, $s0, 2             # $s1 = 0x3fff 16383
   sw $s1, 0x1008($zero)       # [0x1008] = 0x3fff
   
   
   HIT_GOOD_TRAP		#stop temu
