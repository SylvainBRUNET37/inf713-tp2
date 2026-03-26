#ifndef DATA_H
#define DATA_H

#include <cstdint>
#include <vector>

struct ImageInfo
{
	std::vector<uint8_t> pixels;
	int tailleX{}; // nb de pixel en X
	int tailleY{}; // nb de pixel en Y
	int nbCanaux{}; // nb de canaux par pixel. Ici 1, parce que l'image est en noir et blanc

	ImageInfo(const uint8_t* data, int tailleX, int tailleY, int nbCanaux);

	[[nodiscard]] size_t GetDataSize() const noexcept { return static_cast<size_t>(tailleX) * tailleY * nbCanaux; }
};

#endif
