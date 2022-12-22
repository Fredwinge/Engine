#include "ModelLoader.h"
#include <stdio.h>
#include <assert.h>
#include "Math/Vectors.h"
#include <Windows.h>

//TODO: Move somewhere else, rework to apply to primitives aswell?
void CalculateTangentBinormal(SVertexData vertex1, SVertexData vertex2, SVertexData vertex3, Vec3& tangent, Vec3& binormal)
{
	// This function assumes this equation is correct
	// | edge1 | = | edgeUV1 | * | tangent  | 
	// | edge2 | = | edgeUV2 |   | binormal |

	const Vec3 edge1 = vertex2.Position - vertex1.Position;
	const Vec3 edge2 = vertex3.Position - vertex1.Position;
	const Vec2 edgeUV1 = vertex2.TexCoord - vertex1.TexCoord;
	const Vec2 edgeUV2 = vertex3.TexCoord - vertex1.TexCoord;

	const float denominator = 1.0f / (edgeUV1.x * edgeUV2.y - edgeUV2.x * edgeUV1.y);

	tangent = ((edge1 * edgeUV2.y) - (edge2 * edgeUV1.y)) * denominator;
	binormal = ((edge2 * edgeUV1.x) - (edge1 * edgeUV2.x)) * denominator;

	tangent.Normalize();
	binormal.Normalize();

	return;
}

void CModelLoader::LoadModel(const char* path, std::vector<SVertexData>* pVertexBuffer, std::vector<uint16>* pIndexBuffer)
{

	const char* fileExt = strrchr(path, '.');

	//TODO: should we take capitalization into consideration? _stricmp?
	if (strcmp(fileExt, ".obj") == 0)
	{
		LoadModelOBJ(path, pVertexBuffer, pIndexBuffer);
	}
	else
	{
		assert(false && "Model filetype not supported!");
	}
}

void CModelLoader::LoadModelOBJ(const char* path, std::vector<SVertexData>* pVertexBuffer, std::vector<tIndex>* pIndexBuffer)
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

	struct Triangle
	{
		tIndex vert_indices[3];
		tIndex uv_indices[3];
		tIndex normal_indices[3];
	};
	std::vector<Triangle> Triangle_Indices;

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

			Triangle tri;
			for (uint32 i = 0; i < 3; ++i)
			{
				tri.vert_indices[i] = vIdx[i] - 1;
				tri.uv_indices[i] = uvIdx[i] - 1;
				tri.normal_indices[i] = nrmIdx[i] - 1;
			}
			Triangle_Indices.push_back(tri);
			
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
	std::vector<tIndex> IndexBuffer;
	std::vector<SVertexData> VertexBuffer;

	int current_idx = -1;
	for (uint16 i = 0; i < Triangle_Indices.size(); ++i)
	{

		Triangle tri = Triangle_Indices[i];
		SVertexData VertexData[3];
		for (uint16 tri_idx = 0; tri_idx < 3; ++tri_idx)
		{
			VertexData[tri_idx].Position = vertices[tri.vert_indices[tri_idx]];;
			VertexData[tri_idx].TexCoord = texcoords[tri.uv_indices[tri_idx]];
			VertexData[tri_idx].Normal = normals[tri.normal_indices[tri_idx]];
		}

		//TODO: Add ability to read from file instead
		//Calculate tangent and binormals
		CalculateTangentBinormal(VertexData[0], VertexData[1], VertexData[2], VertexData[0].Tangent, VertexData[0].Binormal);
		CalculateTangentBinormal(VertexData[1], VertexData[2], VertexData[0], VertexData[1].Tangent, VertexData[1].Binormal);
		CalculateTangentBinormal(VertexData[2], VertexData[0], VertexData[1], VertexData[2].Tangent, VertexData[2].Binormal);

		for (uint16 tri_idx = 0; tri_idx < 3; ++tri_idx)
		{
			++current_idx;
			bool isDupe = false;
			//Check if vertexData is a dupe
			for (uint16 j = 0; j < VertexBuffer.size(); ++j)
			{
				if (VertexData[tri_idx] == VertexBuffer[j])
				{
					IndexBuffer.push_back(j);
					++numDupes;
					isDupe = true;
					break;
				}
			}

			if (isDupe == true)
				continue;

			IndexBuffer.push_back(current_idx - numDupes);
			VertexBuffer.push_back(VertexData[tri_idx]);
		}
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