#pragma once

#include "function.hpp"
#include "functions1d.hpp"

namespace Math {
	class LinearInterpolation2D : public Func1D::LinearInterpolation
	{
	public:
		float interpolate(ArgVec<float, 4> _values, ArgVec<float, 2> _distances)
		{
			assert(_distances.x >= 0.f && _distances.y <= 1.f);
			float v0 = LinearInterpolation::interpolate(_values[0], _values[1], _distances.x);
			float v1 = LinearInterpolation::interpolate(_values[2], _values[3], _distances.x);

			return LinearInterpolation::interpolate(v0, v1, _distances.y);
		}
	};

	typedef FuncOp<ValueNoise<2, LinearInterpolation2D>> LinearIntFunction2D;

	class CubicInterpolation2D : public Func1D::Polynom5Interpolation
	{
	public:
		float interpolate(ArgVec<AVec2, 4> _values, glm::vec2 _distances)
		{
			using namespace glm;
/*			using namespace Func1D;
			float v0 = Polynom5Interpolation::interpolate(_values[0].x, _values[1].x, _distances.x);
			float v1 = Polynom5Interpolation::interpolate(_values[2].x, _values[3].x, _distances.x);

			float u0 = Polynom5Interpolation::interpolate(_values[0].y, _values[2].y, _distances.y);
			float u1 = Polynom5Interpolation::interpolate(_values[1].y, _values[3].y, _distances.y);

			return v0 * (1.f - _distances.y) + v1 * _distances.y + u0 * (1.f - _distances.x) + u1 * _distances.x;*/

			float sx = Func1D::blend2(_distances.x);
			float w0 = (1.f - sx) * glm::dot((glm::vec2)_values[0], _distances) + sx * glm::dot((glm::vec2)_values[1], _distances-vec2(1.f,0.f));
			float w1 = (1.f - sx) * glm::dot((glm::vec2)_values[2], _distances-vec2(0.f,1.f)) + sx * glm::dot((glm::vec2)_values[3], _distances-vec2(1.f,1.f));

			float sy = Func1D::blend2(_distances.y);
			return (1.f - sy) * w0 + sy * w1;
		}
	};

	typedef FuncOp<GradientNoise<2, CubicInterpolation2D>> CubicIntFunction2D;
}