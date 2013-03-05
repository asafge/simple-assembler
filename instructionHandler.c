/*
 * The instructionHandler files (.h,c) are responsible for declaring and defining data types and
 * static data objects for the use of the instructionCheck and instructionParse files.
 * Also, they handle each instruction check.
 */

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include "config.h"
#include "globalHelper.h"
#include "instructionHandler.h"
#include "instructionParse.h"
#include "mem/symbolMem.h"
#include "mem/instlineMem.h"

extern int pstatus;
extern int lineNumber;
extern int DC;
extern int IC;

/* --- Declaration + Definition (Static data objects) --- */

// An array of operation instructions.
inst opInstructionsArray[Max_Instruction + 1] =
		  { {"mov",0,{1,1,1,1,1,1},{0,1,1,1,1,1},2}, {"cmp",1,{1,1,1,1,1,1},{1,1,1,1,1,1},2},		// Group 1
			{"add",2,{1,1,1,1,1,1},{0,1,1,1,1,1},2}, {"sub",3,{1,1,1,1,1,1},{0,1,1,1,1,1},2},
			{"lea",6,{0,1,1,1,1,0},{0,1,1,1,1,1},2},

			{"not",4,{0,0,0,0,0,0},{0,1,1,1,1,1},1}, {"clr",5,{0,0,0,0,0,0},{0,1,1,1,1,1},1},		// Group 2
			{"inc",7,{0,0,0,0,0,0},{0,1,1,1,1,1},1}, {"dec",8,{0,0,0,0,0,0},{0,1,1,1,1,1},1},
			{"jmp",9,{0,0,0,0,0,0},{0,1,0,1,0,0},1}, {"bne",10,{0,0,0,0,0,0},{0,1,0,1,0,0},1},
			{"red",11,{0,0,0,0,0,0},{0,1,1,1,1,1},1}, {"prn",12,{0,0,0,0,0,0},{0,1,1,1,1,1},1},
			{"jsr",13,{0,0,0,0,0,0},{0,1,0,0,0,0},1},

			{"rts",14,{0,0,0,0,0,0},{0,0,0,0,0,0},0}, {"stop",15,{0,0,0,0,0,0},{0,0,0,0,0,0},0},	// Group3

			{".entry",16,{0,0,0,0,0,0},{0,1,1,1,1,1},0} };											// .entry

// An array of registers. Each register is in he form of {name,code} - see above
reg registersArray[MAX_REGISTER] = {{"r0",0},{"r1",1},{"r2",2},{"r3",3},{"r4",4},{"r5",5},{"r6",6},{"r7",7}};


// Check if this instruction has a symbol (might not be a valid one).
// If so it will return the length of its name, otherwise 0.
int IsSymbol(char *line)
{
	int i=0;
	while (isspace(line[i])) i++;							// Ignore white spaces

	if(line[i] == '.')										// Not a label if it starts with "."
		return 0;
	else
		for (i=0; i<MAX_LABEL; i++)
			if(line[i] == ':')
				return i;									// Found a label (found ":")
	return 0 ;												// Did not find a label, this instruction is something else
}

// Check if this is a virtual instruction in the form of .string/.data
int IsStringOrData(char* line, int symbolIndex)
{
	int i = (symbolIndex) ? (symbolIndex + 1) : 0;			// Starting index depends on symbolIndex
	while (isspace(line[i])) i++;							// Ignore white spaces

	if (line[i] != '.')										// Data/String instructions starts with "."
		return 0;
	else
	{
		int isD, isS = 0;									// Is STRING or DATA
		char* word = GetWord(&line[++i],' ');				// Pass address of line[i] to get the word
		if ((isD = strcasecmp(word, "DATA") != 0) && (isS = strcasecmp(word, "STRING") != 0))
			return 0;
		else
		{
			// Add to symbol list and set phase status. TODO : Refactor into handleData.. / handleString..
			if (symbolIndex > 0)
				pstatus = SetLabelAsSymbol(line, lineNumber, (short)symbolIndex, DATA, 0, 0, (short)DC);

			// If added successfully, handle the instruction
			if (pstatus)
			{
				i += strlen(word);							// skip the DATA/STRING strings
				pstatus = ((isD) ? HandleDataInstruction(&line[i]) : HandleStringInstruction(&line[i]));
			}
		}
	}
	return pstatus;
}

