#include "trap.h"
#include <regdef.h>
   .org 0x0
   .set noreorder
   .set nomacro
   .set noat
   .globl main
   .text
main:
   addi  $at,$zero,0xff
   nop
   nop
   sb   $at,0x3($zero)       # [0x3] = 0xff
   nop
   nop
   HIT_GOOD_TRAP		#stop temu
