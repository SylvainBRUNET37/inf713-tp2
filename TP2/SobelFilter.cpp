#include "SobelFilter.h"

#include <algorithm>
#include <cmath>
#include <vector>
#include <ranges>

#include "AlgoUtils.h"

SobelFilter::SobelFilter(const ImageInfo& baseImage) :
	gx(baseImage.GetImageSize(), 0.0f),
	gy(baseImage.GetImageSize(), 0.0f),
	pixels(baseImage.GetImageSize(), 0.0f),
	imageParams{baseImage}
{
	ApplyFilter(baseImage);
}

ImageInfo SobelFilter::GetHorizontalFilteredImage() const
{
	return GetFilteredImage(gx);
}

ImageInfo SobelFilter::GetVerticalFilteredImage() const
{
	return GetFilteredImage(gy);
}

ImageInfo SobelFilter::Get() const
{
	return GetFilteredImage(pixels);
}

ImageInfo SobelFilter::GetFilteredImage(const std::vector<float>& data) const
{
	using namespace std;

	vector<uint8_t> filteredPixels(imageParams.GetImageSize(), 0.0);

	ranges::transform(data, filteredPixels.begin(), [](const float pixel)
	{
		return static_cast<uint8_t>(clamp(pixel, 0.0f, 255.0f));
	});

	return ImageInfo{std::move(filteredPixels), imageParams};
}

void SobelFilter::ApplyFilter(const ImageInfo& image)
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
					const int kernelIndice = AlgoUtils::ComputeIndice(ix, iy, imageSizeX);

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
}
