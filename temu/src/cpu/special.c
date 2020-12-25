#include "helper.h"
#include "monitor.h"

extern uint32_t instr;
extern char assembly[80];

/* invalid opcode */
make_helper(inv) {

	uint32_t temp;
	temp = instr_fetch(pc, 4);

	uint8_t *p = (void *)&temp;
	printf("invalid opcode(pc = 0x%08x): %02x %02x %02x %02x ...\n\n", 
			pc, p[3], p[2], p[1], p[0]);

	printf("There are two cases which will trigger this unexpected exception:\n\
1. The instruction at pc = 0x%08x is not implemented.\n\
2. Something is implemented incorrectly.\n", pc);
	printf("Find this pc value(0x%08x) in the disassembling result to distinguish which case it is.\n\n", pc);

	assert(0);
}

/* stop temu */
make_helper(temu_trap) {

	printf("\33[1;31mtemu: HIT GOOD TRAP\33[0m at $pc = 0x%08x\n\n", cpu.pc);

	temu_state = END;

}


make_helper(j){
	uint32_t instr_index = instr & 0x03ffffff;
	cpu.pc = (cpu.pc & 0xf0000000) | (instr_index << 2);
	cpu.pc = cpu.pc - 4;
}
make_helper(jal){
	reg_w(31) = cpu.pc + 8;
	uint32_t instr_index = instr & 0x03ffffff;
	cpu.pc = (cpu.pc & 0xf0000000) | (instr_index << 2);
	cpu.pc = cpu.pc - 4;
}
make_helper(syscall){
	SignalException(0,0x08);
}
make_helper(eret){
	uint32_t temp = cp0.status;
	uint32_t exl_mask = 0xfffffffd;
	temp = temp & exl_mask;
	cp0.status = temp;
}

