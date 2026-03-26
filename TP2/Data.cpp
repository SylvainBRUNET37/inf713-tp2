#include "Data.h"

ImageInfo::ImageInfo(const uint8_t* data, const int tailleX, const int tailleY, const int nbCanaux)
	: tailleX{ tailleX },
	tailleY{ tailleY },
	nbCanaux{ nbCanaux }
{
	const size_t imageSize =
		static_cast<size_t>(tailleX) *
		static_cast<size_t>(tailleY) *
		static_cast<size_t>(nbCanaux);

	pixels.assign(data, data + imageSize);
}
