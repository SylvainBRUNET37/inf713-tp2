#include "SobelFilter.h"

#include <algorithm>
#include <cmath>
#include <vector>
#include <ranges>

#include "AlgoUtils.h"

SobelFilter::SobelFilter(ImageInfo baseImage) :
	image{ std::move(baseImage) },
	gx(image.GetImageSize(), 0.0f),
	gy(image.GetImageSize(), 0.0f),
	pixels(image.GetImageSize(), 0.0f)
{
	ApplyFilter();
}

ImageInfo SobelFilter::GetHorizontalFilteredImage()
{
}

ImageInfo SobelFilter::GetVerticalFilteredImage()
{
}

ImageInfo SobelFilter::Get()
{
	return image;
}

void SobelFilter::ApplyFilter()
{
	using namespace std;

	const auto imageSizeX = image.tailleX;
	const auto imageSizeY = image.tailleY;

	for (int indiceY = 1; indiceY < imageSizeY - 1; ++indiceY)
	{
		for (int indiceX = 1; indiceX < imageSizeX - 1; ++indiceX)
		{
			const int pixelIndice = AlgoUtils::ComputeIndice(indiceX, indiceY, imageSizeX);

			for (int ky = 0; ky < KERNEL_SAMPLE_SIZE; ++ky)
			{
				for (int kx = 0; kx < KERNEL_SAMPLE_SIZE; ++kx)
				{
					const int ix = indiceX + kx - 1;
					const int iy = indiceY + ky - 1;
					const int kernelIndice = AlgoUtils::ComputeIndice(ix, iy,imageSizeX);

					const float pixel = image.pixels[kernelIndice];

					const float kx_val = horizontalKernel[ky * 3 + kx];
					const float ky_val = verticalKernel[ky * 3 + kx];

					gx[pixelIndice] += pixel * kx_val;
					gy[pixelIndice] += pixel * ky_val;
				}
			}

			float magnitude = sqrt(
				gx[pixelIndice] * gx[pixelIndice] + gy[pixelIndice] * gy[pixelIndice]);

			magnitude = min(magnitude, 255.0f);

			pixels[pixelIndice] = magnitude;
		}
	}

	ranges::transform(pixels, image.pixels.begin(),
		[](const float pixel)
		{
			return static_cast<uint8_t>(pixel);
		});
}
