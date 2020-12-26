#include "trap.h"
   .set noat
   .globl main
   .text
main:
   lui $t0, 0xf84c
   ori $t0, $t0, 0x7352	# $t0 = 0xf84c7352
   sw $t0, 0x1000($zero)       # [0x1000] = 0xf84c7352
   lui $t1, 0xc209
   ori $t1, $t1, 0x9acb	# $t1 = 0xc2099acb
   sw $t1, 0x1004($zero)       # [0x1004] = 0xc2099acb
   sub $t2, $t0, $t1           # $t2 = 0x3642d887
   mult $t0, $t1               # $hi = 0x01dd321a $lo = 0x13e0c606
   mfhi $t3                    # $t3 = 0x01dd321a
   mflo $t4                    # $t4 = 0x13e0c606
   sra $t5, $t3, 7             # $t5 = 0x0003ba64
   nor $t6, $t5, $t2           # $t6 = 0xc9bc0518
   nor $t7, $t4, $t3           # $t7 = 0xec0209e1
   sh $t6, 0x1000($zero)       # [0x1000] = 0xf84c0518
   sh $t7, 0x1002($zero)       # [0x1000] = 0x09e10518
   sb $t5, 0x1004($zero)       # [0x1004] = 0xc2099a64
   sb $t4, 0x1006($zero)       # [0x1004] = 0xc2069a64
   lw $t8, 0x1000($zero)       # $t8 = 0x09e10518
   lw $t9, 0x1004($zero)       # $t9 = 0xc2069a64
   lh $s0, 0x1004($zero)       # $s0 = 0xffff9a64
   lhu $s1, 0x1006($zero)      # $s1 = 0x0000c206
   
   HIT_GOOD_TRAP		#stop temu
