#include "ImageIO.h"

#include <cassert>

#include "stb_image_write.h"
#include "stb_image.h"

std::optional<ImageInfo> ImageIO::LireImage(const char* const filename)
{
	using namespace std;

	assert(filename);

	// Nombre de canaux desirer. Si la valeur est 0, retourne le nombre de canaux dans l'image.
	static constexpr int nbCanauxDesire = 0;
	int tailleX{}, tailleY{}, nbCanaux{};

	uint8_t* const imageData = stbi_load(filename, &tailleX, &tailleY, &nbCanaux, nbCanauxDesire);

	if (not imageData)
	{
		return nullopt;
	}

	ImageInfo img{ imageData, tailleX, tailleY, nbCanaux };
	stbi_image_free(imageData);

	return img;
}

bool ImageIO::EcrireImage(const ImageInfo& imageInfo, const char* const filename)
{
	assert(filename);

	static constexpr int byteParPixel = 1;

	// Taille en byte d 'une ligne horizontale de l' image
	const int strideEnByte = byteParPixel * imageInfo.tailleX;

	return stbi_write_png(filename, imageInfo.tailleX,
		imageInfo.tailleY, imageInfo.nbCanaux,
		imageInfo.pixels.data(), strideEnByte);
}
