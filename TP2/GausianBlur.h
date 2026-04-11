#pragma once

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

	void ForEachPixel(const std::function<void(int, int)>& function) const;
	void ForEachKernelSample(const std::function<void(int)>& function) const;

	[[nodiscard]] static int ComputeIndice(int xIndice, int yIndice, int xSize);
};
