#include "trap.h"
   .set noat
   .globl main
   .text
main:
   li $v1, 0x00000005           #$v1 = 0x00000005
   li $v0, 0x00000006           #$v0 = 0x00000006
   add $t0, $v1, $v0            #$t0 = 0x0000000b
   addi $t2,$zero,0xFFFF	#$t2 = 0xffffffff
   addi $t3,$zero,0x0001	#$t3 = 0x00000001
   and  $t1,$t2,$t3		#$t1 = 0x00000001
   andi $t2, $t2, 0xf0f0 	#$t2 = 0x0000f0f0
   addi $v0, $v0, 26		#$v0 = 0x00000020
   sll  $t3, $t2, 32		
   sllv $t2, $t2, $v0		
   mthi $v1			
   mtlo $t0			
   mfhi $t0			
   mflo $v1			
   sw $t2, 0($zero)		
   lb $t0, 3($zero)		
   sb $v1, 4($zero)		
   lb $t0, 4($zero)		
   lbu $t0, 2($zero)		
   add $t0, $t0, $v1		
   
   HIT_GOOD_TRAP		#stop temu
