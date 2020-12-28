#include "trap.h"
   .set noat
   .globl main
   .text
main:
   li $at, 0x0FFFFFFF           #$at = 0x0FFFFFFF
   li $v0, 0x11111111           #$v0 = 0x11111111
   sw $at, 0($zero)		#0x00000000 0x0FFFFFFF
   sh $v0, 4($zero)		#0x00000004 0x00001111
   srl $t0, $v0, 24		#$t0 = 0x00000011
   sb $t0, 6($zero)		#0x00000004 0x00111111
   lw $t1, 0($zero)		#$t1 = 0x0FFFFFFF
   lh $t2, 0($zero)		#$t2 = 0xFFFFFFFF
   lb $t3, 3($zero)		#$t3 = 0x0000000F
   lbu $t4,0($zero)		#$t4 = 0x000000FF
   lhu $t5,0($zero)		#$t5 = 0x0000FFFF
   sw $t5, 7($zero)		#error
   sw $t5, 8($zero)		#0x00000008 0x0000FFFF
   sh $t5, 10($zero)		#0x00000008 0xFFFFFFFF
   lw $t6, 8($zero)		#$t6 = 0xFFFFFFFF
   lb $t7, 8($zero)		#$t7 = 0xFFFFFFFF
   addiu $t7, $zero, 0xFFFF    #$t7 = 0xFFFFFFFF
   sll $t7, $t7, 16		#$t7 = 0xFFFF0000
   lhu $t7, 8($zero)		#$t7 = 0x0000FFFF
   lbu $t7, 8($zero)		#$t7 = 0x000000FF
   srl $t0,$t0, 4		#$t0 = 0x00000001
   add $t0,$t0,$t6  		#$t6 = 0x00000000
   
   HIT_GOOD_TRAP		#stop temu
