#ifndef TOOL_IMAGE_STB_H_INCLUDED
#define TOOL_IMAGE_STB_H_INCLUDED
#include "Image.h"

namespace gce
{
	/////////////////////////////////////
	/// @class ImageSTB
	/// @brief Generic image class using stb_image/stb_image_write (supports PNG & JPG).
	/////////////////////////////////////
	class ImageSTB : public Image
	{
	public:
#pragma region Constructors
		/////////////////////////////////////
		/// @brief Constructor.
		/////////////////////////////////////
		ImageSTB() {};

		~ImageSTB();
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
		/// @brief Get pointer to raw pixel data (RGB).
		/// @return Pointer to pixel buffer.
		/////////////////////////////////////
		const unsigned char* GetPixels() const override { return m_pPixels; }

		///////////////////////////////////////////////////////
		/// @brief Retrieves the currently set file extension.
		/// @return A string representing the extension (e.g., ".jpg").
		///////////////////////////////////////////////////////
		const char* GetExtension() const { return m_pExtension; }
#pragma endregion

#pragma region Setters
		void SetPixels(int w, int h, const unsigned char* pData) override;

		///////////////////////////////////////////////////////
		/// @brief Sets the file extension to use for saving.
		/// @param pExtension The file extension (e.g., ".jpg" or ".png").
		///////////////////////////////////////////////////////
		void SetExtension(const char* pExtension) { m_pExtension = pExtension; }
#pragma endregion
	private:
		int m_width = 0;
		int m_height = 0;
		unsigned char* m_pPixels = nullptr;
		const char* m_pExtension = nullptr;
	};
}
#endif //TOOL_IMAGE_STB_H_INCLUDED
