#ifndef TOOL_IMAGE_H_INCLUDED
#define TOOL_IMAGE_H_INCLUDED

namespace gce
{
	class Image
	{
	public:
#pragma region Constructors
		/////////////////////////////////////
		/// @brief Virtual destructor.
		/////////////////////////////////////
		virtual ~Image() {};
#pragma endregion
		/////////////////////////////////////
		/// @brief Load image from file.
		/// @param pFilepath Path to the image file.
		/// @return True if successful, false otherwise.
		/////////////////////////////////////
		virtual bool Load(const char* pFilepath) = 0;

		/////////////////////////////////////
		/// @brief Save image to file.
		/// @param pFilepath Path where to save the image.
		/// @return True if successful, false otherwise.
		/////////////////////////////////////
		virtual bool Save(const char* pFilepath) const = 0;
#pragma region Tools
		static bool HasExtension(const char* pFilepath, const char* pExtension);

		static bool FileExists(const char* pFilepath);
#pragma endregion
#pragma region Getters
		/////////////////////////////////////
		/// @brief Get image width.
		/// @return Width in pixels.
		/////////////////////////////////////
		virtual int GetWidth() const = 0;

		/////////////////////////////////////
		/// @brief Get image height.
		/// @return Height in pixels.
		/////////////////////////////////////
		virtual int GetHeight() const = 0;

		/////////////////////////////////////
		/// @brief Get raw pixel buffer.
		/// @return Pointer to pixels (RGB).
		/////////////////////////////////////
		virtual const unsigned char* GetPixels() const = 0;
#pragma endregion
#pragma region Setters
		/////////////////////////////////////
		/// @brief Replace the pixel buffer of the image.
		/// @param w Image width.
		/// @param h Image height.
		/// @param pData Pixel data to copy.
		/////////////////////////////////////
		virtual void SetPixels(int w, int h, const unsigned char* pData) = 0;
#pragma endregion
	};
}
#endif //TOOL_IMAGE_H_INCLUDED