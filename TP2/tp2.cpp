////////////////////////////////////////////////////////////
// NOM: Sylvan Brunet
// Compiler   : Microsoft (R) C/C++ Optimizing Compiler Version 19.38.33145 for x86
// C++ Version: C++20
// INF 713 - TP 2 
// A mettre le 20 avril 2026 ?
/////////////////////////////////////////////////////////

#include <algorithm>
#include <array>
#include <cassert>
#include <format>
#include <iostream>
#include <memory>

#include "Data.h"
#include "GausianBlur.h"
#include "ImageUtils.h"
#include "SobelFilter.h"

int main()
{
	using namespace std;

	/////////////////////////////////////////////////////////
	// Charger une image en memoire
	/////////////////////////////////////////////////////////

	static constexpr auto INPUT_FILE_NAME = "bike.png";
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

	const ImageInfo blurredImage = GaussianBlur::Apply(*baseImage);
	static constexpr auto BLURRED_IMAGE_FILE = "barbara_flou.png";
	if (not ImageUtils::EcrireImage(blurredImage, BLURRED_IMAGE_FILE))
	{
		cerr << format("Erreur d'ecriture de l'image {}", BLURRED_IMAGE_FILE);
	}

	// 2a - Appliquer un filtre Sobel en X. Ecrire le resultat dans
	//      le fichier Gx.png.

	const SobelFilter sobelFilter{blurredImage};

	const ImageInfo gx = sobelFilter.GetHorizontalFilteredImage();
	static constexpr auto SOBEL_IMAGE_X = "Gx.png";
	if (not ImageUtils::EcrireImage(gx, SOBEL_IMAGE_X))
	{
		cerr << format("Erreur d'ecriture de l'image {}", SOBEL_IMAGE_X);
	}

	// 2b - Appliquer un filtre Sobel en Y. Ecrire le resultat dans
	//      le fichier Gy.png.

	const ImageInfo gy = sobelFilter.GetVerticalFilteredImage();
	static constexpr auto SOBEL_IMAGE_Y = "Gy.png";
	if (not ImageUtils::EcrireImage(gy, SOBEL_IMAGE_Y))
	{
		cerr << format("Erreur d'ecriture de l'image {}", SOBEL_IMAGE_Y);
	}

	// 2c - Combiner les resulats Gx et Gy pour trouver les contours.
	//      Ecrire le resultat dans barbara_countour.png.

	const ImageInfo sobelFilteredImage = sobelFilter.Get();
	static constexpr auto SOBEL_FILTERED_IMAGE = "barbara_countour.png";
	if (not ImageUtils::EcrireImage(sobelFilteredImage, SOBEL_FILTERED_IMAGE))
	{
		cerr << format("Erreur d'ecriture de l'image {}", SOBEL_FILTERED_IMAGE);
	}

	// 3 -  Appliquer un filtre median 3x3 sur barbara_bruit.png. Ecrire le
	//      resulat dans barbara_moinsdebruit.png

	return 0;
}
