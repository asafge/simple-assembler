/*
 * The instructionParse files (.h,c) are responsible for the more lower-level
 * handling of the instructions, making sure they don't contain errors etc.
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "globalHelper.h"
#include "instructionHandler.h"
#include "instructionParse.h"
#include "mem/symbolMem.h"
#include "mem/instlineMem.h"
#include "mem/datastringMem.h"

// This function handles the VirtualInstruction .DATA [#],[#],...
int HandleDataInstruction(char* data)
{
	char* num = strtok(data, ",");
	if (!num)
		return PrintSaveError("", "no data provided");

	while (num != NULL)							// Read all numbers
	{
		int i = FirstNonWhitespace(num, 0);
		if (!IsNumber(&num[i]))
			return PrintSaveError(&num[i], "not numeric");
		else
		{
			StoreNumber(atoi(&num[i]));
			num = strtok (NULL, ",");			// Get the next number from the list
		}
	}
	return 1;
}

// This function handles the VirtualInstruction .STRING "..."
int HandleStringInstruction(char* data)
{
	int len = strlen(data);
	if (!len)
		return PrintSaveError(data, "no data provided");

	int i = FirstNonWhitespace(data, 0);		// Ignore white spaces

	if (data[i] != '"')							// Make sure it's the beginning of a string
		return PrintSaveError(data, "first char is not \"");

	i++;										// Skip " char
	char* innerStr = GetWord(&data[i],'\"');
	if (!innerStr || (strlen(innerStr) == 0))	// Make sure the string ends and has length
		return PrintSaveError(data, "not a valid string");

	StoreString(innerStr);
	free(innerStr);
	return 1;
}

// This function handles the instruction .EXTERN
// It just saves the label in the symbols memory.
int HandleExternInstruction(char* label)
{
	return SaveLeadingLabel(label, (short)CODE, (short)1, (short)0, (short)0);
}

// This function handles the instruction .ENTRY
// It saves the label as an "instruction" for phase 2
int HandleEntryInstruction(char* label)
{
	char* slabel = GetWord(label,'\0');

	operand* ops[MAX_ARGUMENTS];
	for (int k=0; k<MAX_ARGUMENTS; k++)
		ops[k] = calloc(1, sizeof(operand));

	strcpy(ops[0]->name, "");
	ops[0]->addrType = LABEL;
	ops[0]->reg = 0;

	strcpy(ops[1]->name, slabel);
	ops[1]->addrType = LABEL;
	ops[1]->reg = 0;

	int flag = StoreInstline(GetInstByName("entry"), &ops, -1, -1);
	if (!flag)
	{
		free(slabel);
		for (int k=0; k<MAX_ARGUMENTS; k++)
			free(ops[k]);
	}
	return flag;
}

// This function handles opcode instructions
int HandleOpInstruction(char* line, inst* ins)
{
	// Save line
	char* temp = calloc(MAX_LINE, sizeof(char));
	strcpy(temp, line);

	int type = GetOpType(temp);									// Get type
	if (type == -1) { free(temp); return 0; }
	line = &line[2];

	int comb = 0;
	if (type == 1)
	{
		strcpy(temp, line);
		comb = GetOpComb(temp);									// Get comb
		if (comb == -1)  { free(temp); return 0; }
		line = &line[4];
	}
	free(temp);

	if (ins->numOfOp == 0)										// Process commands based on groups
	{
		if ((comb != 0) || (FirstNonWhitespace(line, 0) != -1))
			return PrintSaveError(ins->name, "too many arguments");
		StoreInstline(ins, NULL, comb, type);
	}
	else
	{
		operand* ops[MAX_ARGUMENTS];
		line = &line[FirstNonWhitespace(line,0)];

		for (int j=0; j<MAX_ARGUMENTS; j++)
			if (!(ops[j] = GetOperand(line, ins, j)))
				return 0;

		StoreInstline(ins, &ops, comb, type);
	}
	return 1;
}

// This function gets a pointer to an operand struct from a given line.
operand* GetOperand(char* line, inst* ins, int srcOrDest)
{
	if ((ins->numOfOp == 1) && (srcOrDest == 0))
		return (operand*)calloc(1, sizeof(operand));		// Just an empty operand

	static int i = 0;
	char* word = GetWord(&line[i], ',');
	if (strcmp(word,"") == 0)
	{
		PrintSaveError(ins->name, "too few arguments");
		return NULL;
	}
	else
	{
		operand* op = calloc(1, sizeof(operand));
		strcpy(op->name, word);
		i += strlen(word) + 1;
		free(word);

		op->addrType = GetAddressingType(op->name);
		if (op->addrType == DIRECT_REG || op->addrType == INDEX_REGISTER)
			op->reg = GetOpRegister(op->name, op->addrType);

		if ((op->addrType == -1) || !IsAllowedAddressing(ins, srcOrDest, op->addrType))
		{
			free(op);
			PrintSaveError(ins->name, "unsupported addressing type");
			return NULL;
		}
		else
		{
			if (srcOrDest == DEST) i = 0;
			return op;
		}
	}
}

// This function gets the type bit of the command
// starting at the address of data
int GetOpType(char* data)
{
	int type;
	char* word = GetWord(data, '\n');
	if ((strcmp(word,"") == 0))
	{
		PrintSaveError("", "no type provided");
		return -1;
	}
	else
	{
		free(word);
		type = atoi(strtok(&data[1], "/"));
		if ((type != 0) && (type != 1))
		{
			PrintSaveError("", "unknown type provided");
			return -1;
		}
		else
			return type;
	}
}

// This function gets the type comb bits of the command
// starting at the address of data
int GetOpComb(char* data)
{
	int comb[2];
	char* word = GetWord(data, '\n');
	if ((strcmp(word,"") == 0))
	{
		PrintSaveError("", "no comb provided");
		return -1;
	}
	else
	{
		comb[0] = atoi(strtok(word,"/"));
		comb[1] = atoi(strtok(NULL,"/"));
		free(word);
		if ((comb[0] != 0 && comb[0] != 1) || (comb[1] != 0 && comb[1] != 1))
		{
			PrintSaveError("", "unknown comb provided");
			return -1;
		}
		else
		{
			if ((comb[0] == 0) && (comb[1] == 0)) return 0;
			else if ((comb[0] == 0) && (comb[1] == 1)) return 1;
			else if ((comb[0] == 1) && (comb[1] == 0)) return 2;
			else return 3;
		}
	}
}

// This function extracts the register code from a given operand.
int GetOpRegister(char* operand, int addrType)
{
	if (addrType == DIRECT_REG)
		return GetRegisterByName(operand)->code;
	else if (addrType == INDEX_REGISTER)
		return GetRegisterByName(GetOpIndex(operand))->code;
	return -1;
}

// This function gets the type of addressing used on this operand.
int GetAddressingType(char* operand)
{
	if ((strcmp(operand, "") == 0))
		return -1;
	else if (GetRegisterByName(operand))
		return DIRECT_REG;
	else if (operand[0] == '#' && IsNumber(&operand[1]))
		return IMMEDIATE;
	else
	{
		char* label = GetWord(operand,'{');
		if (label && IsAllowedLabelName(label))
		{
			char* index = GetOpIndex(operand);
			if (index)
			{
				if (IsNumber(index))
					return INDEX_DIRECT;
				if (GetRegisterByName(index))
					return INDEX_REGISTER;
				if (IsAllowedLabelName(index))
					return INDEX_LABLE;
				return -1;
			}
			else
				return LABEL;
		}
	}
	return -1;
}

// This function extracts the index of a given index typed operand.
char* GetOpIndex(char* data)
{
	char* a = strchr(data,'{') + sizeof(char);
	char* b = strchr(data,'}');

	if ((a && b) && (a < b))
	{
		int size = (b-a) * sizeof(char);
		char* index = (char*)calloc(size, sizeof(char));
		strncpy(index, a, size);
		index[size] = '\0';
		return index;
	}
	return NULL;
}

// This function returns true if the addressing type is allowed
// for this instruction, false otherwise.
int IsAllowedAddressing(inst* ins , int opType , int addrType)
{
	int t = (opType == SOURCE) ? (ins->sourceAllowedAddr[addrType]) : (ins->destAllowedAddr[addrType]);
	return t;
}