// Check if this is an .extern or .entry instruction.
int IsEntryOrExtern(char *line, int symbolIndex)
{
	int i = (symbolIndex) ? (symbolIndex + 1) : 0;			// Starting index depends on symbolIndex
	while (isspace(line[i])) i++;							// Ignore white spaces

	if (line[i] != '.')										// Entry/Extern instructions starts with "."
		return 0;
	else
	{
		int isEx, isEn = 0;									// Is EXTERN or ENTRY
		char* word = GetWord(&line[++i], ' ');				// Pass address of line[i] to get the word
		if ((isEx = strcasecmp(word, "EXTERN") == 0) || (isEn = strcasecmp(word, "ENTRY") == 0))
		{
			i += strlen(word);								// Skip the EXTERN/ENTRY strings
			pstatus = ((isEx) ? HandleExternInstruction(&line[i]) : HandleEntryInstruction(&line[i]));
		}
		free(word);
	}
	return pstatus;
}

// Check if this is an opcode instruction (like "mov")
// TODO: This is the only function that save the symbol before checking validity of command
int IsOpInstruction(char *line , int symbolIndex)
{
	if(symbolIndex)											// Try to save symbol if needed
	{
		pstatus = SetLabelAsSymbol(line, lineNumber, symbolIndex, CODE, 0, 0, IC);
		if (!pstatus)
			return 0;
	}

	int i = (symbolIndex) ? (symbolIndex + 1) : 0;			// Starting index depends on symbolIndex
	while (isspace(line[i])) i++;							// Ignore white spaces

	char* cmdName = GetWord(&line[i], ' ');					// Try to get command name
	inst* ins = GetInstByName(cmdName);						// Try to get corresponding instruction
	if(strcasecmp(ins->name, "null") == 0)					// Make sure we got an actual command
		return PrintErrIsRecoverable(line, lineNumber, cmdName, "an invalid command", 0);
	else
	{
		char* operands[MAX_ARGUMENTS];						// Save operands found in an array
		int opAddrTypes[MAX_ARGUMENTS];						// Save addressing type for each operand
		int registers[MAX_ARGUMENTS];						// Save registers found

		int len = strlen(line);
		i += strlen(cmdName);								// Increment index to pass cmdName
		if ((ins->numOfOp) == 0)							// Process commands differently based on their group
		{
			while (i<len)
				if (!isspace(line[i]))
					return PrintErrIsRecoverable(line, lineNumber, cmdName, "too many arguments", 0);
		}
		else
		{
			int j = 0;										// Counter for scanning operands
			int opSrcOrDest = SOURCE;						// SOURCE or DEST

			while (j < (ins->numOfOp))
			{
				operands[j] = GetWord(&line[i], ',');
				if (!operands[j])
					return PrintErrIsRecoverable(line, lineNumber, cmdName, "too few arguments", 0);
				else
				{
					opAddrTypes[j] = GetAddressingType(operands[j]);
					if (opAddrTypes[j] == -1)
						return PrintErrIsRecoverable(line, lineNumber, cmdName, "unsupported addressing type", 0);

					if (ins->numOfOp == 1)								// We might just need one argument, treat it as DEST
						opSrcOrDest++;
					if (!IsAllowedAddressing(ins, opSrcOrDest, opAddrTypes[j]))
						return PrintErrIsRecoverable(line, lineNumber, cmdName, "unsupported addressing for this command", 0);

					if (opAddrTypes[j] == INDEX_LABLE || opAddrTypes[j] == INDEX_REGISTER)
					{
						// FIXME: Implement
						//char* label = GetWord(operand, '{');
						//char* index = GetIndex(operand);				// Could be num,register or label
					}
				}
			}
		}
		if (!StoreInstline(ins,&*operands,opAddrTypes,registers))
			return PrintErrIsRecoverable(line, lineNumber, cmdName, "could not store instruction line", 0);
	}
	return 1;
}
