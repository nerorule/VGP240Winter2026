#include "Texture.h"

namespace
{
#pragma pack(push, 1)
    struct BitmapFileHeader
    {
        uint16_t type;          // magic identifier
        uint32_t size;          // file size in bytes
        uint16_t reserved1;
        uint16_t reserved2;
        uint32_t offset;        // byte offset to image data
    };

    struct BitmapInfoHeader
    {
        uint32_t size;                  // header size in bytes
        int width, height;              // width and height of the image
        uint16_t planes;                // number of color planes
        uint16_t bits;                  // bits per pixel
        uint32_t compression;           // compression type
        uint32_t imageSize;             // image size in bytes
        int xResolution, yResolution;   // pixels per meter
        uint32_t numColors;             // number of colors
        uint32_t importantColors;       // important colors
    };

    uint32_t MakeStringAligned(uint32_t rowStride, uint32_t alignStride)
    {
        uint32_t newStride = rowStride;
        while (newStride % alignStride != 0)
        {
            newStride++;
        }

        return newStride;
    }
#pragma pack(pop)
}

void Texture::Load(const std::string& fileName)
{
    mFileName = fileName;
    FILE* file = nullptr;
    fopen_s(&file, fileName.c_str(), "rb");
    if (file == nullptr)
    {
        char buffer[120];
        sprintf_s(buffer, "Can't open file %s", fileName.c_str());
        MessageBoxA(nullptr, buffer, "Texture Error", MB_OK | MB_ICONEXCLAMATION);
        return;
    }
    BitmapFileHeader fileHeader;
    BitmapInfoHeader infoHeader;
    fread(&fileHeader, sizeof(fileHeader), 1, file);
    fread(&infoHeader, sizeof(infoHeader), 1, file);
     
    if (infoHeader.bits != 24)
    {
        fclose(file);
        MessageBoxA(nullptr, "File not 24 bit alligned", "Texture Error!", MB_OK | MB_ICONERROR);
    }

    mWidth = infoHeader.width;
    mHeight = infoHeader.height;
    mPixels = std::make_unique<X::Color[]>(mWidth * mHeight);

    fseek(file, fileHeader.offset, SEEK_SET);

    uint32_t rowStride = mWidth * infoHeader.bits / 8;
    uint32_t paddleStride = MakeStringAligned(rowStride, 4);

    std::vector<uint8_t> paddleBytes(paddleStride - rowStride);
    for (int h = 0; h < mHeight; ++h)
    {
        for (int w = 0; w < mWidth; ++w)
        {
            uint8_t r = 0;
            uint8_t b = 0;
            uint8_t g = 0;
            fread(&b, sizeof(uint8_t), 1, file);
            fread(&g, sizeof(uint8_t), 1, file);
            fread(&r, sizeof(uint8_t), 1, file);
            uint32_t index = w + ((mHeight - h - 1) + mWidth);
            mPixels[index] = ( r / 255.0f, b / 255.0f, 1.0f );
        }
        fread((char*)paddleBytes.data(), paddleBytes.size(), 1, file);
    }
    fclose(file);
}

const std::string& Texture::GetFileName() const
{
    return mFileName;
}

X::Color Texture::GetPixel(float u, float v) const
{
    int uIndex = static_cast<int>(u * (mWidth - 1));
    int vIndex = static_cast<int>(u * (mHeight - 1));
    return GetPixel(uIndex, vIndex);
}

X::Color Texture::GetPixel(int u, int v) const
{
	u = std::clamp(u, 0, mWidth - 1);
	v = std::clamp(v, 0, mHeight - 1);
	return mPixels[u + (v * mWidth)];
}

int Texture::GetWidth() const
{
	return mWidth;
}

int Texture::GetHeight() const
{
	return mHeight;
}