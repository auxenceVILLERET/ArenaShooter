#include "ImageSTB.h"
#include "Lib/stb/stb_image.h"
#include "Lib/stb/stb_image_write.h"

#include <windows.h>
#include <stdio.h>
#include <cstring> //strcmp

//$(SolutionDir)..\src;$(SolutionDir)..\src\Tool;$(SolutionDir)..\src\Core;% (AdditionalIncludeDirectories);$(SolutionDir)..\src\Tool\Lib

namespace gce
{
#pragma region Constructors
	/////////////////////////////////////
	/// @brief Destructor.
	/////////////////////////////////////
	ImageSTB::~ImageSTB()
	{
		if (m_pPixels)
		{
			stbi_image_free(m_pPixels);
			m_pPixels = nullptr;
		}
	}
#pragma endregion
		
	/////////////////////////////////////
	/// @brief Load an image file using stb_image.
	/// @param pFilepath Path to the file (must end in .jpg or .png).
	/// @return true if loaded successfully, false otherwise.
	/////////////////////////////////////
	bool ImageSTB::Load(const char* pFilepath)
	{
		if (m_pPixels)
		{
			stbi_image_free(m_pPixels);
			m_pPixels = nullptr;
		}
	
		if (HasExtension(pFilepath, ".jpg"))
			m_pExtension = ".jpg";
		else if (HasExtension(pFilepath, ".png"))
			m_pExtension = ".png";
		else
		{
			printf("Unsupported extension in STB loader: %s\n", pFilepath);
			return false;
		}
	
		printf("[ImageSTB::Load] Filepath = %s | Extension set to = %s\n", pFilepath, m_pExtension);
	
		FILE* pTest = nullptr;
		fopen_s(&pTest, pFilepath, "rb");
		if (!pTest)
		{
			printf("ERROR: fopen failed on path: %s\n", pFilepath);
		}
		else
		{
			printf("SUCCESS: fopen succeeded\n");
			fclose(pTest);
		}
	
		int channels = 0;
		m_pPixels = stbi_load(pFilepath, &m_width, &m_height, &channels, 3);
	
		if (!m_pPixels)
		{
			const char* pReason = stbi_failure_reason();
			printf("Failed to load image: %s\n", pFilepath);
			printf("Reason: %s\n", pReason);
	
			char debugMsg[512];
			sprintf_s(debugMsg, "Failed to load: %s\nReason: %s\n", pFilepath, pReason);
			OutputDebugStringA(debugMsg);
			return false;
		}
		return true;
	}
	
	/////////////////////////////////////
	/// @brief Save the image to disk based on extension.
	/// @param pFilepath Filepath to save to (.jpg or .png).
	/// @return true if successful.
	/////////////////////////////////////
	bool ImageSTB::Save(const char* pFilepath) const
	{
		if (!m_pPixels || m_width <= 0 || m_height <= 0)
		{
			printf("Save failed: invalid image data\n");
			return false;
		}
	
		if (strcmp(m_pExtension, ".jpg") == 0)
		{
			const int quality = 90;
			if (!stbi_write_jpg(pFilepath, m_width, m_height, 3, m_pPixels, quality))
			{
				printf("Failed to save JPEG: %s\n", pFilepath);
				return false;
			}
			printf("JPEG saved successfully: %s\n", pFilepath);
		}
		else if (strcmp(m_pExtension, ".png") == 0)
		{
			int stride = m_width * 3;
			if (!stbi_write_png(pFilepath, m_width, m_height, 3, m_pPixels, stride))
			{
				printf("Failed to save PNG: %s\n", pFilepath);
				return false;
			}
			printf("PNG saved successfully: %s\n", pFilepath);
		}
		else
		{
			printf("Unsupported format for saving: %s\n", m_pExtension);
			return false;
		}
	
		return true;
	}
	
#pragma region Setters
	/////////////////////////////////////
	/// @brief Set new pixel data (copied internally).
	/// @param w Width.
	/// @param h Height.
	/// @param pData Pointer to (w * h * 3) buffer.
	/////////////////////////////////////
	void ImageSTB::SetPixels(int w, int h, const unsigned char* pData)
	{
		if (m_pPixels)
		{
			stbi_image_free(m_pPixels);
			m_pPixels = nullptr;
		}
	
		m_width = w;
		m_height = h;
		int size = m_width * m_height * 3;
		m_pPixels = new unsigned char[size];
	
		for (int i = 0; i < size; ++i)
		{
			m_pPixels[i] = pData[i];
		}
	}
#pragma endregion
}