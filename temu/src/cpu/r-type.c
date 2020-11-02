#include "helper.h"
#include "monitor.h"
#include "reg.h"

extern uint32_t instr;
extern char assembly[80];

/* decode R-type instrucion */
static void decode_r_type(uint32_t instr) {

	op_src1->type = OP_TYPE_REG;
	op_src1->reg = (instr & RS_MASK) >> (RT_SIZE + IMM_SIZE);
	op_src1->val = reg_w(op_src1->reg);
	
	op_src2->type = OP_TYPE_REG;
	op_src2->imm = (instr & RT_MASK) >> (RD_SIZE + SHAMT_SIZE + FUNC_SIZE);
	op_src2->val = reg_w(op_src2->reg);

	op_dest->type = OP_TYPE_REG;
	op_dest->reg = (instr & RD_MASK) >> (SHAMT_SIZE + FUNC_SIZE);
}

make_helper(and) {

	decode_r_type(instr);
	reg_w(op_dest->reg) = (op_src1->val & op_src2->val);
	sprintf(assembly, "and   %s,   %s,   %s", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), REG_NAME(op_src2->reg));
}

make_helper(slt) {

	decode_r_type(instr);
	reg_w(op_dest->reg) = (int)op_src1->val < (int)op_src2->val;
	sprintf(assembly, "slt   %s,   %s,   %s", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), REG_NAME(op_src2->reg));
}

make_helper(sltu) {

	decode_r_type(instr);
	reg_w(op_dest->reg) = op_src1->val < op_src2->val;
	sprintf(assembly, "sltu   %s,   %s,   %s", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), REG_NAME(op_src2->reg));
}

make_helper(or) {

	decode_r_type(instr);
	reg_w(op_dest->reg) = op_src1->val | op_src2->val;
	sprintf(assembly, "or   %s,   %s,   %s", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), REG_NAME(op_src2->reg));
}

make_helper(xor){

	decode_r_type(instr);
	reg_w(op_dest->reg) = (~op_src1->val & op_src2->val) | (op_src1->val & (op_src2->val));
	sprintf(assembly, "xor   %s,   %s,   %s", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), REG_NAME(op_src2->reg));
}
make_helper(srl) {

	decode_r_type(instr);
	reg_w(op_dest->reg) = op_src2->val >> ((instr & SHAMT_MASK) >> FUNC_SIZE);
	sprintf(assembly, "srl   %s,   %s,   %d", REG_NAME(op_dest->reg), REG_NAME(op_src2->reg), (instr & SHAMT_MASK) >> FUNC_SIZE);
}

make_helper(srlv) {

	decode_r_type(instr);
	reg_w(op_dest->reg) = op_src2->val >> (op_src1->val & 0x1F);
	sprintf(assembly, "srlv   %s,   %s,   %s", REG_NAME(op_dest->reg), REG_NAME(op_src2->reg), REG_NAME(op_src1->reg));
}

make_helper(mthi) {

	decode_r_type(instr);
	cpu.hi = op_src1->val;
	sprintf(assembly, "mthi   %s", REG_NAME(op_src1->reg));
}

make_helper(mult){

	decode_r_type(instr);
	long tmp = (int)op_src1->val * (int)op_src2->val;
	cpu.lo = tmp & 0x00000000ffffffff;
	cpu.hi = tmp >> 0x20;
	sprintf(assembly, "mult   %s,   %s", REG_NAME(op_src2->reg), REG_NAME(op_src1->reg));
}
make_helper(multu){

	decode_r_type(instr);
	unsigned long tmp = op_src1->val * op_src2->val;
	cpu.lo = 0x00000000ffffffff & tmp;
	cpu.hi = tmp >> 0x20;
	sprintf(assembly, "multu   %s,   %s", REG_NAME(op_src2->reg), REG_NAME(op_src1->reg));
}
// I ADD *******************************************************************
make_helper(addu){

	decode_r_type(instr);
	reg_w(op_dest->reg) = (op_src1->val + op_src2->val);
	sprintf(assembly, "addu   %s,   %s,   %s", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), REG_NAME(op_src2->reg));
}

