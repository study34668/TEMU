#include "trap.h"
   .set noat
   .globl main
   .text
main:
   li $at, 0xFFFFFFFF           #$at = 0xFFFFFFFF
   li $v0, 0x01011111           #$v0 = 0x01011111
   and $t0, $at, $v0           #$t0 = 0x00001010
   sb $v0, 13($t0)
   lw $at, 12($t0)
   
   HIT_GOOD_TRAP		#stop temu
