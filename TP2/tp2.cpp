////////////////////////////////////////////////////////////
// NOM: Sylvan Brunet
// Compiler   : Microsoft (R) C/C++ Optimizing Compiler Version 19.38.33145 for x86
// C++ Version: c++20
// INF 713 - TP 2 
// A mettre le 20 avril 2026 ?
/////////////////////////////////////////////////////////

#include <algorithm>
#include <array>
#include <cassert>
#include <format>
#include <iostream>
#include <memory>
#include <ranges>

#include "Data.h"
#include "ImageUtils.h"

int main()
{
	using namespace std;

	/////////////////////////////////////////////////////////
	// Charger une image en memoire
	/////////////////////////////////////////////////////////

	static constexpr auto INPUT_FILE_NAME = "barbara.png";
	const auto baseImage = ImageUtils::LireImage(INPUT_FILE_NAME);
	if (not baseImage)
	{
		cerr << format("Erreur de lecture de l'image {}", INPUT_FILE_NAME);
		return EXIT_FAILURE;
	}

	/////////////////////////////////////////////////////////
	// Traitement de l'image
	/////////////////////////////////////////////////////////

	// 1 -  Appliquer un flou gaussien sur l'image avec un noyau de 5X5.
	//      Note: Le noyau est separable. Ecrire le resulat dans le fichier
	//      barbara_flou.png. Utiliser un sigma = 1.0.
	//      Note: http://dev.theomader.com/gaussian-kernel-calculator/
	//            Pour les coefficients gaussiens.

	static constexpr float SIGMA_SQUARED = 1.0f;
	static constexpr float DENO = 2 * SIGMA_SQUARED;

	static constexpr int KERNEL_SIZE_X = 5;
	static constexpr int KERNEL_SIZE_Y = 5;

	const auto& baseImagePixels = baseImage->pixels;
	const auto baseImageSizeX = baseImage->tailleX;
	const auto baseImageSizeY = baseImage->tailleY;

	vector blurredPixels(baseImage->GetImageSize(), 0.0f);

	const auto computeIndice = [](
		auto xIndice, auto yIndice, const auto xSize, const auto ySize)
		{
			xIndice = (xIndice % xSize + xSize) % xSize;
			yIndice = (yIndice % ySize + ySize) % ySize;

			const auto result = xIndice + yIndice * xSize;
			assert(result >= 0 && result < xSize * ySize);

			return result;
		};


	for (int blurryPixelX = 0; blurryPixelX < baseImageSizeX; ++blurryPixelX)
	{
		for (int blurryPixelY = 0; blurryPixelY < baseImageSizeY; ++blurryPixelY)
		{
			float weightSum = 0.0f;

			// Current pixel
			const int blurredPixelIndice = 
				computeIndice(blurryPixelX, blurryPixelY, baseImageSizeX, baseImageSizeY);

			const float sample1 = baseImagePixels[blurredPixelIndice];
			const float weight1 = exp(0.0f);

			blurredPixels[blurredPixelIndice] += sample1 * weight1;
			weightSum += weight1;

			// X
			for (const int xIndice : views::iota(-KERNEL_SIZE_X / 2, KERNEL_SIZE_X / 2 + 1))
			{
				if (xIndice == 0) continue;

				const int indice = 
					computeIndice(blurryPixelX + xIndice, blurryPixelY, baseImageSizeX, baseImageSizeY);

				const float sample = baseImagePixels[indice];
				const float form = static_cast<float>(xIndice * xIndice) / DENO;
				const float weight = exp(-form);

				blurredPixels[blurredPixelIndice] += sample * weight;
				weightSum += weight;
			}

			// Y
			for (const int yIndice : views::iota(-KERNEL_SIZE_Y / 2, KERNEL_SIZE_Y / 2 + 1))
			{
				if (yIndice == 0) continue;

				const int indice = 
					computeIndice(blurryPixelX, blurryPixelY + yIndice, baseImageSizeX, baseImageSizeY);

				const float sample = baseImagePixels[indice];
				const float form = static_cast<float>(yIndice * yIndice) / DENO;
				const float weight = exp(-form);

				blurredPixels[blurredPixelIndice] += sample * weight;
				weightSum += weight;
			}

			blurredPixels[blurredPixelIndice] /= weightSum;
		}
	}

	auto blurredImage = *baseImage;
	for (size_t i = 0; i < blurredImage.GetImageSize(); ++i)
	{
		blurredImage.pixels[i] = static_cast<uint8_t>(blurredPixels[i]);
	}

	static constexpr auto BLURRED_FILE_NAME = "barbara_flou.png";
	if (not ImageUtils::EcrireImage(blurredImage, BLURRED_FILE_NAME))
	{
		cerr << format("Erreur de lecture de l'image {}", BLURRED_FILE_NAME);
		return EXIT_FAILURE;
	}

	// 2a - Appliquer un filtre Sobel en X. Ecrire le resultat dans
	//      le fichier Gx.png.

	// 2b - Appliquer un filtre Sobel en Y. Ecrire le resultat dans
	//      le fichier Gy.png.

	// 2c - Combiner les resulats Gx et Gy pour trouver les contours.
	//      Ecrire le resultat dans barbara_countour.png.

	// 3 -  Appliquer un filtre median 3x3 sur barbara_bruit.png. Ecrire le
	//      resulat dans barbara_moinsdebruit.png

	return 0;
}


/*
const auto computeIndex = [](
    const auto xIndice, const auto yIndice, const auto sizeY)
    {
        return xIndice * sizeY + yIndice;
    };

const auto computePixelX = [](
    const auto indice, const auto sizeY)
    {
        return indice / sizeY;
    };

const auto computePixelY = [](
    const auto indice, const auto sizeX)
    {
        return indice / sizeX;
    };

static constexpr auto SIZE_X = 5;
static constexpr auto SIZE_Y = 5;
for (int blurryPixelIndice = 0; cmp_less(blurryPixelIndice, blurredPixels.size()); ++blurryPixelIndice)
{
    for (int xIndice = blurryPixelIndice - SIZE_X / 2; xIndice < blurryPixelIndice + SIZE_X / 2; ++xIndice)
    {
        for (int yIndice = blurryPixelIndice - SIZE_Y / 2; yIndice < blurryPixelIndice + SIZE_Y / 2; ++yIndice)
        {
            const int sampleIndice = computeIndex(xIndice, yIndice, SIZE_Y);
            const int sample = baseImage->pixels[sampleIndice];

            const int pixelDiffX = abs(
                computePixelX(blurryPixelIndice, baseImage->tailleY) -
                computePixelX(sampleIndice, baseImage->tailleY));
            const int pixelDiffY = abs(
                computePixelY(blurryPixelIndice, baseImage->tailleY) -
                computePixelY(sampleIndice, baseImage->tailleY));

            const int squaredDistance = pow(pixelDiffX, 2) + pow(pixelDiffY, 2);

            blurredPixels[blurryPixelIndice] += exp(squaredDistance / DENO);

            // TODO: check bounds + conversions
        }
    }

    for (const uint8_t baseImagePixel : baseImage->pixels)
    {

    }
}
*/
