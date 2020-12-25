#ifndef __RTYPE_H__
#define __RTYPE_H__

make_helper(and);

make_helper(slt);
make_helper(sltu);
make_helper(or);
make_helper(srl);
make_helper(srlv);
make_helper(mthi);
make_helper(xor);
make_helper(mult);
make_helper(multu);

make_helper(add);
make_helper(addu);
make_helper(sll);
make_helper(sllv);
make_helper(mfhi);
make_helper(mtlo);

make_helper(sub);
make_helper(subu);
make_helper(nor);
make_helper(sra);
make_helper(srav);
make_helper(mflo);



make_helper(jr);
make_helper(jalr);
make_helper(mfc0);
make_helper(mtc0);
make_helper(div);
make_helper(divu);

#endif
