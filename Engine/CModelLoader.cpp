#include "CModelLoader.h"
#include <stdio.h>
#include <assert.h>
#include "Maths/Vectors.h"
#include <Windows.h>

CModel* CModelLoader::LoadModel(CGraphics& rGfx, const char* path)
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

	std::vector<unsigned short> indices;
	std::vector<unsigned short> uv_indices;
	std::vector<unsigned short> normal_indices;

	std::vector<CModel::VertexData> VertexBuffer;

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
			
			UINT vertex_indices[4];

			UINT uvIdx[4];
			UINT nrmIdx[4];

			//TODO: Figure out format, since it seems to differ all over the place
			int matches = fscanf_s(pFile, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", 
									&vertex_indices[0], &uvIdx[0], &nrmIdx[0],
									&vertex_indices[1], &uvIdx[1], &nrmIdx[1],
									&vertex_indices[2], &uvIdx[2], &nrmIdx[2],
									&vertex_indices[3], &uvIdx[3], &nrmIdx[3]);

			if(matches != 12)
				assert(false && "This ain't gonna work");

			for (int i = 0; i < 4; ++i)
			{
				indices.push_back(vertex_indices[i] - 1);
				uv_indices.push_back(uvIdx[i] - 1);
				normal_indices.push_back(nrmIdx[i] - 1);
			}
			
			/*
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

					//fscanf_s(pFile, "\n");
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

	for (int i = 0; i < indices.size(); ++i)
	{
		CModel::VertexData VertexData;
		VertexData.Position = vertices[indices[i]];
		VertexData.TexCoord = texcoords[uv_indices[i]];
		VertexData.Normal = normals[normal_indices[i]];

		VertexBuffer.push_back(VertexData);
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

	//Vertices are already added in correct order
	std::vector<unsigned short> tempIndices;
	for (int i = 2; i < VertexBuffer.size(); i += 3)
	{
		tempIndices.push_back(i - 2);
		tempIndices.push_back(i - 1);
		tempIndices.push_back(i);

		tempIndices.push_back(i - 1);
		tempIndices.push_back(i);
		tempIndices.push_back(i + 1);
	}

	delete pFile;

	return new CModel(rGfx, &VertexBuffer, &tempIndices);

}