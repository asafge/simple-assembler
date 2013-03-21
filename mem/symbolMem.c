/*
 * The symbol files (.h,c) are responsible for declaring and defining
 * symbols collected during phase 1.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../globalHelper.h"
#include "../config.h"
#include "../instructionParse.h"
#include "symbolMem.h"

SymbolNode* symbList = NULL;
SymbolNode* last = NULL;

// This function saves a leading label in the symbols list.
// Leading means it's the label of the line, followed by ':',
// or its the label of an Entry instruction.
int SaveLeadingLabel(char *line, short type, short isExtern, short isEntery, short address)
{
	char* label = GetWord(line,':');
	int flag = 1;

	if (IsAllowedLabelName(label))
	{
		if(!CreateAddSymbol(label, type, isExtern, isEntery, address))
			flag = PrintSaveError(label, "symbol was previously defined");
	}
	else
		flag = PrintSaveError(label, "symbol has an invalid name");

	if (!flag) free(label);					// Only free if not needed
	return flag;
}

// This function creates a symbol object and adds it to
// the symbols list. This is a helper function for SetLableAsSymbol.
int CreateAddSymbol(char* name, short type, short isExtern, short isEntry, short address)
{
    if (GetSymbol(name, type) != NULL)						// Make sure this symbol doesn't exist
        return 0;
    else
    {
        // Create the symbol
        Symbol* symb = (Symbol*)calloc(1, sizeof(Symbol));
        symb->name = name;
		symb->type = type;
		symb->isExtern = isExtern;
		symb->isEntry = isEntry;
		symb->address = address;

        // Create the node
        SymbolNode* newNode = (SymbolNode*)calloc(1, sizeof(SymbolNode));
        newNode->symbol = symb;

        // Add to list
        if (symbList == NULL)
            symbList = newNode;
        else
            last->nextNode = newNode;
        last = newNode;
    
        return 1;
    }
}

// This function checks that the name of a label is allowed.
int IsAllowedLabelName(char* label)
{
	return (isalpha(label[0]) && !GetInstByName(label) && !GetRegisterByName(label) && (strlen(label) <= 30));
}

// This function returns the Symbol* from the list of all symbols.
// Makes sure it matches name + type (code/data), unless type is -1.
// Returns NULL if not found in the list.
Symbol* GetSymbol(char* name, short type)
{
    SymbolNode* currentNode = symbList;
    if (name == NULL)
    	return NULL;

    // Until we reach the end of the list
    while (currentNode != NULL)
        if ((strcmp(currentNode->symbol->name, name) == 0) && ((type == -1) || (currentNode->symbol->type == type)))
        	return currentNode->symbol;					// Found, return it
		else
			currentNode = currentNode->nextNode;		// Next on the list

    return NULL;
}

// This function frees all space allocated for symbols up until now.
// Will destroy the list itself and point it to NULL.
void RestartSymbols()
{
    SymbolNode* currentNode = symbList;
    while (currentNode != NULL)
    {
        SymbolNode* temp;
        free(currentNode->symbol->name);
        free(currentNode->symbol);

        temp = currentNode->nextNode;
        free(currentNode);
        currentNode = temp;
    }
    symbList = NULL;
    last = NULL;
}
