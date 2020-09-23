#include "CModelLoader.h"
#include <stdio.h>
#include <assert.h>

void CModelLoader::LoadModel(const char* path)
{

	//"r" is for read
	FILE* pFile;// = fopen(path, "r");

	//fopen_s is a more secure version of fopen, also returns error strings which may
	//be useful for error handling.
	//TODO: Look into performance implications of fopen_s // maybe use sstream instead?
	if(fopen_s(&pFile, path, "r") != 0)
		assert(false && "failed to open file");

	FILE* pLogFile;
	fopen_s(&pLogFile, "ModelErrorLog.txt", "w+");

	char fileBuffer[1024];

	//fgets returns null if it reaches the end of a file,
	//fscanf should be used to get data
	while(fgets(fileBuffer, 1024, pFile) != NULL)
		fputs(fileBuffer, pLogFile);

	fclose(pLogFile);
	delete pLogFile;
	//delete fileBuffer;

	delete pFile;

}