#pragma once

#include "function.hpp"
#include "functions1d.hpp"
#include "functions2d.hpp"

namespace Math {

	/* Modifier ************************************
	 * A one dimensional function used to manipulate the output of another function.
	 */

	template<typename _Mod>
	class Modifier : public _Mod
	{
	public:
		static constexpr int Dimensions = 1;

		// _Mod constructor
		using _Mod::_Mod;

		float operator()(float _val)
		{
			return filter(_val);
		}
	};


	// ********************************************************* //
	class Clamp
	{
	public:
		Clamp(float _lower, float _upper)
			: m_lower(_lower), m_upper(_upper)
		{}

		float filter(float _val)
		{
			return glm::clamp(_val, m_lower, m_upper);
		}

	private:
		float m_lower;
		float m_upper;
	};

	typedef FuncOp<Modifier<Clamp>> ClampFunction;

	// ********************************************************* //
	class Absolute
	{
	public:
		float filter(float _val)
		{
			return abs(_val);
		}
	};

	typedef FuncOp<Modifier<Absolute>> AbsFunction;

	// ********************************************************* //
	class Inverse
	{
	public:
		float filter(float _val)
		{
			return 1.f - _val;
		}
	};

	typedef FuncOp<Modifier<Inverse>> InvFunction;

	// ********************************************************* //
	class Square
	{
	public:
		float filter(float _val)
		{
			return _val * _val;
		}
	};

	typedef FuncOp<Modifier<Square>> SqrFunction;

	// ********************************************************* //
	class Distort
	{
	public:
		static constexpr int Dimensions = 2;

		auto operator()(ArgVec<float, 2> _val)
		{
			static glm::vec2 base(50.f, 50.f);
			static CubicIntFunction2D f(1501, 0.1f, 0x12CC345);
			float x = f(AVec2(_val.x, 0.f))*10.f;

			float distance = sqrt((base.x - _val.x) * (base.x - _val.x) + (base.y - _val.y) * (base.y - _val.y) +1.f);
			return ArgVec<float, 2>(_val.x + x, _val.y-x);
		}
	};

	typedef FuncOp<Distort> DistortFunction;
}