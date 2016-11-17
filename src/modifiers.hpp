#pragma once

#include "function.hpp"

namespace Math {

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

	typedef FO<Modifier<Clamp>> ClampFunction;
}