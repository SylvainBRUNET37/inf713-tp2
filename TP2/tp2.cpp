/////////////////////////////////////////////////////////
// NOM:
// INF 713 - TP 2
// A mettre le 20 avril 2020
/////////////////////////////////////////////////////////

#include "stb_image.h"
#include "stb_image_write.h"

#include <algorithm>
#include <array>
#include <limits>
#include <memory>

#include <cassert>
#include <cstdio>
#include <cstdlib>

// Pour generer l'image avec bruit.
#include <random>

constexpr int minIntensity = 0;
constexpr int maxIntensity = 255;

struct ImageInfo
{
    ImageInfo() 
        : data(nullptr)
        , tailleX(0)
        , tailleY(0)
        , nbCanaux(0)
    {}

    uint8_t* data;
    int tailleX;  // nb de pixel en X
    int tailleY;  // nb de pixel en Y
    int nbCanaux; // nb de canaux par pixel. Ici 1, parce que l'image est en
                  // noir et blanc
};

// LireImage
// en entree
// - filename : le nom du fichier a lire sur le disque.
// - imageInfo : recevera le contenue de l'image lue sur le disque
// en sortie
// 0 indique un echec. != 0 indique un succes.
int LireImage(const char* filename, ImageInfo* outImageInfo)
{
    assert(filename);
    assert(outImageInfo);

    // Nombre de canaux desirer. Si la valeur est 0, retourne le nombre de
    // canaux dans l'image.
    const int nbCanauxDesire = 0;
    outImageInfo->data = static_cast<uint8_t*>(
        stbi_load(filename, &outImageInfo->tailleX, &outImageInfo->tailleY,
                  &outImageInfo->nbCanaux, nbCanauxDesire));

    return outImageInfo->data ? 1 : 0;
}

// EcrireImage
// en entree
// - imageInfo : l'image a ecrire sur disque.
// - filename : le nom du fichier a ecrire sur le disque.
// en sortie
//  0 indique un echec. != 0 indique un succes.
int EcrireImage(const ImageInfo imageInfo, const char* filename)
{
    assert(filename);

    const int byteParPixel = 1;
    // Taille en byte d 'une ligne horizontale de l'image
    const int strideEnByte = byteParPixel * imageInfo.tailleX;
    return stbi_write_png(filename, imageInfo.tailleX, imageInfo.tailleY,
                          imageInfo.nbCanaux, (void*)imageInfo.data,
                          strideEnByte);
}

void GenererImageBruitee(const ImageInfo& imageOriginal, const char* filename)
{
    std::mt19937 rng(0);
    std::uniform_int_distribution<std::mt19937::result_type> IsNoiseRandom(
        1, 100); // distribution in range [1, 100]
    std::uniform_int_distribution<std::mt19937::result_type> blackWhiteRandom(
        0, 1); // distribution in range [0, 1]

    int tailleImage = imageOriginal.tailleX * imageOriginal.tailleY;
    ImageInfo resultatInfo;
    resultatInfo.tailleX = imageOriginal.tailleX;
    resultatInfo.tailleY = imageOriginal.tailleY;
    resultatInfo.nbCanaux = imageOriginal.nbCanaux;
    std::unique_ptr<uint8_t[]> data(new uint8_t[tailleImage]);
    resultatInfo.data = data.get();

    for (int i = 0; i < tailleImage; ++i)
    {
        uint8_t value = imageOriginal.data[i];

        if (IsNoiseRandom(rng) != 100)
        {
            resultatInfo.data[i] = value;
        }
        else
        {
            resultatInfo.data[i] =
                blackWhiteRandom(rng) ? maxIntensity : minIntensity;
        }
    }

    EcrireImage(resultatInfo, filename);
}

int main()
{
    /////////////////////////////////////////////////////////
    // Charger une image en memoire
    /////////////////////////////////////////////////////////
    const char* filename = "barbara.png";
    ImageInfo imageInfo;
    const int result = LireImage(filename, &imageInfo);
    if (result == 0) {
        std::printf("Erreur de lecture de l'image %s", filename);
        std::exit(1);
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
