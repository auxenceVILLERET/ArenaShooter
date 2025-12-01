#ifndef CORE_MTLPARSER_H_INCLUDED
#define CORE_MTLPARSER_H_INCLUDED
#include "define.h"
#include "Maths/Vector3.h"
#include "File.h"

namespace gce {

	struct MtlMaterial
	{
		float32 specularExponent = 0; // Ns
		Vector3f32 ambientColor = { 0,0,0 }; // Ka
		Vector3f32 diffuseColor = { 0,0,0 }; // Kd
		Vector3f32 specularColor = { 0,0,0 }; // Ks
		Vector3f32 emissiveColor = { 0,0,0 }; // Ke
		float32 indexOfRefraction = 0; // Ni
		float32 dissolve = 0; // d
		uint8 illuminationModel = 0; // illum
	};

	class MtlParser
	{
	public:
		MtlParser(const String& filePath);
		uint16 materialCount = 0;
		UnorderedMap<String, MtlMaterial> Parse();
	private:
		File m_file;
	};

}
#endif
