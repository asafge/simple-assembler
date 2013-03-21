/*
 * The instlineMem files (.h,c) are responsible for saving instruction lines to memory.
 */
#include <string.h>
#include "../config.h"
#include "instlineMem.h"

extern int IC;
extern instLine instlineArray[MAX_ARRAY_LENGTH];
extern int LIC;

// Restart global variables
void RestartInstlines()
{
	IC = 0;
	LIC = 0;
}

// Stores instructions in the instlineArray according to its structure.
// Uses CreateAddLine for the actual storing and updating of LIC.
int StoreInstline(inst* ins, operand* ops[MAX_ARGUMENTS], int comb, int type)
{
	int added = 1;
	instlineArray[LIC].ins = ins;

	if ((ins->numOfOp == 2) && (ops != NULL) && (ops[0] != NULL))
	{
		strcpy(instlineArray[LIC].op1.name, ops[0]->name);
		instlineArray[LIC].op1.addrType = ops[0]->addrType;
		instlineArray[LIC].op1.reg = ops[0]->reg;
		added += GetInstAdditionalLength(ops[0]->addrType);
	}

	if ((ops != NULL) && (ops[1] != NULL))
	{
		strcpy(instlineArray[LIC].op2.name, ops[1]->name);
		instlineArray[LIC].op2.addrType = ops[1]->addrType;
		instlineArray[LIC].op2.reg = ops[1]->reg;
		added += GetInstAdditionalLength(ops[1]->addrType);
	}

	instlineArray[LIC].comb = (comb != -1) ? comb: 0;
	instlineArray[LIC].type = (type != -1) ? type: 0;

	if (strcmp(ins->name,"entry") != 0) IC += added;			// Don't increment on entry
    LIC++;
    return 1;
}

// This function returns how many additional words this instruction takes, based on its addressing type
int GetInstAdditionalLength(int addressType)
{
	if (addressType == IMMEDIATE || addressType == LABEL || addressType == INDEX_REGISTER)
		return 1;
	else if (addressType == INDEX_LABLE || addressType == INDEX_DIRECT)
		return 2 ;
	else if (addressType == DIRECT_REG)
		return 0;
	return 0;
}
