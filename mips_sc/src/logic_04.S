#include "trap.h"
   .set noat
   .globl main
   .text
main:
   li $at, 0x10100010           #$at = 0x10100010
   li $v0, 0x01011111           #$v0 = 0x01011111
   and $t0, $at, $v0            #$t0 = 0x00000010
   
   xor $t1, $at, $v0            #$t1 = 0x11111101
   xori $s0, $at, 0x1011        #$s0 = 0x10101001
   sw $s0, 4($t0)
   sltiu $s1, $v0, 0x1234       #$s1 = 0
   li $t2, 0x0011               #$t2 = 0x0011          
   srlv $s2, $at, $t2           #$s2 = 0x00000808
   mult $at, $v0                #$lo = 0x31211110    $hi = 0x00102122
   add $t3, $at, $v0
   sub $t4, $at, $v0
   multu $s2, $s0               #$lo = 0x20110100    $hi = 0x00001020
   mtlo $t1                     #$lo = 0x11111101
   
   HIT_GOOD_TRAP		#stop temu
