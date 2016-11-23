#pragma once

#include <vector>
#include <functional>
#include <array>
#include <queue>

#include "glm.hpp"
#include "random.hpp"
#include "argvec.hpp"

namespace Math{
	
	// common used function signatures
	typedef std::function<float(float)> Function1D;
	typedef std::function<float(ArgVec<float, 2>)> Function2D;


	//all used math functions should inherit from this type to make them identifiable in compile time
	class Function
	{};

	// individual operations
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
	template<typename _T1, typename _T2, int _Dim>
	class FunctionalComposition
	{
	//	static_assert(_T1::Dimensions == 1, "The outer function must be one dimensional.");
	public:

		static constexpr int Dimensions = _Dim;

		FunctionalComposition(_T1& _func1, _T2& _func2)
			: m_func1(_func1), m_func2(_func2)
		{
		}

		float operator()(ArgVec<float, _T2::Dimensions> _val)
		{
			return m_func1(m_func2(_val));
		}

	private:
		_T1& m_func1;
		_T2& m_func2;
	};

	// ********************************************************************* //
	// This type gives the given _Super type algebraic operations.
	// The name of this class should be short to avoid warning C4503
	// "decorated name length exceeded, name was truncated"
	template< typename _Super >
	class FuncOp : public _Super
	{
	public:
		template<typename... _Args>
		FuncOp(_Args&&... _args)
			: _Super(std::forward< _Args >(_args)...)
		{}

		//scalars
		auto operator+(float _val)
		{
			return FuncOp<FunctionalScalarAdd<_Super, _Super::Dimensions>>(*this, _val);
		}

		auto operator*(float _val)
		{
			return FuncOp<FunctionalScalarMul<_Super, _Super::Dimensions>>(*this, _val);
		}

		//binary operations
		template<typename _T2>
		auto operator+(_T2& _oth)
		{
			return FuncOp<FunctionalAdd<_Super, _T2, _Super::Dimensions>>(*this, _oth);
		}

		template<typename _T2>
		auto operator-(_T2& _oth)
		{
			return FuncOp<FunctionalSub<_Super, _T2, _Super::Dimensions>>(*this, _oth);
		}

		template<typename _T2>
		auto operator*(_T2& _oth)
		{
			return FuncOp<FunctionalMul<_Super, _T2, _Super::Dimensions>>(*this, _oth);
		}

		template<typename _T2>
		auto operator/(_T2& _oth)
		{
			return FuncOp<FunctionalDiv<_Super, _T2, _Super::Dimensions>>(*this, _oth);
		}

		//composition
		template<typename _T2>
		auto operator[](_T2& _oth)
		{
			return FuncOp<FunctionalComposition<_Super, _T2, _T2::Dimensions>>(*this, _oth);
		}

	};

	//operators for lhs float
	template <typename _T>
	static auto operator+(float _val, _T& _super)
	{
		return FuncOp<FunctionalScalarAdd<_T, _T::Dimensions>>(_super, _val);
	}
	template <typename _T, typename = std::enable_if< std::is_base_of<Function, _T>::value >::type>
	static auto operator*(float _val, _T& _super)
	{
		return FuncOp<FunctionalScalarMul<_T, _T::Dimensions>>(_super, _val);
	}

	/* MemFunction *********************************************
	 * Function that keeps discrete values in memory.
	 * @param _Dimensions Data dimensions this function has.
	 *	One corresponds to a function of the style f(x)=y.
	 * @param _Int A type that implements:
	 *	float interpolate(float _a, float _b, float _x)
	 */
	template< int _Dimensions, typename _ValueT, typename _Int>
	class MemFunction : public _Int, public Function
	{
	public:
		//argument type used to access stored values
		typedef ArgVec<int, _Dimensions> KeyType;

		static constexpr int Dimensions = _Dimensions;

		MemFunction(int _size, float _freq = 1.f):
			m_size(_size),
			m_frequency(_freq)
		{
			m_values.resize((int)pow(_size, _Dimensions));
		}

