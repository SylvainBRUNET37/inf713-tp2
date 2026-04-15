#ifndef DATA_H
#define DATA_H

#include <cstdint>
#include <vector>

struct ImageParams
{
	int tailleX{}; // nb de pixel en X
	int tailleY{}; // nb de pixel en Y
	int nbCanaux{}; // nb de canaux par pixel. Ici 1, parce que l'image est en noir et blanc

	ImageParams(const int tailleX, const int tailleY, const int nbCanaux)
		: tailleX{tailleX}, tailleY{tailleY}, nbCanaux{nbCanaux}
	{
	}

	[[nodiscard]] size_t GetDataSize() const noexcept { return static_cast<size_t>(tailleX) * tailleY * nbCanaux; }
	[[nodiscard]] size_t GetImageSize() const noexcept { return static_cast<size_t>(tailleX) * tailleY; }
};

struct ImageInfo : ImageParams
{
	static constexpr int MIN_INTENSITY = 0;
	static constexpr int MAX_INTENSITY = 255;

	std::vector<uint8_t> pixels;

	ImageInfo(const uint8_t* data, int sizeX, int sizeY, int numChannels);
	explicit ImageInfo(std::vector<uint8_t> data, ImageParams params);
};

#endif
