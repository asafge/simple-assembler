/*
 * The symbolHandler files (.h,c) are responsible for declaring and defining
 * symbols collected during phase 1.
 */
#ifndef SYMBOLMEM_H
	#define SYMBOLMEM_H

	#include <ctype.h>

	enum symbolType { DATA, CODE };			// Symbol type enum
	typedef struct {						// Symbol structure
		char* name;
		short type;							// DATA or CODE
		short isExtern;
		short isEntry;
		short address;
	} Symbol;
	typedef struct SymbolNode {				// Symbol node structure
		Symbol* symbol;
		struct SymbolNode *nextNode;
	} SymbolNode;

	void RestartSymbols();
	int IsSymbolInstruction(char *line);
	int SaveLeadingLabel(char *line, short type, short isExtern, short isEntery, short address);
	int CreateAddSymbol(char* name, short type, short isExtern, short isEntry, short address);
	int IsAllowedLabelName(char *label);
	Symbol* GetSymbol(char* name, short type);

#endif

