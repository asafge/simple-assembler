/*
 * The entFile files (.h,c) are responsible for creating the .ent file.
 */
#ifndef ENTFILE_H
	#define ENTFILE_H

	#include "../config.h"
	#include "../mem/symbolMem.h"

	int WriteEntryFile (char fileName[MAX_FILE_NAME]);
	Symbol* GetNextEntrySymbol (int lastAddress);
	void AddEntrySymbol (Symbol* sym);

#endif
