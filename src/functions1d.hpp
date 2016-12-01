#pragma once

#include "function.hpp"

namespace Math {
	namespace Func1D {

		// ***************************************************** //
		class LinearInterpolation
		{
		public:
			float interpolate(float _a, float _b, float _x) const
			{
				return _a * (1.f - _x) + _b * _x;
			}
		};

		typedef NoiseInt1D<LinearInterpolation> LinearIntFunction;

		// ***************************************************** //
		class CosInterpolation
		{
			static float cosBlend(float x)
			{
				return (1.f - cos(x * 3.1415f)) * 0.5f;
			}
		public:
			float interpolate(float _a, float _b, float _x) const
			{
			//	return (1.f - cosBlend(_x))* _a*_x + cosBlend(_x)*_b*(_x - 1.f);
					_x = (1.f - cos(_x * 3.1415f)) * 0.5f;

					return _a * (1.f - _x) + _b * _x;
			}
		};

		typedef NoiseInt1D<CosInterpolation> CosIntFunction;


		// ***************************************************** //
		float cubicBlend(float _x)
		{
			return 3.f * _x*_x - 2.f * _x * _x * _x;
		}
		float blend(float x)
		{
			return x * x * x * (x * (x * 6 - 15.f) + 10.f);
		}

		class PolynomInterpolation
		{
		public:
			float interpolate(float _a, float _b, float _x) const
			{
				float f_2 = _x * _x;

				return (1.f - cubicBlend(_x))* _a*_x + cubicBlend(_x)*_b*(_x - 1.f);
			}
		};

		typedef NoiseInt1D<PolynomInterpolation> PerlinNoise1D;
	}
}