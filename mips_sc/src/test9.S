#include "trap.h"
   .set noat
   .globl main
   .text
main:
   lui $a0, 0xc729
   ori $a0, $a0, 0x75aa	# $a0 = 0xc72975aa
   lui $a1, 0xa356
   ori $a1, $a1, 0x22c0	# $a1 = 0xa35622c0
   ori $a2, $a2, 0xffff	# $a2 = 0x0000ffff
   
   andi $t1, $a0, 0xfff0	# $t1 = 0x000075a0
   andi $t2, $a0, 0x5769	# $t2 = 0x00005528
   and $t3, $a0, $a1		# $t3 = 0x83002080
   and $t4, $a2, $a1		# $t4 = 0x000022c0
   
   ori $t5, $a0, 0xfff0	# $t5 = 0xc7a9fffa
   ori $t6, $a1, 0x5769	# $t6 = 0xa35677e9
   or $t7, $a0, $a1		# $t7 = 0xe77f77ea
   or $t8, $a2, $a1		# $t8 = 0xa356ffff
   
   xori $s1, $a0, 0xfff0	# $s1 = 0xc7298a5a
   xori $s2, $a0, 0x5769	# $s2 = 0xc72922c3
   xor $s3, $a0, $a1		# $s3 = 0x647f576a
   xor $s4, $a2, $a1		# $s4 = 0xa356dd3f
   
   nor $s5, $a0, $a1		# $s5 = 0x18808815
   nor $s6, $a2, $a1		# $s6 = 0x5ca90000
   
   and $s7, $a0, $a0		# $s7 = 0xc72975aa
   or $s8, $a0, $a0		# $s8 = 0xc72975aa
   xor $a3, $a0, $a0		# $a3 = 0x00000000
   
   HIT_GOOD_TRAP		#stop temu
