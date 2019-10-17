#pragma once

#include "glm.hpp"

namespace Math{

	/* Vector with template specified dimension.
	 * For operations on vectors use glm::vec
	 */

	template<typename _ValT, int _D>
	struct Component
	{
		_ValT data[_D];
	};

	// specializations for named access of the first 3 dimensions 
	template<typename _ValT>
	struct Component<_ValT, 1>
	{
		union{
			_ValT x;
			_ValT data[1];
		};
	};

	template<typename _ValT>
	struct Component<_ValT, 2>
	{
		union{
			struct{ _ValT x, y; };
			_ValT data[2];
		};
	};

	template<typename _ValT>
	struct Component<_ValT, 3>
	{
		union{
			struct{ _ValT x, y, z; };
			_ValT data[3];
		};
	};

	//the actual vector class
	template<typename _ValT, int _D>
	struct ArgVec : public Component<_ValT, _D>
	{
		typedef Component<_ValT, _D> ST;
		
		ArgVec() { };
		//construct with one arg for every dimension
		template<bool Enable = true, typename = typename std::enable_if< _D == 1 && Enable >::type>
		ArgVec(_ValT _x) { ST::x = _x; };
		template<bool Enable = true, typename = typename std::enable_if< _D == 2 && Enable >::type>
		ArgVec(_ValT _x, _ValT _y){ ST::x = _x; ST::y = _y; };
		template<bool Enable = true, typename = typename std::enable_if< _D == 3 && Enable >::type>
		ArgVec(_ValT _x, _ValT _y, _ValT _z){ ST::x = _x; ST::y = _y; ST::z = _z; };

		// return value of _ind dimension
		_ValT operator[](size_t _ind) const { return ST::data[_ind]; }
		_ValT& operator[](size_t _ind) { return ST::data[_ind]; }

		template<typename _ValOther>
		_ValT distance(const ArgVec<_ValOther, _D>& _oth) const
		{
			_ValT ret = 0.f;
			for(int i = 0; i < _D; ++i)
				ret += (ST::data[i] - (_ValT)_oth[i]) * (ST::data[i] - (_ValT)_oth[i]);

			return sqrt(ret);
		}

		operator glm::vec2() const
		{
			return glm::vec2(ST::x, ST::y);
		}

		operator float()
		{
			return ST::x;
		}
	};

	template<typename _ValT, int _D>
	auto operator*(float _lhs, ArgVec<_ValT, _D> _rhs)
	{
		for (int i = 0; i < _D; ++i)
			_rhs[i] *= _lhs;

		return _rhs;
	}

	//some common types
	typedef ArgVec<float, 2> AVec2;
}