		// value of this function is acquired by interpolation.
		float operator()(ArgVec<float, _Dimensions> _arg)
		{
			for (int i = 0; i < Dimensions; ++i)
				_arg[i] *= m_frequency;
			std::array<KeyType, 1 << Dimensions> edgePoints;
			ArgVec<_ValueT, 1 << Dimensions> values;
			ArgVec<float, Dimensions> distances;

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
			}
			for(int i = 0; i < Dimensions; ++i)
				distances[i] = _arg[i] - edgePoints[0][i];

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
		_ValueT getStored(KeyType _arg)
		{
			return m_values[getIndex(_arg)];
		}

		// Assigns all discrete values using _init.
		void setAllStored(std::function<_ValueT(KeyType)> _init)
		{
			for (int i = 0; i < (int)m_values.size(); ++i)
				m_values[i] = _init(i);
		}
//	private:
		std::vector <_ValueT> m_values;
		int m_size; //size in any direction
		float m_frequency;
	};

	/* ValueNoise ********************************************************** //
	 * A memory based function with one dimensional random values.
	 */
	template< int _Dimensions, typename _Int >
	class ValueNoise : public MemFunction<_Dimensions, float, _Int>
	{
	public:
		ValueNoise(int _size, float _min = -1.f, float _max = 1.f, uint32_t _seed = 0x5FF3AC21)
			: MemFunction(_size)
		{
			Util::Random rng(_seed);
			setAllStored([&](MemFunction<_Dimensions, float, _Int>::KeyType _arg)
			{
				return rng.uniform(_min, _max);
			});
		}
	};

	/* GradientNoise ******************************************************* //
	* A memory based function with n dimensional values in [-1,1] that can be interpreted
	* as gradients of the function.
	*/
	template< int _Dimensions, typename _Int >
	class GradientNoise : public MemFunction<_Dimensions, ArgVec<float, _Dimensions>, _Int>
	{
	public:
		GradientNoise(int _size, float _freq = 1.f, uint32_t _seed = 0xB8EE2133)
			: MemFunction(_size, _freq)
		{
			Util::Random rng(_seed);
			setAllStored([&](MemFunction<_Dimensions, ArgVec<float, _Dimensions>, _Int>::KeyType _arg)
			{
				return rng.vector();
			});
		}
	};




	// ********************************************************************* //
	// simpler interpolation interface for 1d functions

	template<typename _Int>
	class Interpolation1D : public _Int
	{
	public:
		float interpolate(ArgVec<float, 2> _values, ArgVec<float, 1> _distances)
		{
			return _Int::interpolate(_values[0], _values[1], _distances[0]);
		}
	};

	template<typename _Int>
	using NoiseInt1D = FuncOp<ValueNoise<1, Interpolation1D<_Int>>>;

	// ********************************************************************* //
	// distance fields

	template<int _D, int _NumPoints, int _Min, int _Max>
	class PointField
	{
	public:
		static constexpr int Dimensions = _D;
		PointField()
		{
			Util::Random rng(0x529ef12c);
			
			int sqrtNum = (int)sqrt((float)_NumPoints);
			float freq = (_Max - _Min) / (float)sqrtNum;

			//generate some points
			for(int ix = 0; ix < sqrtNum; ++ix)
				for (int iy = 0; iy < sqrtNum; ++iy)
				{
					int ind = iy + sqrtNum * ix;
					m_points[ind][0] = (float)_Min + freq * rng.uniform((float)ix - 0.3f, (float)ix + 0.3f);
					m_points[ind][1] = (float)_Min + freq * rng.uniform((float)iy - 0.3f, (float)iy + 0.3f);
					m_heights[ind] = rng.uniform(1.f, 10.f);
				}
	/*		float frame = sqrt((float)_NumPoints) / ((_Max - _Min)(_Max - _Min));
			for (int i = 0; i < _NumPoints; ++i)
			{
				for (int j = 0; j < _D; ++j)
					m_points[i][j] = rng.uniform((float)_Min, (float)_Max);
			}*/
		}

