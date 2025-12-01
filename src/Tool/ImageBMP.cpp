#include "ImageBMP.h"
#include <stdio.h>  // fopen_s, fread, fwrite
#include <cstdint>

namespace gce
{

#pragma pack(push,1)
    /////////////////////////////////////////////////////////
    /// @brief BMP file header structure (Windows BITMAP).
    ///
    /// This struct represents the binary layout of a standard
    /// 24-bit uncompressed BMP file header, starting from the
    /// file signature to the DIB header fields. It is packed
    /// to match the exact byte alignment required by the BMP
    /// file format.
    /// @note Use with `#pragma pack(push, 1)` to prevent padding.
    /////////////////////////////////////////////////////////
    struct BMPHeader
    {
        uint16_t signature = BMP_SIGNATURE;
        uint32_t fileSize = 0;
        uint32_t reserved = 0;
        uint32_t dataOffset = 54;
        uint32_t headerSize = 40;
        int32_t width = 0;
        int32_t height = 0;
        uint16_t planes = 1;
        uint16_t bitsPerPixel = 24;
        uint32_t compression = 0;
        uint32_t imageSize = 0;
        int32_t xPixelsPerM = 0;
        int32_t yPixelsPerM = 0;
        uint32_t colorsUsed = 0;
        uint32_t importantColors = 0;
    };
#pragma pack(pop)
    
#pragma region Constructors
    /////////////////////////////////////
    /// @brief Copy constructor.
    /// @param other ImageBMP instance to copy.
    /////////////////////////////////////
    ImageBMP::ImageBMP(const ImageBMP& other)
    {
        m_width = other.m_width;
        m_height = other.m_height;
        int size = m_width * m_height * 3;
    
        if (other.m_pPixels)
        {
            m_pPixels = new unsigned char[size];
            for (int i = 0; i < size; ++i)
            {
                m_pPixels[i] = other.m_pPixels[i];
            }
        }
        else
        {
            m_pPixels = nullptr;
        }
    }
    
    /////////////////////////////////////
    /// @brief Assignment operator.
    /// @param other ImageBMP to copy from.
    /// @return Reference to this.
    /////////////////////////////////////
    ImageBMP& ImageBMP::operator=(const ImageBMP& other)
    {
        if (this != &other)
        {
            if (m_pPixels) delete[] m_pPixels;
    
            m_width = other.m_width;
            m_height = other.m_height;
            int size = m_width * m_height * 3;
    
            if (other.m_pPixels)
            {
                m_pPixels = new unsigned char[size];
                for (int i = 0; i < size; ++i)
                {
                    m_pPixels[i] = other.m_pPixels[i];
                }
            }
            else
            {
                m_pPixels = nullptr;
            }
        }
        return *this;
    }
    
    /////////////////////////////////////
    /// @brief Destructor.
    /////////////////////////////////////
    ImageBMP::~ImageBMP()
    {
        if (m_pPixels)
        {
            delete[] m_pPixels;
            m_pPixels = nullptr;
        }
    }
#pragma endregion
    
    /////////////////////////////////////
    /// @brief Load BMP image from file.
    /// @param pFilepath File path to read from.
    /// @return True if loading succeeded.
    /////////////////////////////////////
    bool ImageBMP::Load(const char* pFilepath)
    {
        FILE* pFile = nullptr;
        if (fopen_s(&pFile, pFilepath, "rb") || !pFile)
        { 
            return false;
        }
    
        BMPHeader header;
        fread(&header, sizeof(header), 1, pFile);
    
        if (header.signature != BMP_SIGNATURE || header.bitsPerPixel != 24 || header.compression != 0)
        {
            fclose(pFile);
            return false;
        }
    
        m_width = header.width;
        m_height = header.height;
        size_t rowSize = ((m_width * 3 + 3) / 4) * 4;
    
        if (m_pPixels)
        {
            delete[] m_pPixels;
        }
    
        m_pPixels = new unsigned char[m_width * m_height * 3];
        fseek(pFile, header.dataOffset, SEEK_SET);
    
        for (int y = 0; y < m_height; ++y)
        {
            fread(&m_pPixels[(m_height - 1 - y) * m_width * 3], 1, m_width * 3, pFile);
            fseek(pFile, rowSize - m_width * 3, SEEK_CUR);
        }
        fclose(pFile);
        return true;
    }
    
    /////////////////////////////////////
    /// @brief Save BMP image to file.
    /// @param pFilepath File path to write to.
    /// @return True if save succeeded.
    /////////////////////////////////////
    bool ImageBMP::Save(const char* pFilepath) const
    {
        FILE* pFile = nullptr;
        if (fopen_s(&pFile, pFilepath, "wb") || !pFile)
        {
            return false;
        }
    
        BMPHeader header;
        header.width = m_width;
        header.height = m_height;
        size_t rowSize = ((m_width * 3 + 3) / 4) * 4;
        header.imageSize = rowSize * m_height;
        header.fileSize = header.imageSize + sizeof(BMPHeader);
    
        fwrite(&header, sizeof(BMPHeader), 1, pFile);
    
        size_t paddingSize = rowSize - m_width * 3;
        unsigned char padding[3] = { 0, 0, 0 };
    
        for (int y = m_height - 1; y >= 0; --y)
        {
            fwrite(&m_pPixels[y * m_width * 3], 1, m_width * 3, pFile);
            if (paddingSize > 0)
            { 
                fwrite(padding, 1, paddingSize, pFile);
            }
        }
    
        fclose(pFile);
        return true;
    }
    
#pragma region Setters
    /////////////////////////////////////
    /// @brief Replace image content with new pixel buffer.
    /// @param w Width in pixels.
    /// @param h Height in pixels.
    /// @param pData Pixel data (size = w * h * 3).
    /////////////////////////////////////
    void ImageBMP::SetPixels(int w, int h, const unsigned char* pData)
    {
        if (m_pPixels)
        {
            delete[] m_pPixels;
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