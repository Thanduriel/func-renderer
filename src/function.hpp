#pragma once

#include <vector>
#include <functional>
#include <array>

#include "glm.hpp"
#include "random.hpp"
#include "argvec.hpp"

namespace Math{
	
	typedef std::function<float(float)> Function1D;
	typedef std::function<float(ArgVec<float, 2>)> Function2D;


	template<typename _T, int _Dim>
	class FunctionalScalar
	{
	public:
		static constexpr int Dimensions = _Dim;

		FunctionalScalar(_T& _func, float _scalar)
			: m_func(_func), m_scalar(_scalar)
		{}

	protected:
		_T& m_func;
		float m_scalar;
	};

	template<typename _T, int _Dim>
	class FunctionalScalarAdd : public FunctionalScalar<_T, _Dim>
	{
	public:
		using FunctionalScalar::FunctionalScalar;

		float operator()(ArgVec<float, _Dim> _val)
		{
			return m_scalar + m_func(_val);
		}
	};

	template<typename _T, int _Dim>
	class FunctionalScalarMul : public FunctionalScalar<_T, _Dim>
	{
	public:
		using FunctionalScalar::FunctionalScalar;

		float operator()(ArgVec<float, _Dim> _val)
		{
			return m_scalar * m_func(_val);
		}
	};

	// ********************************************************************* //
	template<typename _T1, typename _T2, int _Dim>
	class FunctionalBinOp
	{
		static_assert(_T1::Dimensions == _T2::Dimensions, "Only functions with the same number of arguments can be added.");
	public:
		static constexpr int Dimensions = _Dim;

		FunctionalBinOp(_T1& _func1, _T2& _func2)
			: m_func1(_func1), m_func2(_func2)
		{
		}

	protected:
		_T1& m_func1;
		_T2& m_func2;

	};

	// ********************************************************************* //
	template<typename _T1, typename _T2, int _Dim>
	class FunctionalAdd : public FunctionalBinOp < _T1, _T2, _Dim>
	{
	public:
		using FunctionalBinOp::FunctionalBinOp;

		float operator()(ArgVec<float, _Dim> _val)
		{
			return m_func1(_val) + m_func2(_val);
		}
	};

	// ********************************************************************* //
	template<typename _T1, typename _T2, int _Dim>
	class FunctionalSub : public FunctionalBinOp < _T1, _T2, _Dim>
	{
	public:
		using FunctionalBinOp::FunctionalBinOp;

		float operator()(ArgVec<float, _Dim> _val)
		{
			return m_func1(_val) - m_func2(_val);
		}
	};

	// ********************************************************************* //
	template<typename _T1, typename _T2, int _Dim>
	class FunctionalMul : public FunctionalBinOp < _T1, _T2, _Dim>
	{
	public:
		using FunctionalBinOp::FunctionalBinOp;

		float operator()(ArgVec<float, _Dim> _val)
		{
			return m_func1(_val) * m_func2(_val);
		}
	};

	// ********************************************************************* //
	template<typename _T1, typename _T2, int _Dim>
	class FunctionalDiv : public FunctionalBinOp < _T1, _T2, _Dim>
	{

	public:
		using FunctionalBinOp::FunctionalBinOp;

		float operator()(ArgVec<float, _Dim> _val)
		{
			return m_func1(_val) / m_func2(_val);
		}
	};

	// ********************************************************************* //
	template< typename _Super >
	class FunctionOperation : public _Super
	{
	public:
		template<typename... _Args>
		FunctionOperation(_Args&&... _args)
			: _Super(std::forward< _Args >(_args)...)
		{}

		//scalars
		auto operator+(float _val)
		{
			return FunctionOperation<FunctionalScalarAdd<_Super, _Super::Dimensions>>(*this, _val);
		}

		auto operator*(float _val)
		{
			return FunctionOperation<FunctionalScalarMul<_Super, _Super::Dimensions>>(*this, _val);
		}

		//binary operations
		template<typename _T2>
		auto operator+(_T2& _oth)
		{
			return FunctionOperation<FunctionalAdd<_Super, _T2, _Super::Dimensions>>(*this, _oth);
		}

		template<typename _T2>
		auto operator-(_T2& _oth)
		{
			return FunctionOperation<FunctionalSub<_Super, _T2, _Super::Dimensions>>(*this, _oth);
		}

		template<typename _T2>
		auto operator*(_T2& _oth)
		{
			return FunctionOperation<FunctionalMul<_Super, _T2, _Super::Dimensions>>(*this, _oth);
		}

		template<typename _T2>
		auto operator/(_T2& _oth)
		{
			return FunctionOperation<FunctionalDiv<_Super, _T2, _Super::Dimensions>>(*this, _oth);
		}
	};

	/* MemFunction *********************************************
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
		//argument type used to access stored values
		typedef ArgVec<int, _Dimensions> KeyType;

		static constexpr int Dimensions = _Dimensions;

		MemFunction(int _size, int _xreq = 1):
			m_size(_size)
		{
			m_values.resize((int)pow(_size, _Dimensions));
		}

		// value of this function is acquired by interpolation.
		float operator()(ArgVec<float, _Dimensions> _arg)
		{
			std::array<KeyType, 1 << Dimensions> edgePoints;
			ArgVec<float, 1 << Dimensions> values;
			ArgVec<float, 1 << Dimensions> distances;

			int border[Dimensions * 2];
			for (int i = 0; i < Dimensions; ++i)
			{
				int i2 = i * 2;
				border[i2] = (int)floor(_arg[i]);
				border[i2+1] = (int)ceil(_arg[i]);
			}
			for (int i = 0; i < 1 << Dimensions; ++i)
			{
				for (int j = 0; j < Dimensions; ++j)
					edgePoints[i][j] = border[2 * j + (1 & (i >> j))];
			//	float f = _arg[0] - lower;

				values[i] = getStored(edgePoints[i]);
				distances[i] = _arg.distance(edgePoints[i]);
			}
			return interpolate(values, distances);
		}

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


	// ********************************************************************* //
	//interpolation interface

	template<typename _Int>
	class Interpolation1D : public _Int
	{
	public:
		float interpolate(ArgVec<float, 2> _values, ArgVec<float, 2> _distances)
		{
			return _Int::interpolate(_values[0], _values[1], _distances[0]);
		}
	};

	template<typename _Int>
	using NoiseInt1D = FunctionOperation<ValueNoise<1, Interpolation1D<_Int>>>;
}