#include "CModelLoader.h"
#include <stdio.h>
#include <assert.h>
#include "Vectors.h"
#include <Windows.h>

void CModelLoader::LoadModel(const char* path)
{

	//"r" is for read
	FILE* pFile;// = fopen(path, "r");
	//fopen_s is a more secure version of fopen, also returns error strings which may
	//be useful for error handling.
	//TODO: Look into performance implications of fopen_s // maybe use sstream instead?
	if(fopen_s(&pFile, path, "r") != 0)
		assert(false && "failed to open file");

	std::vector<Vector3> vertices;
	std::vector<Vector2> texcoords;
	std::vector<Vector3> normals;

	std::vector<UINT> indices;

	//Read file until end is reached
	while (true)
	{
		char lineHeader[128];

		//read first word of line
		int result = fscanf_s(pFile, "%s", lineHeader, _countof(lineHeader));

		//If End Of File is reached, break loop
		if(result == EOF)
			break;

		//vertices
		if (strcmp("v", lineHeader) == 0)
		{
			Vector3 pos;

			fscanf_s(pFile, "%f %f %f\n", &pos.x, &pos.y, &pos.z);

			vertices.push_back(pos);
		}
		//uv
		else if (strcmp("vt", lineHeader) == 0)
		{
			Vector2 uv;

			fscanf_s(pFile, "%f %f\n", &uv.x, &uv.y);

			texcoords.push_back(uv);
		}
		else if (strcmp("vn", lineHeader) == 0)
		{
			Vector3 normal;

			fscanf_s(pFile, "%f %f %f\n", &normal.x, &normal.y, &normal.z);

			normals.push_back(normal);
		}
		else if (strcmp("f", lineHeader) == 0)
		{
			UINT vertex_indices[3];

			UINT uvIdx[3];
			UINT nrmIdx[3];

			int matches = fscanf_s(pFile, "%d/%d/%d %d/%d/%d %d/%d/%d\n", 
									&vertex_indices[0], &uvIdx[0], &nrmIdx[0],
									&vertex_indices[1], &uvIdx[1], &nrmIdx[1],
									&vertex_indices[2], &uvIdx[2], &nrmIdx[2]);

			if(matches != 9)
				assert(false && "This ain't gonna work");

			for(int i = 0; i < 3; ++i)
				indices.push_back(vertex_indices[i]);
		}


	}
	//LOG FILE
	/*
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
	*/

	delete pFile;

}