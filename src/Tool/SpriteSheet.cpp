#include "SpriteSheet.h"
#include "Core/JsonParser.h"

#include <cstdint>
#include <string>

//$(SolutionDir)..\src;$(SolutionDir)..\src\Tool;$(SolutionDir)..\src\Core;% (AdditionalIncludeDirectories);$(SolutionDir)..\src\Tool\Lib

namespace gce
{

#pragma region Constructors
	///////////////////////////////////////////////////////
	/// @brief Destructor. Frees any allocated resources.
	///////////////////////////////////////////////////////
	SpriteSheet::~SpriteSheet()
	{
		for (int i = 0; i < m_imageCount; ++i)
		{
			delete m_pImages[i];
			m_pImages[i] = nullptr;
		}
		delete m_pFinalImage;
		m_pFinalImage = nullptr;
	}
#pragma endregion
	
	///////////////////////////////////////////////////////
	/// @brief Adds an image to the sprite list for generation.
	/// @param pImage Pointer to the image object.
	/// @param pName Associated name for the image in the atlas.
	///////////////////////////////////////////////////////
	void SpriteSheet::AddImage(Image* pImage, const char* pName)
	{
		if (m_imageCount >= MAX_IMAGES) return;
	
		m_pImages[m_imageCount] = pImage;
		m_pNames[m_imageCount] = pName;
		m_imageCount++;
	}
	
	///////////////////////////////////////////////////////
	/// @brief Generates the final sprite sheet based on layout.
	/// @param orientation Specifies horizontal or vertical layout.
	/// @return True if successful, false otherwise.
	///////////////////////////////////////////////////////
	bool SpriteSheet::Generate(Orientation orientation)
	{
		if (m_imageCount == 0) return false;
	
		const char* pExtension = nullptr;
		if (Image::HasExtension(m_pNames[0], ".bmp")) pExtension = ".bmp";
		else if (Image::HasExtension(m_pNames[0], ".jpg")) pExtension = ".jpg";
		else if (Image::HasExtension(m_pNames[0], ".png")) pExtension = ".png";
		else
		{
			printf("Unsupported extension for output image: %s\n", m_pNames[0]);
			return false;
		}
	
		delete m_pFinalImage;
		m_pFinalImage = nullptr;
	
		if (strcmp(pExtension, ".bmp") == 0)
		{
			m_pFinalImage = new ImageBMP();
		}
		else
		{
			auto pSTB = new ImageSTB();
			pSTB->SetExtension(pExtension);
			m_pFinalImage = pSTB;
		}
	
		int totalWidth = 0, totalHeight = 0, maxWidth = 0, maxHeight = 0;
	
		for (int i = 0; i < m_imageCount; ++i)
		{
			int w = m_pImages[i]->GetWidth();
			int h = m_pImages[i]->GetHeight();
	
			if (orientation == Orientation::Horizontal)
			{
				totalWidth += w;
				if (h > maxHeight) maxHeight = h;
			}
			else
			{
				totalHeight += h;
				if (w > maxWidth) maxWidth = w;
			}
		}
	
		int finalWidth = (orientation == Orientation::Horizontal) ? totalWidth : maxWidth;
		int finalHeight = (orientation == Orientation::Horizontal) ? maxHeight : totalHeight;
		int totalPixels = finalWidth * finalHeight * 3;
	
		uint8_t* pFinalPixels = new uint8_t[totalPixels];
		memset(pFinalPixels, 0, totalPixels);
	
		int offsetX = 0, offsetY = 0;
	
		for (int i = 0; i < m_imageCount; ++i)
		{
			Image* pImg = m_pImages[i];
			const uint8_t* pPixels = pImg->GetPixels();
			int w = pImg->GetWidth();
			int h = pImg->GetHeight();
	
			for (int y = 0; y < h; ++y)
			{
				for (int x = 0; x < w; ++x)
				{
					int srcIndex = (y * w + x) * 3;
					int dstX = (orientation == Orientation::Horizontal) ? x + offsetX : x;
					int dstY = (orientation == Orientation::Horizontal) ? y : y + offsetY;
					int dstIndex = (dstY * finalWidth + dstX) * 3;
	
					pFinalPixels[dstIndex + 0] = pPixels[srcIndex + 0];
					pFinalPixels[dstIndex + 1] = pPixels[srcIndex + 1];
					pFinalPixels[dstIndex + 2] = pPixels[srcIndex + 2];
				}
			}
	
			if (m_atlasCount < MAX_SPRITES)
			{
				m_atlas[m_atlasCount++] =
				{
					m_pNames[i],
					(orientation == Orientation::Horizontal) ? offsetX : 0,
					(orientation == Orientation::Horizontal) ? 0 : offsetY,
					w, h
				};
			}
	
			if (orientation == Orientation::Horizontal)
				offsetX += w;
			else
				offsetY += h;
		}
	
		m_pFinalImage->SetPixels(finalWidth, finalHeight, pFinalPixels);
		delete[] pFinalPixels;
		return true;
	}
	
