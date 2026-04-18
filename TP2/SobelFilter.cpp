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
	const int imageSizeX = image.tailleX;

	ForEachPixel([&](const int indiceX, const int indiceY)
	{
		const int pixelIndice = AlgoUtils::ComputeIndice(indiceX, indiceY, imageSizeX);

		// Compute gx & gy
		ForEachKernelSample([&](const int kx, const int ky)
		{
			const int ix = indiceX + kx - 1;
			const int iy = indiceY + ky - 1;
			const int kernelIndice = AlgoUtils::ComputeIndice(ix, iy, imageSizeX);

			const float pixel = image.pixels[kernelIndice];

			const float kxVal = horizontalKernel[ky * 3 + kx];
			const float kyVal = verticalKernel[ky * 3 + kx];

			gx[pixelIndice] += pixel * kxVal;
			gy[pixelIndice] += pixel * kyVal;
		});

		ComputeGradientMagnitude(pixelIndice);
	});
}

void SobelFilter::ComputeGradientMagnitude(const int pixelIndice)
{
	using namespace std;

	const float magnitude = sqrt(
		gx[pixelIndice] * gx[pixelIndice] + gy[pixelIndice] * gy[pixelIndice]);

	pixels[pixelIndice] = ranges::min(magnitude, 255.0f);
}
