#ifndef IMAGE_UTILS_H
#define IMAGE_UTILS_H

#include <optional>

#include "Data.h"

namespace ImageUtils
{
	// LireImage
	// en entree
	// - filename : le nom du fichier a lire sur le disque.
	// - imageInfo : recevera le contenue de l'image lue sur le disque
	// en sortie
	// 0 indique un echec. != 0 indique un succes.
	[[nodiscard]] std::optional<ImageInfo> LireImage(const char* filename);

	// EcrireImage
	// en entree
	// - imageInfo : l'image a ecrire sur disque.
	// - filename : le nom du fichier a ecrire sur le disque.
	// en sortie
	//  0 indique un echec. != 0 indique un succes.
	[[nodiscard]] bool EcrireImage(const ImageInfo& imageInfo, const char* filename);

	[[nodiscard]] bool GenererImageBruitee(const ImageInfo& imageOriginal, const char* filename);
}

#endif
