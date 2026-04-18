#include "AlgoUtils.h"

#include <algorithm>

int AlgoUtils::ComputeIndice(const int xIndice, const int yIndice, const int xSize)
{
	return xIndice + yIndice * xSize;
}

int AlgoUtils::ComputeSampleIndice(const int pixelX, const int pixelY, const int kernelX, const int kernelY,
                                   const int imageWidth,
                                   const int imageHeight,
                                   const int kernelSampleSize)
{
	const int offset = kernelSampleSize / 2;

	int x = pixelX + kernelX - offset;
	int y = pixelY + kernelY - offset;

	x = std::clamp(x, 0, imageWidth - 1);
	y = std::clamp(y, 0, imageHeight - 1);

	return ComputeIndice(x, y, imageWidth);
}
