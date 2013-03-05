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
// TODO: -1 needed?
int StoreInstline(inst* ins, char* argument[], int addressingType[], int registerNum[])
{
	switch (ins->numOfOp)
	{
	case 0:
		CreateAddLine(ins, "", 0, -1, "" , 0, -1);
		break;
	case 1:
		CreateAddLine(ins, "" , 0, -1, argument[0], addressingType[0], registerNum[0]);		// Only Dest
		break;
	case 2:
		CreateAddLine(ins, argument[0], addressingType[0], registerNum[0], argument[1], addressingType[1], registerNum[1]);
		break;
	default:
		return 0;
		break;
	}
	return 1;
}

// This function creates and stores a line of an instruction.
int CreateAddLine(inst* ins, char op1Name[MAX_LINE], int op1AddType, int reg1, char op2Name[MAX_LINE], int op2AddType, int reg2)
{
	instlineArray[LIC].ins = ins;

    strcpy(instlineArray[LIC].op1.name, op1Name);
    instlineArray[LIC].op1.addrType = op1AddType;
    instlineArray[LIC].op1.reg = ((reg1 == -1) ? 0 : reg1);

    strcpy(instlineArray[LIC].op2.name, op2Name);
    instlineArray[LIC].op2.addrType = op2AddType;
    instlineArray[LIC].op2.reg = ((reg2 == -1) ? 0 : reg2);

	IC = IC + GetInstAdditionalLength(op1AddType) + GetInstAdditionalLength(op2AddType);
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

/*
 *   - dataArray (DATA/STRING handling)
 *   - refArray (ENTRY/EXTERN handling)
 *   */
