#include "ModelLoader.h"
#include <stdio.h>
#include <assert.h>
#include "Math/Vectors.h"
#include <Windows.h>

void CModelLoader::LoadModel(const char* path, std::vector<VertexData>* pVertexBuffer, std::vector<uint16>* pIndexBuffer)
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

	std::vector<uint16> vert_indices;
	std::vector<uint16> uv_indices;
	std::vector<uint16> normal_indices;

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
			
			uint32 vIdx[3];

			uint32 uvIdx[3];
			uint32 nrmIdx[3];

			//TODO: Figure out format, since it seems to differ all over the place
			int matches = fscanf_s(pFile, "%d/%d/%d %d/%d/%d %d/%d/%d\n", 
									&vIdx[0], &uvIdx[0], &nrmIdx[0],
									&vIdx[1], &uvIdx[1], &nrmIdx[1],
									&vIdx[2], &uvIdx[2], &nrmIdx[2]);

			if(matches != 9)
				assert(false && "This ain't gonna work");

			for (uint16 i = 0; i < 3; ++i)
			{
				//Subtract by 1 since OBJ indices start at 1 rather than 0
				vert_indices.push_back(vIdx[i] - 1);
				uv_indices.push_back(uvIdx[i] - 1);
				normal_indices.push_back(nrmIdx[i] - 1);
			}
			
			/*
			//Maybe useful for rework
			while (true)
			{
				UINT vertex_indices;

				UINT uvIdx;
				UINT nrmIdx;

				//TODO: Figure out format, since it seems to differ all over the place
				int matches = fscanf_s(pFile, "%d/%d/%d",
					&vertex_indices, &uvIdx, &nrmIdx);

				if (matches != 3)
				{
					//assert(false && "This ain't gonna work");

					fscanf_s(pFile, "\n");
					break;
				}
				else
				{
					indices.push_back(vertex_indices - 1);
					uv_indices.push_back(uvIdx - 1);
					normal_indices.push_back(nrmIdx - 1);
				}
			}
			*/
			

		}
	}

	uint16 numDupes = 0;
	std::vector<uint16> IndexBuffer;
	std::vector<VertexData> VertexBuffer;

	for (uint16 i = 0; i < vert_indices.size(); ++i)
	{
		VertexData VertexData;
		VertexData.Position = vertices[vert_indices[i]];
		VertexData.TexCoord = texcoords[uv_indices[i]];
		VertexData.Normal = normals[normal_indices[i]];

		bool isDupe = false;
		//Check if vertexData is a dupe
		for (uint16 j = 0; j < VertexBuffer.size(); ++j)
		{
			if (VertexData.Position == VertexBuffer[j].Position &&
				VertexData.TexCoord == VertexBuffer[j].TexCoord &&
				VertexData.Normal == VertexBuffer[j].Normal)
			{
				IndexBuffer.push_back(j);
				++numDupes;
				isDupe = true;
				break;
			}
		}

		if (isDupe == true)
			continue;

		IndexBuffer.push_back(i - numDupes);
		VertexBuffer.push_back(VertexData);
	}

	fclose(pFile);

	*pVertexBuffer = VertexBuffer;
	*pIndexBuffer = IndexBuffer;

	//LOG FILE DATA
	/*
	if(fopen_s(&pFile, path, "r") != 0)
		assert(false && "failed to open file");

	FILE* pLogFile;
	fopen_s(&pLogFile, "../ModelErrorLog.txt", "w+");

	char fileBuffer[1024];

	//fgets returns null if it reaches the end of a file,
	//fscanf should be used to get data
	while(fgets(fileBuffer, 1024, pFile) != NULL)
		fputs(fileBuffer, pLogFile);

	fclose(pLogFile);
	delete pLogFile;
	//delete fileBuffer;
	fclose(pFile);
	*/

}