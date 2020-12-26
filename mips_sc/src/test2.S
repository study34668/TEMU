#include "trap.h"
   .set noat
   .globl main
   .text
main:
   li $at, 0x0FFFFFFF           #$at = 0x0FFFFFFF
   li $v0, 0x01011111           #$v0 = 0x01011111
   addi $s0, $zero, 0x1000
   slt $t0, $at, $v0            #$t0 = 0x0
   sb $t0, 0($s0)
   slti $t0, $s0, 0x2000        #$t0 = 0x1
   sb $t0, 1($s0)
   sll $t2, $at, 4
   sltu $t0, $t2, $v0           #$t0 = 0x0
   sb $t0, 2($s0)
   slt $t0, $t2, $v0            #$t0 = 0x1
   sb $t0, 3($s0) 
   or $t0, $at, $v0             #$t0 = 0x0FFFFFFF
   sw $t0, 4($s0)
   ori $t0, $at, 0xFFFF         #$t0 = 0x0FFFFFFF
   sw $t0, 8($s0)
   sll $t2, $at, 4
   srl $t0, $t2, 16             #$t0 = 0xFFFFFFFF
   sw $t0, 12($s0)
   addi $t1, $t1, 0x10
   srlv $t0, $t2, $t1           #$t0 = 0x0000FFFF
   sw $t0, 16($s0)
   mthi $t2
   mfhi $t0                     #$t0 = 0xFFFFFFF0
   sw $t0, 20($s0)
   lw $t0, 0($s0)               #$t0 = 0x01000100
   sw $t0, 24($s0)
   
   HIT_GOOD_TRAP		#stop temu