make_helper(add){

	decode_r_type(instr);
	uint32_t tmp1 = 0x80000000 & (op_src1->val);
	uint32_t tmp2 = 0x80000000 & (op_src2->val);
	uint32_t result = op_src1->val + op_src2->val;
	if(tmp1 == tmp2 && result >> 31 != tmp1){
		printf("add-r overflow\n");
		return ;	
	}
	reg_w(op_dest->reg) = result;
	sprintf(assembly, "add   %s,   %s,   %s", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), REG_NAME(op_src2->reg));
}

make_helper(sll){
	
	decode_r_type(instr);
	uint32_t tmp = (instr & SHAMT_MASK) >> (FUNC_SIZE);
	uint32_t result = op_src2->val << tmp;
	reg_w(op_dest->reg) = result;
	sprintf(assembly, "sll   %s,   %s,   %d", REG_NAME(op_dest->reg), REG_NAME(op_src2->reg), tmp);
}

make_helper(sllv){

	decode_r_type(instr);
	uint32_t tmp = op_src1->val & (0x0000001F);
	uint32_t result = op_src2->val << tmp;
	reg_w(op_dest->reg) = result;
	sprintf(assembly, "sllv   %s,   %s,   %s", REG_NAME(op_dest->reg), REG_NAME(op_src2->reg), REG_NAME(op_src1->reg));
}

make_helper(mfhi){

	decode_r_type(instr);
	reg_w(op_dest->reg)  = cpu.hi;
	sprintf(assembly, "mfhi   %s", REG_NAME(op_dest->reg));
}

make_helper(mtlo){

	decode_r_type(instr);
	cpu.lo = op_src1->val;
	sprintf(assembly,"mtlo   %s", REG_NAME(op_src1->reg));
}

//*************
make_helper(subu){

	decode_r_type(instr);
	reg_w(op_dest->reg) = (op_src1->val - op_src2->val);
	sprintf(assembly, "subu   %s,   %s,   %s", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), REG_NAME(op_src2->reg));
}

make_helper(sub){

	decode_r_type(instr);
	uint32_t tmp1 = 0x80000000 & (op_src1->val);
	uint32_t tmp2 = 0x80000000 & (op_src2->val);
	uint32_t result = op_src1->val - op_src2->val;
	if(tmp1 != tmp2 && result >> 31 != tmp1){
		printf("sub-r overflow\n");
		return ;	
	}
	reg_w(op_dest->reg) = result;
	sprintf(assembly, "sub   %s,   %s,   %s", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), REG_NAME(op_src2->reg));
}

make_helper(nor){

	decode_r_type(instr);
	reg_w(op_dest->reg) = ~(op_src1->val | op_src2->val);
	sprintf(assembly, "nor   %s,   %s,   %s", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), REG_NAME(op_src2->reg));
}

make_helper(sra){

	decode_r_type(instr);
	uint32_t result = op_src2->val;
	uint32_t s = (instr & SHAMT_MASK) >> FUNC_SIZE;
	int r = result;
	r = r >> s;
	result = r;
	reg_w(op_dest->reg) = result;
	sprintf(assembly, "sra   %s,   %s,   %d", REG_NAME(op_dest->reg), REG_NAME(op_src2->reg), (instr & SHAMT_MASK) >> FUNC_SIZE);
}

make_helper(srav){

	decode_r_type(instr);
	uint32_t result = op_src2->val;
	uint32_t s = op_src1->val & 0x1F;
	int r = result;
	r = r >> s;
	result = r;
	reg_w(op_dest->reg) = result;
	sprintf(assembly, "srav   %s,   %s,   %s", REG_NAME(op_dest->reg), REG_NAME(op_src2->reg), REG_NAME(op_src1->reg));
}

make_helper(mflo){

	decode_r_type(instr);
	reg_w(op_dest->reg)  = cpu.lo;
	sprintf(assembly, "mflo   %s", REG_NAME(op_dest->reg));
}
