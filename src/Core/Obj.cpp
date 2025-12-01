#include "Obj.h"
#include "File.h"
#include "Maths/Triangulation/Triangulation.h"
#include <Containers/Vector.hpp>
#include "Utils.h"

namespace gce {
	
	Obj::Obj(String const& path) : pFile(nullptr), m_tempNormVect(), m_tempPosVect()
	{
		File* file = new File(path,File::Type::READ_ONLY);
		pFile = file;
		m_path = path;
	}

	Obj::Obj(String const&& path) : pFile(nullptr), m_tempNormVect(), m_tempPosVect()
	{
		File* file = new File(path, File::Type::READ_ONLY);
		pFile = file;
		m_path = path;
	}

	Obj::~Obj()
	{
		delete pFile;
	}

	void Obj::LoadObj()
	{
		UnorderedMap<String, MtlMaterial> materials;
		Vector<uint32> indices;	
		uint32 last = 0;
		while (pFile->IsEnd() == false)
		{
			Vector<String> data = gce::SplitString(pFile->ReadLine(), ' ');
			
			if (data[0] == "mtllib" && data.Size() >= 2)
			{
				String path = data[1];
				if (data.Size() > 2)
				{
					for (int i = 2; i < data.Size(); i++)
					{
						path += " ";
						if (data[i] == "")
							continue;
						path += data[i];
					}
				}
				
				materials = LoadMtl(path);
			}

			if (data[0] == "usemtl")
			{
				if (mtlExists && data.Size() >= 2)
				{
					String mtlName = data[1];
					if (data.Size() > 2)
					{
						for (int i = 2; i < data.Size(); i++)
						{
							mtlName += " ";
							mtlName += data[i];
						}
					}
					MTLData tmpmtldata;
					tmpmtldata.mtlId = mtlName;
					tmpmtldata.mtlIndexBegin = indices.Size();
					if (mtlData.Size() > 1)
					{
						mtlData[mtlData.Size() - 1].mtlIndexEnd = indices.Size();
					}
					mtlData.PushBack(tmpmtldata);

				}
			}
			data = gce::EmptyRemover(data);
			if (data.Size() == 0) { continue; }

			if (data[0] == "v")
			{
				ASSERT(data.Size() == 4, "Invalid position", ERROR_);

				float32 pos1 = gce::ConvertStringToFloat(data[1]);
				float32 pos2 = gce::ConvertStringToFloat(data[2]);
				float32 pos3 = gce::ConvertStringToFloat(data[3]);
				m_tempPosVect.PushBack({ pos1, pos2, pos3 });
			}
			if (data[0] == "vn")
			{
				ASSERT(data.Size() == 4, "Invalid normal", ERROR_);

				float32 pos1 = gce::ConvertStringToFloat(data[1]);
				float32 pos2 = gce::ConvertStringToFloat(data[2]);
				float32 pos3 = gce::ConvertStringToFloat(data[3]);
				m_tempNormVect.PushBack({ pos1, pos2, pos3 });
			}
			if (data[0] == "vt")
			{
				ASSERT(data.Size() == 3, "Invalid textureCoordinate", ERROR_);

				float32 pos1 = gce::ConvertStringToFloat(data[1]);
				float32 pos2 = gce::ConvertStringToFloat(data[2]);
				m_tempCoordVect.PushBack({ pos1, pos2 });
			}
			if (data[0] == "f")
			{
				ASSERT(data.Size() >= 4, "Invalid face", ERROR_);
				for (int64 i = 1; i < data.Size(); i++)
				{
					Vector<String> face = gce::SplitString((data[i]), '/');

					obj::Vertex tempVertex;

					if (face[0] != "")
						tempVertex.position = m_tempPosVect[gce::ConvertStringToInt(face[0]) - 1];
					if (face[1] != "")
						tempVertex.textureCoordinate = m_tempCoordVect[gce::ConvertStringToInt(face[1]) - 1];
					if (face[2] != "")
						tempVertex.normal = m_tempNormVect[gce::ConvertStringToInt(face[2]) - 1];

					vertices.PushBack(tempVertex);
				}

				if (data.Size() == 4)
				{
					indices.PushBack(last + 0);
					indices.PushBack(last + 1);
					indices.PushBack(last + 2);
					last += 3;
				}
				else if (data.Size() == 5)
				{
					indices.PushBack(last + 0);
					indices.PushBack(last + 1);
					indices.PushBack(last + 3);

					indices.PushBack(last + 1);
					indices.PushBack(last + 2);
					indices.PushBack(last + 3);
					last += 4;
				}
				else
				{
					Vector<gce::Vector3f32> posVertices;

					for (uint8 i = 0; i < data.Size() - 1; ++i)
					{
						posVertices.PushBack(vertices[last + i].position);
					}

					Vector<uint32> tempIndices = gce::Triangulate(gce::ProjectToPlane(posVertices));

					for (int8 i = tempIndices.Size() - 1; i >= 0; --i)
					{
						indices.PushBack(last + tempIndices[i]);
					}
					last += data.Size() - 1;
				}
			}
		}
		meshs.PushBack(Mesh());
		if (mtlExists)
		{
			int count = 0;
			meshs[0].material = materials[mtlData[count].mtlId];

			for (int i = 0; i < indices.Size(); i++)
			{
				if (mtlData[count].mtlIndexBegin > i)
				{
					continue;
				}
				else if (mtlData[count].mtlIndexEnd <= i)
				{
					count++;
					meshs.PushBack(Mesh());
					meshs[count].material = materials[mtlData[count].mtlId];
				}
				meshs[count].indices.PushBack(indices[i]);
			}
		}
		else
		{
			meshs[0].material = MtlMaterial();
			meshs[0].indices = indices;
		}
	}
	
	UnorderedMap<String, MtlMaterial> Obj::LoadMtl(String& path)
	{
		//load mtl if exists in the same directory as the obj
		String mtlPath = m_path.substr(0, m_path.find_last_of('/')) + "/" + path;

		File file(mtlPath, gce::File::Type::READ_ONLY);
		if (file.IsOpened())
		{
			MtlParser mtlParser(mtlPath);
			mtlExists = true;
			return mtlParser.Parse();
		}
		return UnorderedMap<String, MtlMaterial>();
	}
	
	void Obj::FreeVectors()
	{
		// Frees all vectors inside the class except m_meshVector
		m_tempIndiceVect.Clear();
		m_tempNormVect.Clear();
		m_tempPosVect.Clear();
		m_tempCoordVect.Clear();
	}
}