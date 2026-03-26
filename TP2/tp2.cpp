////////////////////////////////////////////////////////////
// NOM: Sylvan Brunet
// Compiler   : Microsoft (R) C/C++ Optimizing Compiler Version 19.38.33145 for x86
// C++ Version: c++20
// INF 713 - TP 2 
// A mettre le 20 avril 2026 ?
/////////////////////////////////////////////////////////

#include <algorithm>
#include <array>
#include <cstdio>
#include <format>
#include <iostream>
#include <memory>
#include <random> // Pour generer l'image avec bruit

#include "Data.h"
#include "ImageIO.h"

constexpr int minIntensity = 0;
constexpr int maxIntensity = 255;

namespace
{
    [[nodiscard]] bool GenererImageBruitee(const ImageInfo& imageOriginal, const char* filename)
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
                    blackWhiteRandom(rng) ? maxIntensity : minIntensity;
            }
        }

        return ImageIO::EcrireImage(resultatInfo, filename);
    }
}


int main()
{
    using namespace std;

    /////////////////////////////////////////////////////////
    // Charger une image en memoire
    /////////////////////////////////////////////////////////

    static constexpr auto INPUT_FILE_NAME = "barbara.png";
    const auto imageInfo = ImageIO::LireImage(INPUT_FILE_NAME);
    if (not imageInfo)
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
