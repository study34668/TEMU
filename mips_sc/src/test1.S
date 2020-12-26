#include "trap.h"
   .set noat
   .globl main
   .text
main:
   li $at, 0xFFFFFFFF           #$at = 0xFFFFFFFF
   li $v0, 0x01011111           #$v0 = 0x01011111
   li $s0, 0x00001000
   slt $t0, $at, $v0            #$t0 = 0x1
   sw $t0, 0($s0)
   slti $t0, $at, 0x1234        #$t0 = 0x1
   sw $t0, 4($s0)
   sltu $t0, $at, $v0           #$t0 = 0x0
   sw $t0, 8($s0)
   li $t1, 0x10101010
   li $t2, 0x01010101
   or $t0, $t1, $t2             #$t0 = 0x11111111
   sw $t0, 12($s0)
   ori $t0, $t1, 0x1011         #$t0 = 0x10101011
   sw $t0, 16($s0)
   srl $t0, $t2, 4              #$t0 = 0x00101010
   sw $t0, 20($s0)
   mthi $v0                     #$hi = 0x01011111
   sw $v0, 24($s0)
   sb $v0, 27($s0)              #27($s0) = 0x11
   lw $s1, 24($s0)              #$s1 = 0x00000011
   
   HIT_GOOD_TRAP		#stop temu
