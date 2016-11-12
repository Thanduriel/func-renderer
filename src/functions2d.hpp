#pragma once

#include "function.hpp"

namespace Math {
	class LinearInterpolation2D
	{
	public:
		float interpolate(ArgVec<float, 4> _values, ArgVec<float, 4> _distances)
		{
			float res = 0.f;
			for (int i = 0; i < 4; ++i)
			{
				res = _values[i]/* * (1.f - _distances[i])*/;
			}

			return _values[0];
		}
	};

	typedef FunctionOperation<ValueNoise<2, LinearInterpolation2D>> LinearIntFunction2D;

}