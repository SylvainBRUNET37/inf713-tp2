#ifndef ALGO_UTILS_H
#define ALGO_UTILS_H

namespace AlgoUtils
{
	[[nodiscard]] int ComputeIndice(int xIndice, int yIndice, int xSize);

    [[nodiscard]] int ComputeSampleIndice(
        int pixelX, int pixelY, int kernelX, int kernelY, int imageWidth, int imageHeight, int kernelSampleSize);

    template <typename Func>
    void For(const int sizeX, const int sizeY, Func&& function)
    {
        for (int indiceX = 0; indiceX < sizeX; ++indiceX)
        {
            for (int indiceY = 0; indiceY < sizeY; ++indiceY)
            {
                function(indiceX, indiceY);
            }
        }
    }
};

#endif
