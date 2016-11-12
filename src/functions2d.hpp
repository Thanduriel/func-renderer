#pragma once

#include "function.hpp"

namespace Math {
	class LinearInterpolation2D
	{
	public:
		float interpolate(float _a, float _b, float _x)
		{
			return _a * (1.f - _x) + _b * _x;
		}
	};

	typedef FunctionOperation<ValueNoise<2, LinearInterpolation2D>> LinearIntFunction2D;

}