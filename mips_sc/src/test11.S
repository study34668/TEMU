#include "trap.h"
   .set noat
   .globl main
   .text
main:
   lui $a0, 0xc729
   ori $a0, $a0, 0x75aa	# $a0 = 0xc72975aa
   lui $a1, 0xa356
   ori $a1, $a1, 0x22c0	# $a1 = 0xa35622c0
   
   mult $a0, $a1
   mfhi $t0			# $t0 = 0x1492d077
   mflo $t1			# $t1 = 0xa5d4d380
   
   mult $t0, $t1
   mfhi $t2			# $t2 = 0xf8c0ea79
   mflo $t3			# $t3 = 0xa0c65080
   
   multu $t2, $t3
   mfhi $t4			# $t4 = 0x9c3945f9
   mflo $t5			# $t5 = 0xa3510c80
   
   multu $t4, $t5
   mfhi $t6			# $t6 = 0x63a9ed4d
   mflo $t7			# $t7 = 0x7ab3a880
   
   mult $t0, $t7
   mfhi $a2			# $a2 = 0x09dc678b
   mflo $a3			# $a3 = 0x1b6b5380
   
   mthi $a3			# $hi = 0x1b6b5380
   mtlo $a2			# $lo = 0x09dc678b
   
   
   HIT_GOOD_TRAP		#stop temu
