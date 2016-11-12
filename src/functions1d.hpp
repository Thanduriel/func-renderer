#pragma once

#include "function.hpp"

namespace Math{
	class LinearInterpolation
	{
	public:
		float interpolate(float _a, float _b, float _x)
		{
			return _a * (1.f - _x) + _b * _x;
		}
	};

	typedef NoiseInt1D<LinearInterpolation> LinearIntFunction;

	class CosInterpolation
	{
	public:
		float interpolate(float _a, float _b, float _x)
		{
			_x = (1.f - cos(_x * 3.1415f)) * 0.5f;

			return _a * (1.f - _x) + _b * _x;
		}
	};

	typedef NoiseInt1D<CosInterpolation> CosIntFunction;

	class PolynomInterpolation
	{
	public:
		float interpolate(float _a, float _b, float _x)
		{
			float f_2 = _x * _x;

			return (_a + _b) * f_2 * _x - (2.f * _a + _b) * f_2 + _a * _x;
		}
	};

	typedef NoiseInt1D<PolynomInterpolation> PolynomIntFunction;
}