#include "helper.h"
#include "monitor.h"
#include "reg.h"

extern uint32_t instr;
extern char assembly[80];

/* decode I-type instrucion with unsigned immediate */
static void decode_imm_type(uint32_t instr) {

	op_src1->type = OP_TYPE_REG;
	op_src1->reg = (instr & RS_MASK) >> (RT_SIZE + IMM_SIZE);
	op_src1->val = reg_w(op_src1->reg);
	
	op_src2->type = OP_TYPE_IMM;
	op_src2->imm = instr & IMM_MASK;
	op_src2->val = op_src2->imm;

	op_dest->type = OP_TYPE_REG;
	op_dest->reg = (instr & RT_MASK) >> (IMM_SIZE);
}

make_helper(lui) {

	decode_imm_type(instr);
	reg_w(op_dest->reg) = (op_src2->val << 16);
	sprintf(assembly, "lui   %s,   0x%04x", REG_NAME(op_dest->reg), op_src2->imm);
}

make_helper(ori) {

	decode_imm_type(instr);
	reg_w(op_dest->reg) = op_src1->val | op_src2->val;
	sprintf(assembly, "ori   %s,   %s,   0x%04x", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), op_src2->imm);
}

make_helper(slti){

	decode_imm_type(instr);
	reg_w(op_dest->reg) = (int)op_src1->val < (int)op_src2->val;
	sprintf(assembly, "slti   %s,   %s,   0x%04x", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), op_src2->imm);
}

make_helper(sltiu){

	decode_imm_type(instr);
	reg_w(op_dest->reg) = op_src1->val < op_src2->val;
	sprintf(assembly, "sltiu   %s,   %s,   0x%04x", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), op_src2->imm);
}

make_helper(lw) {

	decode_imm_type(instr);
	reg_w(op_dest->reg) = mem_read(op_src1->val + (uint32_t)(((int)(op_src2->val << 16)) >> 16), 4);
	sprintf(assembly, "lw   %s,   0x%04x(%s)", REG_NAME(op_dest->reg), op_src2->imm, REG_NAME(op_src1->reg));
}

make_helper(sb) {

	decode_imm_type(instr);
	mem_write(op_src1->val + (uint32_t)(((int)(op_src2->val << 16)) >> 16), 1, reg_b(op_dest->reg));
	sprintf(assembly, "sb   %s,   0x%04x(%s)", REG_NAME(op_dest->reg), op_src2->imm, REG_NAME(op_src1->reg));
}

// I add ******************************************************************
make_helper(addi){

	decode_imm_type(instr);
	uint32_t result;
	if(op_src2->val >> 15 == 1){
		result = 0xFFFF0000 | op_src2->val;
	}else 
		result = op_src2->val;

	uint32_t tmp1 = 0x00000001 & (op_src1->val >> 31);
	uint32_t tmp2 = 0x00000001 & (op_src2->val >> 31);
	result = op_src1->val + result;
	
	if(result >> 31 != tmp1 && tmp1 == tmp2){
		printf("addi-i over flow\n");
		return;
	}
	reg_w(op_dest->reg) = result; 
	sprintf(assembly, "addi   %s,   %s,   0x%04x", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), op_src2->imm);
}

make_helper(andi){

	decode_imm_type(instr);
	reg_w(op_dest->reg) = (op_src1->val &  op_src2->val);
	sprintf(assembly, "andi   %s,   %s,   0x%04x", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), op_src2->imm);
}

make_helper(lb){

	decode_imm_type(instr);
	uint32_t extend_imm = op_src2->val >> 15 == 1? (0xFFFF0000 | op_src2->val) : op_src2->val;
	uint32_t addr = extend_imm + op_src1->val;
	uint32_t result =mem_read(addr, 1);
	result = (0x000000FF & result);
	result = result  >> 7 == 1? (0xFFFFFF00 | result) : result;
	sprintf(assembly, "lb   %s,   %d(%s)", REG_NAME(op_dest->reg), extend_imm,REG_NAME(op_src1->reg));
}

make_helper(lbu){

	decode_imm_type(instr);
	uint32_t extend_imm = op_src2->val >> 15 == 1? (0xFFFF0000 | op_src2->val) : op_src2->val;
	uint32_t addr = extend_imm + op_src1->val;
	uint32_t result =mem_read(addr, 1);
	result = (0x000000FF & result);
	sprintf(assembly, "lb   %s,   %d(%s)", REG_NAME(op_dest->reg), extend_imm,REG_NAME(op_src1->reg));
}

