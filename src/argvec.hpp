#pragma once

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
		//construct from the required amount of arguments
		ArgVec(_ValT _x){ x = _x; };
		ArgVec(_ValT _x, _ValT _y){ x = _x; y = _y; };
		ArgVec(_ValT _x, _ValT _y, int _z){ x = _x; y = _y; z = _z; };

		// return value of _ind dimension
		_ValT operator[](size_t _ind) { return data[_ind]; }
	};

}