#include "Image.h"
#include <cstring>
#include <sys/stat.h>

namespace gce
{

#pragma region Tools
/////////////////////////////////////
/// @brief Check if the filename has the expected extension.
/// @param pFilepath The name of the file.
/// @param pExtension The extension to test.
/// @return True if match, false otherwise.
/////////////////////////////////////
bool Image::HasExtension(const char* pFilepath, const char* pExtension)
{
	const char* dot = strrchr(pFilepath, '.');
	return dot && strcmp(dot, pExtension) == 0;
}

/////////////////////////////////////
/// @brief Check if the file exists on disk.
/// @param pFilepath Path to the file.
/// @return True if the file exists, false otherwise.
/////////////////////////////////////
bool Image::FileExists(const char* pFilepath)
{
	struct stat buffer;
	return (stat(pFilepath, &buffer) == 0);
}
#pragma endregion

}