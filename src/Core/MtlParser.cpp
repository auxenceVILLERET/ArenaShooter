#include "MtlParser.h"
#include <Containers/Vector.hpp>
#include "Utils.h"


namespace gce
{
	////////////////////////////////////////////////////////////////////////////////////////
	/// @brief This Class is made to parse and get the information from an mtl file only 
	////////////////////////////////////////////////////////////////////////////////////////
	MtlParser::MtlParser(const String& filePath) : m_file(filePath, gce::File::Type::READ_ONLY)
	{}
	//////////////////////////////////////////////////////////////////
	/// @brief This Parse the mtl to get the info into m_materials
	//////////////////////////////////////////////////////////////////
	UnorderedMap<String, MtlMaterial> gce::MtlParser::Parse()
	{
		UnorderedMap<String, MtlMaterial> materials;

		MtlMaterial mat;
		String tName;
		MtlMaterial empty; // used to reset the mat
		Vector<String> data;
		while (m_file.GetCursor() < m_file.GetSize())
		{
			data = gce::SplitString(m_file.ReadLine(), ' ');


			if (data[0] == "newmtl") // check if there is a new material
			{
				if (materialCount > 0)
				{
					materials[tName] = mat;
					mat = empty;
					tName.clear();
				}
				materialCount++;
				String name = data[1];
				for (int i = 2; i < data.Size(); i++)
				{
					name += " ";
					if (data[i] == "")	continue;
					name += data[i];
				}
				tName = name;
			}
			data = gce::EmptyRemover(data); // remove empty strings from data
			if (data.Size() == 0) { continue; }

			if (data[0] == "Ns")
			{
				if (data.Size() != 2)
					mat.specularExponent = 0;
				else
					mat.specularExponent = gce::ConvertStringToFloat(data[1]);
			}
			if (data[0] == "Ka")
			{
				if (data.Size() != 4) { mat.ambientColor = { 0,0,0 }; }
				else mat.ambientColor = {
					gce::ConvertStringToFloat(data[1]),
					gce::ConvertStringToFloat(data[2]),
					gce::ConvertStringToFloat(data[3])
				};
			}
			if (data[0] == "Kd")
			{
				if (data.Size() != 4) { mat.diffuseColor = { 0,0,0 }; }
				else mat.diffuseColor = {
					gce::ConvertStringToFloat(data[1]),
					gce::ConvertStringToFloat(data[2]),
					gce::ConvertStringToFloat(data[3])
				};
			}
			if (data[0] == "Ks")
			{
				if (data.Size() != 4) { mat.specularColor = {0,0,0}; }
				else mat.specularColor = {
					gce::ConvertStringToFloat(data[1]),
					gce::ConvertStringToFloat(data[2]),
					gce::ConvertStringToFloat(data[3])
				};
			}
			if (data[0] == "Ke")
			{
				if (data.Size() != 4) { mat.emissiveColor = { 0,0,0 }; }
				else mat.emissiveColor = {
					gce::ConvertStringToFloat(data[1]),
					gce::ConvertStringToFloat(data[2]),
					gce::ConvertStringToFloat(data[3])
				};
			}
			if (data[0] == "Ni")
			{
				if (data.Size() != 2) { mat.indexOfRefraction = 0; }
				else  mat.indexOfRefraction = gce::ConvertStringToFloat(data[1]);
			}
			if (data[0] == "d")
			{
				if (data.Size() != 2) { mat.dissolve = 0; }
				else mat.dissolve = gce::ConvertStringToFloat(data[1]);
			}
			if (data[0] == "illum")
			{
				if (data.Size() != 2) { mat.illuminationModel = 0; }
				else mat.illuminationModel = gce::ConvertStringToFloat(data[1]);
			}
		}
		materials[tName] = mat;
		return materials;
	}
}