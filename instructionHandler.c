/*
 * The instructionHandler files (.h,c) are responsible for declaring and defining data types and
 * static data objects for the use of the instructionParse files. Also, they handle instruction check.
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

			{"entry",16,{0,0,0,0,0,0},{0,1,0,0,0,0},1} };											// .entry

// An array of registers. Each register is in he form of {name,code} - see above
reg registersArray[MAX_REGISTER] = {{"r0",0},{"r1",1},{"r2",2},{"r3",3},{"r4",4},{"r5",5},{"r6",6},{"r7",7}};

// This function looks up an instruction name in the instruction definition array.
// If NULL is returned there's no instruction with that name found.
inst* GetInstByName(char* name)
{
	for (int i=0; i<Max_Instruction; i++)
		if (strcmp(name,opInstructionsArray[i].name) == 0)
			return &opInstructionsArray[i];
	return NULL;
}

// This function looks up a register name in the registers array.
// If NULL is returned there's no register with that name found.
reg* GetRegisterByName(char* name)
{
	for (int i=0; i<MAX_REGISTER; i++)
		if (strcmp(name, registersArray[i].name) == 0)
			return &registersArray[i];
	return NULL;
}

// Check if this instruction has a symbol (might not be a valid one).
// If so it will return the length of its name, otherwise 0.
int IsSymbol(char *line)
{
	int i = FirstNonWhitespace(line, 0);
	if(line[i] == '.') return 0;						// Not a label if it starts with "."

	for (i=0; i<MAX_LABEL; i++)
		if(line[i] == ':')
			return i;									// Found a label (found ":")
	return 0 ;											// Did not find a label, this instruction is something else
}

// Check if this is a virtual instruction in the form of .string/.data.
// If so it will save the symbol (if any), and the string/integer given.
int IsStringOrData(char* line, int symbolIndex)
{
	int i = (symbolIndex) ? (symbolIndex + 1) : 0;		// Starting index depends on symbolIndex
	i = FirstNonWhitespace(line, i);					// Ignore whitespaces

	char* word = GetWord(&line[i], ' ');				// Pass address of line[i] to get the word
	if ((strcmp(word, "") == 0)) return 0;
	int isD = (strcasecmp(word, ".data") == 0);			// Is data?
	int isS = (strcasecmp(word, ".string") == 0);		// Is string?
	i += strlen(word);									// skip the data/string strings if found
	free(word);

	if (!isD && !isS) return 0;							// Not a data or a string
	if (symbolIndex > 0)								// Add to symbol list
		if (!SaveLeadingLabel(line, DATA, 0, 0, (short)DC))
			return 0;

	// Add the string or the integers to the memory
	return ((isD) ? HandleDataInstruction(&line[i]) : HandleStringInstruction(&line[i]));
}

// Check if this is an .extern or .entry instruction, save if needed.
int IsEntryOrExtern(char *line, int symbolIndex)
{
	int i = (symbolIndex) ? (symbolIndex + 1) : 0;		// Starting index depends on symbolIndex
	i = FirstNonWhitespace(line,i);						// Ignore white spaces

	char* word = GetWord(&line[i], ' ');				// Pass address of line[i] to get the word
	if ((strcmp(word, "") == 0)) return 0;
	int isEx = (strcasecmp(word, ".extern") == 0);		// Is extern?
	int isEn = (strcasecmp(word, ".entry") == 0);		// Is entry?
	i += strlen(word);									// Skip the extern/entry strings if found
	free(word);

	if (!isEx && !isEn) return 0;						// Not extern or entry

	return ((isEx) ? HandleExternInstruction(&line[i]) : HandleEntryInstruction(&line[i]));
}

// Check if this is an opcode instruction (like "mov")
int IsOpInstruction(char *line , int symbolIndex)
{
	if (symbolIndex > 0)
		if (!SaveLeadingLabel(line, CODE, 0, 0, IC))		// Save symbol
			return 0;

	int i = (symbolIndex) ? (symbolIndex + 1) : 0;			// Starting index depends on symbolIndex
	i = FirstNonWhitespace(line, i);						// Ignore white spaces

	char* cmdName = GetWord(&line[i], '/');					// Try to get command name
	inst* ins = GetInstByName(cmdName);						// Try to get corresponding instruction
	free(cmdName);

	if(!ins)
		return PrintSaveError(cmdName, "an invalid instruction");

	i += strlen(ins->name);									// Increment index to pass cmdName
	return HandleOpInstruction(&line[i], ins);				// Save symbol if exists
}
