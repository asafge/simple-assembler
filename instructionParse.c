/*
 * The instructionParse files (.h,c) are responsible for the more lower-level
 * handling of the instructions, making sure they don't contain errors etc.
 */
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "config.h"
#include "globalHelper.h"
#include "instructionHandler.h"
#include "instructionParse.h"
#include "mem/symbolMem.h"
#include "mem/instlineMem.h"
#include "mem/datastringMem.h"

extern int lineNumber;
extern inst opInstructionsArray[Max_Instruction + 1];
extern reg registersArray[MAX_REGISTER];

// This function handles the VirtualInstruction .DATA [#],[#],...
// First call assuming no spaces.
// FIXME: Probably buggy.
int HandleDataInstruction(char* data)
{
	char* num = strtok(data,",");
	if (!num)
	{
		printf("Error in line %d | DATA %s ,no data provided.\n", lineNumber, data);
		return 0;
	}
	else
	{
		while (num != NULL)							// Read all numbers
		{
			int i=0;
			for (int i=0; isspace(num[i]);i++) ;			// Ignore white spaces
			if (!IsNumber(&num[i]))
			{
				printf("Error in line %d | DATA %s, is not a numeric.\n", lineNumber, data);
				return 0;
			}
			else
			{
				StoreData(atoi(&num[i]));
				num = strtok (NULL, ",");			// Get the next number from the list
			}
		}
	}
	return 1;
}

// This function handles the VirtualInstruction .STRING "..."
// FIXME: Probably buggy.
int HandleStringInstruction(char* data)
{
	int len = strlen(data);
	if (!len)
	{
		printf("Error in line %d | No data provided.\n", lineNumber);
		return 0;
	}
	else
	{
		int i,j = 0;
		int strLength = 0;

		for (; isspace(data[i]); i++) ;			// Ignore white spaces
		if (data[i] != '"')						// Make sure it's the beginning of a string
		{
			printf("Error in line %d | First char is not \".\n", lineNumber);
			return 0;
		}
		else
		{
			i++;
			for (j=i; (j<len && data[j] != '"'); j++)
					strLength++;

			if (!strLength)
			{
				printf("Error in line %d | STRING %s not a valid string.\n", lineNumber, data);
				return 0;
			}
			else
			{
				/* TODO: Test this */
				char temp[j-i];
				memcpy(temp, &data[i], j-i);
				temp[j] = '\0';
				StoreData((int)temp);
			}
		}
	}
	return 1;
}

// This function handles the instruction .EXTERN ...
int HandleExternInstruction(char* label)
{
	return SetLabelAsSymbol(label, lineNumber, strlen(label) ,(short)CODE, (short)1, (short)0, (short)0);
}

// This function handles the instruction .ENTRY ...
// FIXME: save this as an instruction line
int HandleEntryInstruction(char* label)
{
	//return SetLabelAsSymbol(label, lineNumber, strlen(label) ,(short)CODE, (short)0, (short)1, (short)0);
	return -1;
}

// This function looks up an instruction name in the instruction definition array.
// If NULL is returned there's no instruction with that name found.
inst* GetInstByName(char* name)
{
	int i;
	for (i=0; i<Max_Instruction; i++)
		if (strcmp(name,opInstructionsArray[i].name) == 0)
			return &opInstructionsArray[i];
	return NULL;
}

// This function looks up a register name in the registers array.
// If NULL is returned there's no register with that name found.
reg* GetRegisterByName(char* name)
{
	int i;
	for (i=0; i<MAX_REGISTER; i++)
		if (strcmp(name, registersArray[i].name) == 0)
			return &registersArray[i];
	return NULL;
}

// This function gets the type of addressing used on this operand.
int GetAddressingType(char* operand)
{
	if (GetRegisterByName(operand))
			return DIRECT_REG;
	else if (IsAllowedLabelName(operand))
		return LABEL;
	if (operand[0] == '#' && IsNumber(&operand[1]))
		return IMMEDIATE;
	else
	{
		char* label = GetWord(operand,'{');
		if (label && IsAllowedLabelName(label))
		{
			char* a = strchr(operand,'{');
			char* b = strchr(operand,'}');

			if ((a && b) && (a < b))
			{
				char index[MAX_LINE];
				strncpy(index, a, b-a);
				index[b-a] = '\0';					// TODO: Test

				if (IsNumber(index))
					return INDEX_DIRECT;
				if (GetRegisterByName(index))
					return INDEX_REGISTER;
				if (IsAllowedLabelName(index))
					return INDEX_LABLE;
			}
		}
	}
	return -1;
}

// FIXME: implement
char* GetIndex(char* operand)
{
/*
int WhichRegister (char *_register,int model)
{
	int i;
	char *registryName;

	if(model==REGISTER)
	{
		for (i=0;i<MAX_REGISTER;i++)
			if(strcmp(_register,strRegister[i].name)==0)
			{
				return i;
			}
	}

	else if (model==LABEL_PLUS_REGISTER)
	{
		for(i=0;i<MAX_LINE;i++)
			if(_register[i]=='[')
				break;
		registryName=GetWord(&_register[i+1],']');

		for (i=0;i<MAX_REGISTER;i++)
			if(strcmp(registryName,strRegister[i].name)==0)
			{
				return i;
			}
	}
		return -1;
}
 */
	return NULL;
}

// Check if this instruction addressing is legal
int IsAllowedAddressing(inst* ins , int operandType , int addressingType)
{
	return (operandType == SOURCE) ? (ins->sourceAllowedAddr[addressingType]) : (ins->destAllowedAddr[addressingType]);
}
