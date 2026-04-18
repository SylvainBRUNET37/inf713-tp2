#ifndef MEDIAN_FILTER_H
#define MEDIAN_FILTER_H

#include "Data.h"

class MedianFilter
{
public:
	[[nodiscard]] static ImageInfo Denoise(ImageInfo image);

private:
	static constexpr auto KERNEL_SAMPLE_SIZE = 3;
	static constexpr auto KERNEL_SIZE = 9;
};

#endif
