#include "trap.h"
   .set noat
   .globl main
   .text
main:
   li $v1, 0xf0000000           #$v1 = 0xf0000000
   li $v0, 0x0000000f           #$v0 = 0x0000000f
   add $t0, $v1, $v0            #$t0 = 0xf000000f
   addi $t2,$zero,0xFFFF	#$t2 = 0xffffffff
   addi $t3,$zero,0x0001	#$t3 = 0x00000001
   and  $t1,$t2,$t3		#$t1 = 0x00000001
   andi $t2, $t2, 0xf0f0 	#$t2 = 0x0000f0f0
   addi $v0, $v0, 1		#$v0 = 0x00000010
   sll  $t3, $t2, 4		#$t3 = 0x000f0f00
   mthi $v1			#$hi = 0xf0000000
   mtlo $t0			#$lo = 0xf000000f
   mfhi $t0			#$t0 = 0xf0000000
   mflo $v1			#$v1 = 0xf000000f
   sw $v1, 0($zero)		#0x00000000 0xf000000f
   lb $t0, 3($zero)		#$t0 = 0xfffffff0
   sb $v1, 4($zero)		#0x00000004 0x0f
   lb $t0, 4($zero)		#$t0 = 0x0000000f
   lbu $t0, 4($zero)		#$t0 = 0x0000000f
   add $t0, $t0, $v1		#$t0 = 0xf000001e
   
   HIT_GOOD_TRAP		#stop temu