	///////////////////////////////////////////////////////
	/// @brief Saves the final generated sprite sheet to a file.
	/// @param pFilepath Output filepath (without or with extension).
	/// @return True if saved successfully, false otherwise.
	///////////////////////////////////////////////////////
	bool SpriteSheet::Save(const char* pFilepath)
	{
		char finalPath[256];
		strcpy_s(finalPath, sizeof(finalPath), pFilepath);
	
		const char* pExtension = nullptr;
	
		if (dynamic_cast<ImageBMP*>(m_pFinalImage)) pExtension = ".bmp";
		else if (auto pSTB = dynamic_cast<const ImageSTB*>(m_pFinalImage)) pExtension = pSTB->GetExtension();
		else
		{
			printf("Unknown image type for saving.\n");
			return false;
		}
	
		if (!pExtension || pExtension[0] == '\0')
		{
			printf("Invalid or missing extension in final image.\n");
			return false;
		}
	
		if (!Image::HasExtension(pFilepath, pExtension)) strcat_s(finalPath, sizeof(finalPath), pExtension);
	
		if (Image::FileExists(finalPath))
		{
			printf("File : %s already exists.\n", finalPath);
			return false;
		}
	
		if (m_pFinalImage->Save(finalPath))
		{
			printf("File : %s has been saved.\n", finalPath);
			return true;
		}
	
		printf("File : %s couldn't be saved.\n", finalPath);
		return false;
	}
	
	///////////////////////////////////////////////////////
	/// @brief Displays all image positions and sizes in the atlas.
	///////////////////////////////////////////////////////
	void SpriteSheet::PrintAtlas() const
	{
		for (const auto& sprite : m_atlas)
		{
			printf("%s: x=%d, y=%d, w=%d, h=%d\n",
				sprite.name, sprite.x, sprite.y, sprite.width, sprite.height);
		}
	}
	
	///////////////////////////////////////////////////////
	/// @brief Saves the sprite atlas metadata to a .json file.
	/// @param pFilepath Path to save the JSON file.
	/// @return True if JSON was written successfully, false otherwise.
	///////////////////////////////////////////////////////
	bool SpriteSheet::SaveAtlasAsJson(const char* pFilepath)
	{
		char finalPath[256];
		strcpy_s(finalPath, sizeof(finalPath), pFilepath);

		const char* ext = ".json";
		if (!Image::HasExtension(pFilepath, ext)) strcat_s(finalPath, sizeof(finalPath), ext);

		if (Image::FileExists(finalPath))
		{
			printf("File: %s already exists. JSON save aborted.\n", finalPath);
			return false;
		}

		JsonObject* pRoot = new JsonObject();

		for (int i = 0; i < m_imageCount; ++i)
		{
			const SpriteInfo& sprite = m_atlas[i];

			JsonObject* pSpriteObj = new JsonObject();
			pSpriteObj->AddPair("x", new JsonInteger(sprite.x));
			pSpriteObj->AddPair("y", new JsonInteger(sprite.y));
			pSpriteObj->AddPair("width", new JsonInteger(sprite.width));
			pSpriteObj->AddPair("height", new JsonInteger(sprite.height));

			pRoot->AddPair(sprite.name, pSpriteObj);
		}

		File* pFile = JsonParser::Serialize(pRoot, finalPath);
		if (!pFile)
		{
			printf("Failed to save JSON atlas: %s\n", finalPath);
			delete pRoot;
			return false;
		}

		delete pRoot;
		printf("JSON atlas saved successfully: %s\n", finalPath);
		return true;
	}
	
	///////////////////////////////////////////////////////
	/// @brief Runs the full generation pipeline: image + JSON.
	/// @param orientation Layout mode for the sprite sheet.
	/// @param pFilepath Base filepath used for both outputs.
	/// @return True if generation and saving succeeded.
	///////////////////////////////////////////////////////
	bool SpriteSheet::GenerateSpriteSheet(Orientation orientation, const char* pFilepath)
	{
		if (Generate(orientation) && Save(pFilepath) && SaveAtlasAsJson(pFilepath))
		{
			printf("Sprite sheet successfully generated: %s\n", pFilepath);
			return true;
		}
		printf("Failed generating sprite sheet: %s\n", pFilepath);
		return false;
	}
}

