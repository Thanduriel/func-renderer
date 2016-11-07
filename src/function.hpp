#include <vector>
#include <functional>

#include "glm.hpp"
#include "random.hpp"
#include "argvec.hpp"

namespace Math{
	
	typedef std::function<float(float)> Function1D;
	typedef std::function<float(float, float)> Function2D;


	/* MemFunction *******************************************oo
	 * Function that keeps discrete values in memory.
	 * @param _Dimensions Data dimensions this function has.
	 *	One corresponds to a function of the style f(x)=y.
	 * @param _Int A type that implements:
	 *	float interpolate(float _a, float _b, float _x)
	 */
	template< int _Dimensions, typename _Int>
	class MemFunction : public _Int
	{
	public:

		const int Dimensions = _Dimensions;

		MemFunction(int _size, int _xreq = 1)
		{
			m_values.resize((int)pow(_size, _Dimensions));
		}

		// value of this function is acquired by interpolation.
		float operator()(float _arg)
		{
			float lower = floor(_arg);
			float upper = ceil(_arg);
			float f = _arg - lower;

			return interpolate(getStored((int)lower), getStored((int)upper), f);
		}

		//argument type used to access stored values
		typedef ArgVec<int, _Dimensions> KeyType;

	protected:
		// Calculates the index at which the element can be found in m_values.
		int getIndex(KeyType _arg)
		{
			int ind = _arg[0];
			int size = m_size;
			for (int i = 1; i < _Dimensions; ++i)
			{
				ind += size * _arg[i];
				size *= m_size;
			}

			return ind;
		}

		// Return value stored at _arg.
		float getStored(KeyType _arg)
		{
			return m_values[getIndex(_arg)];
		}

		// Assigns all discrete values using _init.
		void setAllStored(std::function<float(KeyType)> _init)
		{
			for (int i = 0; i < (int)m_values.size(); ++i)
				m_values[i] = _init(i);
		}
//	private:
		std::vector <float> m_values;
		int m_size; //size in any direction
	};

	// ********************************************************************* //
	template< int _Dimensions, typename _Int >
	class ValueNoise : public MemFunction<_Dimensions, _Int>
	{
	public:
		ValueNoise(int _size, int _xreq = 1)
			: MemFunction(_size, _xreq)
		{
			Util::Random rng(0x1a23f);
			setAllStored([&](MemFunction<_Dimensions, _Int>::KeyType _arg)
			{
				return rng.uniform(-1.f, 1.f);
			});
		}
	};

	class LinearInterpolation
	{
	public:
		float interpolate(float _a, float _b, float _x)
		{
			return _a * (1.f - _x) + _b * _x;
		}
	};

	typedef ValueNoise<1, LinearInterpolation> LinearIntFunction;

	class CosInterpolation
	{
	public:
		float interpolate(float _a, float _b, float _x)
		{
			_x = (1.f - cos(_x * 3.1415f)) * 0.5f;

			return _a * (1.f - _x) + _b * _x;
		}
	};

	typedef ValueNoise<1, CosInterpolation> CosIntFunction;

	class PolynomInterpolation
	{
	public:
		float interpolate(float _a, float _b, float _x)
		{
			float f_2 = _x * _x;

			return (_a + _b) * f_2 * _x  - (2.f * _a + _b) * f_2 + _a * _x;
		}
	};

	typedef ValueNoise<1, PolynomInterpolation> PolynomIntFunction;
}