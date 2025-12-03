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
		Vector<uint32> indices;
		uint32 last = 0;
		while (pFile->IsEnd() == false)
		{
			Vector<String> data = gce::SplitString(pFile->ReadLine(), ' ');
			
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
				float32 pos2 = 1.0f - gce::ConvertStringToFloat(data[2]);
				m_tempCoordVect.PushBack({ pos1, pos2 });
			}
			if (data[0] == "f")
			{
				ASSERT(data.Size() > 3, "Invalid face", ERROR_);
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
		Mesh mesh = Mesh();
		mesh.indices = indices;
		meshs.PushBack(mesh);
	}

	void Obj::LoadJsonObj(json const& object)
	{
		json mesh = object["mesh"];
		json vVertices = mesh["vertices"];
		json indices = mesh["indices"];
		json uvs = mesh["uvs"];

		int nVertices = (int)vVertices.size() / 3;

		for (int i = 0; i < indices.size(); i++)
		{
			int vIndex = indices[i].get<int>();

			// Position from vertex index
			Vector3f32 pos;
			pos.x = vVertices[vIndex * 3 + 0].get<float>();
			pos.y = vVertices[vIndex * 3 + 1].get<float>();
			pos.z = vVertices[vIndex * 3 + 2].get<float>();

			// UV from loop index
			Vector2f32 texCoord;
			texCoord.x = uvs[i * 2 + 0].get<float>();
			texCoord.y = uvs[i * 2 + 1].get<float>();

			// Normals not exported yet
			Vector3f32 normal = {0,0,0};

			vertices.PushBack(obj::Vertex(pos, normal, texCoord));
		}

		Mesh m;
		
		for (int i = 0; i < indices.size(); i++)
		{
			m.indices.PushBack(i);
		}

		meshs.PushBack(m);
	}


	
}