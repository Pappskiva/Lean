#include "MathFuncs.h"
#include "MathStructs.h"

const float FastSqrt(const float x)
{
	float y = x, x2 = x * 0.5f;
	int z = *(int*)&y;
	z = 0x5f3759df - (z>>1);
	y = *(float*)&z;
	y = y * (1.5f - (x2 * y * y));

	return y * x;		// now that's real magic!
}
