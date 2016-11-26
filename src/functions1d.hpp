#pragma once

#include "function.hpp"

namespace Math {
	namespace Func1D {
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
			//	return (1.f - cosBlend(_x))* _a*_x + cosBlend(_x)*_b*(_x - 1.f);
					_x = (1.f - cos(_x * 3.1415f)) * 0.5f;

					return _a * (1.f - _x) + _b * _x;
			}
		};

		typedef NoiseInt1D<CosInterpolation> CosIntFunction;


		float blend2(float _x)
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
			float interpolate(float _a, float _b, float _x)
			{
				float f_2 = _x * _x;

				return (2 * _a + _b) * f_2 * f_2 - (3.f*_a + _b)*f_2*_x + _a * _x; // x^4, a2 = 0
	//			return (_a + _b) * f_2 * _x - (2.f * _a + _b) * f_2 + _a * _x; // x^3
			//return (1.f - blend2(_x))* _a*_x + blend2(_x)*_b*(_x - 1.f); // alternative form
			}
		};

		typedef NoiseInt1D<PolynomInterpolation> PolynomIntFunction;

		class Polynom5Interpolation
		{
		public:
			float interpolate(float _a, float _b, float _x)
			{
				return (1.f - blend(_x))* _a*_x + blend(_x)*_b*(_x - 1.f);
			}
		};
		typedef NoiseInt1D<Polynom5Interpolation> Polynom5IntFunction;
	}
}