	/*	float operator()(ArgVec<float, _D> _arg)
		{
			float minDist = 999999.f;

			for (auto& p : m_points)
			{
				float d = p.distance(_arg);
				if (d < minDist) minDist = d;
			}

			return minDist;
		}*/
	protected:
		std::array< ArgVec<float, _D>, _NumPoints> m_points;
		std::array< float, _NumPoints> m_heights;
	};


	// ********************************************************************* //
	struct Line
	{
		int begin;
		int end;

		float length;

		// is only used for the priority queue
		bool operator<(const Line& _rhs) const
		{
			return length > _rhs.length;
		}
	};

	inline float distance(glm::vec2 p, glm::vec2 v, glm::vec2 w, float len)
	{
		// minimum distance between line segment vw and point p
		float l2 = len * len;  // i.e. |w-v|^2 -  avoid a sqrt
								  // Consider the line extending the segment, parameterized as v + t (w - v).
								  // We find projection of point p onto the line. 
								  // It falls where t = [(p-v) . (w-v)] / |w-v|^2
								  // We clamp t from [0,1] to handle points outside the segment vw.
		float t = std::max(0.f, std::min(1.f, dot(p - v, w - v) / l2));
		glm::vec2 projection = v + t * (w - v);  // Projection falls on the segment

		return glm::distance(projection, p);//((projection.y - p.y)*(projection.y - p.y) + (projection.x - p.x)*(projection.x - p.x));
	}

	inline float sqr(float x) { return x * x; }

	// ********************************************************************* //
	template<int _NumPoints = 16>
	class MSTDistanceFunction : public PointField<2, _NumPoints, 20, 80>
	{
	public:
		MSTDistanceFunction():
			m_marks({true, false})
		{
			std::priority_queue<Line> queue;
			
			//add all connections of the first point
			for (int i = 1; i < _NumPoints; ++i) {
				Line l;
				l.begin = 0;
				l.end = i;
				l.length = m_points[l.begin].distance(m_points[l.end]);
				queue.push(l);
			}

			while (queue.size())
			{
				//take first element
				const Line& l = queue.top();
				
				if (!m_marks[l.end]) //point has not been visited
				{
					//add to the tree
					m_lines.push_back(l);
					const Line& lnew = m_lines.back(); // l is invalidated by pushes to the queue
					//mark point
					m_marks[l.end] = true;

					for (int i = 0; i < _NumPoints; ++i)
					{
						if (!m_marks[i])
						{
							Line next;
							next.begin = lnew.end;
							next.end = i;
							next.length = m_points[next.begin].distance(m_points[next.end]);
							queue.push(next);
						}
					}
				}
				queue.pop();
			}
		}

		float operator()(ArgVec<float, 2> _arg)
		{
			float minDist = 999999.f;
			for (auto& l : m_lines)
			{
				float d = distance(_arg, m_points[l.begin], m_points[l.end], l.length);
				if (d < minDist) minDist = d;
			}

			return 15.f / (minDist * 0.15f+1.f);//std::max(10.f - minDist, 0.f);
		}
/*		float operator()(ArgVec<float, 2> _arg)
		{
			float height = 0;
			float weightSum = 0;
			for (int i = 0; i < (int)m_points.size(); ++i)
			{
				auto& p = m_points[i];
				float distance = 2.0f/(1.0f + 2.0f*(sqr(p.y-_arg.y) + sqr(p.x-_arg.x)));	// Inverse quadric RBF
			//	float distance = exp(-0.0006f*(sqr(p.y - _arg.y) + sqr(p.x - _arg.x)));		// Gaussian
				height += distance * m_heights[i];
				weightSum += distance;
			}
			return height / weightSum;
		}*/

	protected:
		std::vector< Line > m_lines;
		std::array< bool, _NumPoints > m_marks;
	};

	typedef FuncOp<MSTDistanceFunction<>> DistanceFunction2D;

}