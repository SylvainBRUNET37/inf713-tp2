#include "MedianFilter.h"

#include <algorithm>
#include <vector>

#include "AlgoUtils.h"

ImageInfo MedianFilter::Denoise(ImageInfo image)
{
	using namespace std;

	vector<uint8_t> pixels(image.GetImageSize(), 0);
	const int imageSizeX = image.tailleX;
	const int imageSizeY = image.tailleY;

	vector<uint8_t> kernelValues;
	kernelValues.reserve(KERNEL_SIZE);

	AlgoUtils::For(imageSizeX, imageSizeY, [&](const int indiceX, const int indiceY)
	{
		const int pixelIndice = AlgoUtils::ComputeIndice(indiceX, indiceY, imageSizeX);

		AlgoUtils::For(KERNEL_SAMPLE_SIZE, KERNEL_SAMPLE_SIZE,
		               [&](const int sampleIndiceX, const int sampleIndiceY)
		               {
			               const int sampleIndice = AlgoUtils::ComputeSampleIndice(
				               indiceX, indiceY,
				               sampleIndiceX, sampleIndiceY,
				               imageSizeX, imageSizeY, KERNEL_SAMPLE_SIZE);

						   kernelValues.emplace_back(image.pixels[sampleIndice]);
		               });

		ranges::sort(kernelValues);
		pixels[pixelIndice] = kernelValues[KERNEL_SIZE / 2];

		kernelValues.clear();
	});

	for (size_t i = 0; i < image.GetImageSize(); ++i)
	{
		image.pixels[i] = pixels[i];
	}

	return image;
}
