#ifndef TOOL_SPRITE_SHEET_H_INCLUDED
#define TOOL_SPRITE_SHEET_H_INCLUDED

#include "Image.h"
#include "ImageBMP.h"
#include "ImageSTB.h"

namespace gce
{
	///////////////////////////////////////////////////////
	/// @brief Handles image composition and atlas generation from multiple image inputs.
	///////////////////////////////////////////////////////
	class SpriteSheet
	{
	public:
#pragma region Constructors
		///////////////////////////////////////////////////////
		/// @brief Default constructor.
		///////////////////////////////////////////////////////
		SpriteSheet() {};
		
		~SpriteSheet();
#pragma endregion

#pragma region OrientationEnum
		///////////////////////////////////////////////////////
		/// @brief Defines layout orientation for sprite placement.
		///////////////////////////////////////////////////////
		enum class Orientation
		{
			Horizontal,
			Vertical
		};
#pragma endregion
		void AddImage(Image* pImage, const char* pName);
		
		bool Generate(Orientation orientation);

		bool Save(const char* pFilepath);

		void PrintAtlas() const;

		bool SaveAtlasAsJson(const char* pFilepath);

		bool GenerateSpriteSheet(Orientation orientation, const char* pFilepath);
	private:
		static const int MAX_IMAGES = 128;
		static const int MAX_SPRITES = 128;

		struct SpriteInfo
		{
			const char* name;
			int x, y, width, height;
		};

		Image* m_pImages[MAX_IMAGES] = { nullptr };
		const char* m_pNames[MAX_IMAGES];
		SpriteInfo m_atlas[MAX_SPRITES];

		int m_imageCount = 0;
		int m_atlasCount = 0;

		Image* m_pFinalImage = nullptr;
	};
}
#endif // !TOOL_SPRITE_SHEET_H_INCLUDED
