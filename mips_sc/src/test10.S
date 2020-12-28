#include "trap.h"
   .set noat
   .globl main
   .text
main:
   li $v1, 0x10010011          #$v1 = 0x10010011 
   li $v0, 0x0100101F          #$v0 = 0x0100101F 
   and $t0, $v0, $v1           #$t0 = 0x11 dec = 17
   sll $0, $0,0                #nop
   srl $t1, $v0, 0x04          #$t1 = 0x00100101
   add $t2, $v0, $v1           #$t2 = 0x11011030 
   sra $s1, $t2, 0x08          #$s1 = 0x00110110
   xor $t3, $s1, $t1           #$t3 = 0x00010011
   li $at, 0x08                #$at = 0x08
   sllv $s2, $v1, $at          #$s2 = 0x01001100
   srlv $s3, $v0, $at          #$s3 = 0x00010010
   and $t4, $s2, $s3           #$t4 = 0
   nor $t5, $s2, $s3           #$t5 = 0xfefeeeef
   srav $s4, $t5, $at          #$s4 = 0xfffefeee
   sub $t6, $s4, $s3           #$t6 = 0xfffdfede
   mthi $s4                    #$hi = 0xfffefeee
   mtlo $t6                    #$lo = 0xfffdfede
   sll $0, $0,0                #nop
   sub $t7, $s4, $t6           #$t7 = 0x00101F00
   sltiu $t8, $t7, 0x1234      #$t8 = 0
   xori $t9, $t6, 0x1234       #$t9 = 0xfffdecea
   
   HIT_GOOD_TRAP		#stop temu
