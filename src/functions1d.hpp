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
		float cosBlend(float x)
		{
			return (1.f - cos(x * 3.1415f)) * 0.5f;
		}
	public:
		float interpolate(float _a, float _b, float _x)
		{
			return (1.f - cosBlend(_x))* _a*_x + cosBlend(_x)*_b*(_x - 1.f);
	//		_x = (1.f - cos(_x * 3.1415f)) * 0.5f;

	//		return _a * (1.f - _x) + _b * _x;
		}
	};

	typedef NoiseInt1D<CosInterpolation> CosIntFunction;


	float blend(float _x)
	{
		return 3.f * _x*_x - 2.f * _x * _x * _x;
	}

	class PolynomInterpolation
	{
	public:
		float interpolate(float _a, float _b, float _x)
		{
			float f_2 = _x * _x;

		//	return (_a + _b) * f_2 * _x - (2.f * _a + _b) * f_2 + _a * _x;
			return (1.f - blend(_x))* _a*_x + blend(_x)*_b*(_x-1.f);
		}
	};

	typedef NoiseInt1D<PolynomInterpolation> PolynomIntFunction;
}