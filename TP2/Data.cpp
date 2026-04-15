#include "Data.h"

ImageInfo::ImageInfo(const uint8_t* data, const int sizeX, const int sizeY, const int numChannels)
	: ImageParams{sizeX, sizeY, numChannels}
{
	const size_t imageSize = GetDataSize();
	pixels.assign(data, data + imageSize);
}

ImageInfo::ImageInfo(std::vector<uint8_t> data, const ImageParams params)
	: ImageParams{params},
	  pixels{std::move(data)}
{
}
