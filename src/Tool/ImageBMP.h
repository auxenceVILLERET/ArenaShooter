#ifndef TOOL_IMAGE_BMP_H_INCLUDED
#define TOOL_IMAGE_BMP_H_INCLUDED
#include "Image.h"
#include <cstdint>

namespace gce
{
	/////////////////////////////////////
	/// @brief BMP file signature used to validate BMP format (0x4D42 = "BM").
	/////////////////////////////////////
	constexpr uint16_t BMP_SIGNATURE = 0x4D42;
	class ImageBMP : public Image 
	{
	public:
#pragma region Constructors
		/////////////////////////////////////
		/// @brief Default constructor.
		/////////////////////////////////////
		ImageBMP() {};
		ImageBMP(const ImageBMP& other);
		ImageBMP& operator=(const ImageBMP& other);
		~ImageBMP();
#pragma endregion
		bool Load(const char* pFilepath) override;
		bool Save(const char* pFilepath) const override;
#pragma region Getters
		/////////////////////////////////////
		/// @brief Get image width in pixels.
		/// @return Width.
		/////////////////////////////////////
		int GetWidth() const override { return m_width; }

		/////////////////////////////////////
		/// @brief Get image height in pixels.
		/// @return Height.
		/////////////////////////////////////
		int GetHeight() const override { return m_height; }

		/////////////////////////////////////
		/// @brief Access image pixel buffer (RGB).
		/// @return Pointer to pixel buffer.
		/////////////////////////////////////
		const unsigned char* GetPixels() const override { return m_pPixels; }
#pragma endregion
#pragma region Setters
		void SetPixels(int w, int h, const unsigned char* pData) override;
#pragma endregion

	private:
		int m_width = 0;
		int m_height = 0;
		unsigned char* m_pPixels = nullptr;
	};
}
#endif //TOOL_IMAGE_BMP_H_INCLUDED