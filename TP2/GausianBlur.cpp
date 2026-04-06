#include "GausianBlur.h"

#include <algorithm>
#include <format>
#include <iostream>

#include "ImageUtils.h"

GausianBlur::GausianBlur(ImageInfo image) :
	blurredImage{std::move(image)},
	blurredPixels(blurredImage.GetImageSize(), 0.0f)
{
	const auto kernel = ComputeKernel();

	HorizontalPass(kernel);
	VerticalPass(kernel);

	for (size_t i = 0; i < blurredImage.GetImageSize(); ++i)
	{
		blurredImage.pixels[i] = static_cast<uint8_t>(blurredPixels[i]);
	}

	if (not ImageUtils::EcrireImage(blurredImage, "bbr.png"))
	{
		std::cerr << std::format("Erreur de lecture de l'image {}", "bbr.png");
	}
}

std::vector<float> GausianBlur::ComputeKernel()
{
	std::vector kernel(KERNEL_SIZE, 0.0f);

	float weightSum = 0.0f;
	ForEachKernelSample([&](const int sampleIndice)
	{
		const float distance = static_cast<float>(sampleIndice * sampleIndice);
		const float weight = std::exp(-distance / DENO);

		weightSum += weight;
		kernel[sampleIndice + KERNEL_RADIUS] = weight;
	});

	for (auto& weight : kernel)
	{
		if (weightSum > 0)
		{
			weight /= weightSum;
		}
	}

	return kernel;
}

void GausianBlur::HorizontalPass(const std::vector<float>& kernel)
{
	ForEachImagePixel([&](const int indiceX, const int indiceY)
	{
		float weightSum = 0.0f;

		ForEachKernelSample([&](const int sampleIndice)
		{
			const auto sampleX = std::clamp(
				indiceX + sampleIndice, 0, blurredImage.tailleX - 1);
			const size_t baseImageIndice = sampleX + indiceY * blurredImage.tailleX;

			const float baseImagePixel = blurredImage.pixels[baseImageIndice];
			const float weight = kernel[sampleIndice + KERNEL_RADIUS];

			weightSum += baseImagePixel * weight;
		});

		const int imageIndice = ComputeIndice(indiceX, indiceY, blurredImage.tailleX);
		blurredPixels[imageIndice] = weightSum;
	});
}

void GausianBlur::VerticalPass(const std::vector<float>& kernel)
{
	ForEachImagePixel([&](const int indiceX, const int indiceY)
	{
		float weightSum = 0.0f;

		ForEachKernelSample([&](const int sampleIndice)
		{
			const auto sampleY = std::clamp(
				indiceY + sampleIndice, 0, blurredImage.tailleY - 1);
			const size_t imageIndice = indiceX + sampleY * blurredImage.tailleX;

			const float imagePixel = blurredPixels[imageIndice];
			const float weight = kernel[sampleIndice + KERNEL_RADIUS];

			weightSum += imagePixel * weight;
		});

		const int imageIndice = ComputeIndice(indiceX, indiceY, blurredImage.tailleX);
		blurredPixels[imageIndice] = weightSum;
	});
}

// Utils

int GausianBlur::ComputeIndice(const int xIndice, const int yIndice, const int xSize)
{
	return xIndice + yIndice * xSize;
}

void GausianBlur::ForEachPixelKernel(const std::function<void(int, int, int)>& function) const
{
	ForEachImagePixel([this, &function](const int indiceX, const int indiceY)
	{
		ForEachKernelSample([this, &function, indiceX, indiceY](const int sampleIndice)
		{
			function(indiceX, indiceY, sampleIndice);
		});
	});
}

void GausianBlur::ForEachImagePixel(const std::function<void(int, int)>& function) const
{
	for (int indiceX = 0; indiceX < blurredImage.tailleX; ++indiceX)
	{
		for (int indiceY = 0; indiceY < blurredImage.tailleY; ++indiceY)
		{
			function(indiceX, indiceY);
		}
	}
}

void GausianBlur::ForEachKernelSample(const std::function<void(int)>& function)
{
	for (int sampleIndice = -KERNEL_RADIUS; sampleIndice <= KERNEL_RADIUS; ++sampleIndice)
	{
		function(sampleIndice);
	}
}
