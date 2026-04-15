#ifndef SOBEL_FILTER_H
#define SOBEL_FILTER_H

#include <array>
#include <vector>

#include "Data.h"

class SobelFilter
{
public:
    explicit SobelFilter(const ImageInfo& baseImage);

    [[nodiscard]] ImageInfo GetHorizontalFilteredImage() const;
    [[nodiscard]] ImageInfo GetVerticalFilteredImage() const;
    [[nodiscard]] ImageInfo Get() const;

private:
	static constexpr auto KERNEL_SAMPLE_SIZE = 3;
	static constexpr auto KERNEL_SIZE = KERNEL_SAMPLE_SIZE * KERNEL_SAMPLE_SIZE;
    static constexpr std::array<float, KERNEL_SIZE> horizontalKernel =
    {
        -1, 0, 1,
        -2, 0, 2,
        -1, 0, 1
    };
    static constexpr std::array<float, KERNEL_SIZE> verticalKernel =
    {
        -1, -2, -1,
         0,  0,  0,
         1,  2,  1
    };

    std::vector<float> gx;
    std::vector<float> gy;
    std::vector<float> pixels;
    ImageParams imageParams;

    void ApplyFilter(const ImageInfo& image);

    [[nodiscard]] ImageInfo GetFilteredImage(const std::vector<float>& data) const;
};

#endif