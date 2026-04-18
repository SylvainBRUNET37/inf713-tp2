#include "GausianBlur.h"

#include <algorithm>
#include <cassert>
#include <format>

#include "AlgoUtils.h"
#include "ImageUtils.h"

GaussianBlur::GaussianBlur(ImageInfo imageToBlur, const GausianBlurSettings settings) :
	image{std::move(imageToBlur)},
	kernel(settings.kernelSize, 0.0f),
	pixels(image.GetImageSize(), 0.0f),
	settings{settings}
{
}

// Gaussian blur

ImageInfo GaussianBlur::Apply(ImageInfo imageToBlur, const GausianBlurSettings blurSettings)
{
	assert(blurSettings.kernelSize % 2 != 0 && "Cannot blur image with a pair kernel size");

	return GaussianBlur{std::move(imageToBlur), blurSettings}.BlurImage();
}

ImageInfo GaussianBlur::BlurImage()
{
	ComputeKernel();
	GaussianHorizontalPass();
	GaussianVerticalPass();

	for (size_t i = 0; i < image.GetImageSize(); ++i)
	{
		image.pixels[i] = static_cast<uint8_t>(pixels[i]);
	}

	return std::move(image);
}

void GaussianBlur::ComputeKernel()
{
	float weightSum = 0.0f;
	ForEachKernelSample([&](const int sampleIndice)
	{
		const float distance = static_cast<float>(sampleIndice * sampleIndice);
		const float weight = std::exp(-distance / settings.GausianDenominator());

		weightSum += weight;
		kernel[sampleIndice + settings.KernelRadius()] = weight;
	});

	for (auto& weight : kernel)
	{
		if (weightSum > 0)
		{
			weight /= weightSum;
		}
	}
}

void GaussianBlur::GaussianHorizontalPass()
{
	AlgoUtils::For(image.tailleX, image.tailleY,[&](const int indiceX, const int indiceY)
	{
		float weightSum = 0.0f;

		ForEachKernelSample([&](const int sampleIndice)
		{
			const auto sampleX = std::clamp(
				indiceX + sampleIndice, 0, image.tailleX - 1);
			const size_t baseImageIndice = sampleX + indiceY * image.tailleX;

			const float baseImagePixel = image.pixels[baseImageIndice];
			const float weight = kernel[sampleIndice + settings.KernelRadius()];

			weightSum += baseImagePixel * weight;
		});

		const int imageIndice = AlgoUtils::ComputeIndice(indiceX, indiceY, image.tailleX);
		pixels[imageIndice] = weightSum;
	});
}

void GaussianBlur::GaussianVerticalPass()
{
	AlgoUtils::For(image.tailleX, image.tailleY, [&](const int indiceX, const int indiceY)
	{
		float weightSum = 0.0f;

		ForEachKernelSample([&](const int sampleIndice)
		{
			const auto sampleY = std::clamp(
				indiceY + sampleIndice, 0, image.tailleY - 1);
			const size_t imageIndice = indiceX + sampleY * image.tailleX;

			const float imagePixel = pixels[imageIndice];
			const float weight = kernel[sampleIndice + settings.KernelRadius()];

			weightSum += imagePixel * weight;
		});

		const int imageIndice = AlgoUtils::ComputeIndice(indiceX, indiceY, image.tailleX);
		pixels[imageIndice] = weightSum;
	});
}
