#include "AlgoUtils.h"

int AlgoUtils::ComputeIndice(const int xIndice, const int yIndice, const int xSize)
{
	return xIndice + yIndice * xSize;
}