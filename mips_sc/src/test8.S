#include "trap.h"
   .set noat
   .globl main
   .text
main:
   li $s0, 0x12345678
   li $s1, 0xffffffff
   li $s2, 0x00005678
   add $t0, $s0, $s0            # $t0 = 0x2468acf0
   add $t1, $s0, $s1            # $t1 = 0x12345677
   addi $t2, $s0, 0xffff        # $t2 = 0x12345677
   addu $t3, $s0, $s0           # $t3 = 0x2468acf0
   addiu $t4, $s0, 0x0fff       # $t4 = 0x12346677
   sub $t5, $s0, $s1            # $t5 = 0x12345679
   subu $t6, $s0, $s2           # $t6 = 0x12340000
   slt $t7, $s1, $s0            # $t7 = 0x00000001
   slti $s3, $s2, 0x5679        # $s3 = 0x00000001
   sltu $s4, $s0, $s1           # $s4 = 0x00000001
   sltiu $s5, $s2, 0xffff       # $s5 = 0x00000001
   slti $s6, $s2, 0xffff        # $s6 = 0x00000000
   add $s0, $t7, $s3            # $s0 = 0x00000002
   sub $s1, $t7, $s3            # $s1 = 0x00000000
   addi $s2, $zero, 0xffff      # $s2 = 0xffffffff
   addi $s7, $zero, 0x0001      # $s7 = 0x00000001
   sub $t8, $t7, $t6            # $t8 = 0xedcc0001
   subu $t9, $t7, $s3           # $t9 = 0x00000000
   slt $at, $s2, $zero          # $at = 0x00000001
   slti $v0, $s2, 0xfff0        # $v0 = 0x00000000
   
   HIT_GOOD_TRAP		#stop temu
