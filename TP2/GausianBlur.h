#pragma once

#include <functional>
#include <vector>

#include "Data.h"

class GausianBlur
{
public:
	explicit GausianBlur(ImageInfo image);

private:
	static constexpr float SIGMA_SQUARED = 1.0f;
	static constexpr float DENO = 2 * SIGMA_SQUARED;

	static constexpr int KERNEL_SIZE = 31;
	static constexpr int KERNEL_RADIUS = KERNEL_SIZE / 2;

	ImageInfo blurredImage;
	std::vector<float> blurredPixels;

	void VerticalPass(const std::vector<float>& kernel);
	void HorizontalPass(const std::vector<float>& kernel);

	void ForEachPixelKernel(const std::function<void(int, int, int)>& function) const;
	void ForEachImagePixel(const std::function<void(int, int)>& function) const;
	static void ForEachKernelSample(const std::function<void(int)>& function);

	[[nodiscard]] static std::vector<float> ComputeKernel();
	[[nodiscard]] static int ComputeIndice(int xIndice, int yIndice, int xSize);
};
