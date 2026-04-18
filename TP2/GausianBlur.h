#ifndef GAUSSIAN_BLUR_H
#define GAUSSIAN_BLUR_H

#include <functional>
#include <vector>

#include "Data.h"

class GaussianBlur
{
public:
	struct GausianBlurSettings
	{
		float simga = 1.0f;
		int kernelSize = 5;

		[[nodiscard]] constexpr int KernelRadius() const
		{
			return kernelSize / 2;
		}

		[[nodiscard]] constexpr float GausianDenominator() const
		{
			return 2.0f * simga * simga;
		}
	};

	[[nodiscard]] static ImageInfo Apply(ImageInfo image, GausianBlurSettings settings = {});

private:
	ImageInfo image;
	std::vector<float> kernel;
	std::vector<float> pixels;
	GausianBlurSettings settings;

	explicit GaussianBlur(ImageInfo imageToBlur, GausianBlurSettings settings = {});

	[[nodiscard]] ImageInfo BlurImage();

	void ComputeKernel();
	void GaussianHorizontalPass();
	void GaussianVerticalPass();

	// Utils

	template <typename Func>
	void ForEachPixel(Func&& function) const
	{
		for (int indiceX = 0; indiceX < image.tailleX; ++indiceX)
		{
			for (int indiceY = 0; indiceY < image.tailleY; ++indiceY)
			{
				function(indiceX, indiceY);
			}
		}
	}

	template <typename Func>
	void ForEachKernelSample(Func&& function) const
	{
		const auto kernelRadius = settings.KernelRadius();

		for (int sampleIndice = -kernelRadius; sampleIndice <= kernelRadius; ++sampleIndice)
		{
			function(sampleIndice);
		}
	}
};

#endif
