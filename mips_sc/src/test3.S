#include "trap.h"
   .set noat
   .globl main
   .text
main:
   li $v1, 0x00000004           #$v1 = 0x00000004
   li $v0, 0x00000006           #$v0 = 0x00000006
   add $t0, $v1, $v0            #$t0 = 0x0000000a
   addi $t2,$zero,0xFFFF	#$t2 = 0xffffffff
   addi $t3,$zero,0x0001	#$t3 = 0x00000001
   and  $t1,$t2,$t3		#$t1 = 0x00000001
   andi $t2, $t2, 0xf0f0 	#$t2 = 0x0000f0f0
   addi $v0, $v0, 26		#$v0 = 0x00000020
   sll  $t3, $t2, 4		#$t3 = 0x000f0f00
   sllv $t2, $t2, $v1		#$t2 = 0x000f0f00
   mthi $v1			#$hi = 0x00000004
   mtlo $t0			#$lo = 0x0000000a
   mfhi $t0			#$t0 = 0x00000004
   mflo $v1			#$v1 = 0x0000000a
   sw $t2, 0($zero)		#0x00000000 0x000f0f00
   lb $t0, 3($zero)		#$t0 = 0x00000000
   sb $v1, 4($zero)		#0x00000004 0x0000000a
   lb $t0, 4($zero)		#$t0 = 0x0000000a
   lbu $t0, 2($zero)		#$t0 = 0x0000000f
   add $t0, $t0, $v1		#$t0 = 0x00000019
   
   HIT_GOOD_TRAP		#stop temu
