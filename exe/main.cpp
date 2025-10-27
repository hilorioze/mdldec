/*
==============================================================

	Half-Life MDL Decompiler
	2002, Kratisto. Based on code from HL SDK

==============================================================
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include "../class/mdldec.h"

int main(int argc, char *argv[])
{
	CMDLDecompiler *mdl = new CMDLDecompiler;

	printf("\nHalf-Life MDL Decompiler v1.01.\n");
	printf("2002, Kratisto. Based on code from Valve's HL SDK.\n");
	printf("--------------------------------------------------\n");

	if (argc == 1)
	{
		printf("Usage: mdldec <path%cmdlfile.mdl> [<destpath>]\n", PATH_SEPARATOR);
		printf("--------------------------------------------------\n");
	
		return 1;
	}

	if (argc == 3)
		strcpy(mdl->DestPath, argv[2]);

	if (mdl->LoadModel(argv[1]))
	{
		//mdl.DumpInfo();
		mdl->FixRepeatedSequenceNames();
		mdl->QC_GenerateScript();
		mdl->SMD_GenerateReferences();
		mdl->SMD_GenerateSequences();
		mdl->BMP_GenerateTextures();
		mdl->LogMessage(MDLDEC_MSG_INFO,"Done.\r\n");
	}

	printf("--------------------------------------------------\n");

	delete mdl;
	return 0;
}
