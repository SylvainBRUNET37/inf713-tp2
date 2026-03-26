#include "ImageUtils.h"

#include <cassert>
#include <random>

#include "stb_image_write.h"
#include "stb_image.h"

std::optional<ImageInfo> ImageUtils::LireImage(const char* const filename)
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

bool ImageUtils::EcrireImage(const ImageInfo& imageInfo, const char* const filename)
{
	assert(filename);

	static constexpr int byteParPixel = 1;

	// Taille en byte d 'une ligne horizontale de l' image
	const int strideEnByte = byteParPixel * imageInfo.tailleX;

	return stbi_write_png(filename, imageInfo.tailleX,
		imageInfo.tailleY, imageInfo.nbCanaux,
		imageInfo.pixels.data(), strideEnByte);
}

bool ImageUtils::GenererImageBruitee(const ImageInfo& imageOriginal, const char* filename)
{
    std::mt19937 rng(0);
    std::uniform_int_distribution<std::mt19937::result_type> IsNoiseRandom(
        1, 100); // distribution in range [1, 100]
    std::uniform_int_distribution<std::mt19937::result_type> blackWhiteRandom(
        0, 1); // distribution in range [0, 1]

    const int tailleImage = imageOriginal.tailleX * imageOriginal.tailleY;
    ImageInfo resultatInfo
    {
        new uint8_t[tailleImage],
        imageOriginal.tailleX,
        imageOriginal.tailleY,
        imageOriginal.nbCanaux
    };

    for (int i = 0; i < tailleImage; ++i)
    {
        const uint8_t value = imageOriginal.pixels[i];

        if (IsNoiseRandom(rng) != 100)
        {
            resultatInfo.pixels[i] = value;
        }
        else
        {
            resultatInfo.pixels[i] =
                blackWhiteRandom(rng) ? ImageInfo::MAX_INTENSITY : ImageInfo::MIN_INTENSITY;
        }
    }

    return EcrireImage(resultatInfo, filename);
}
