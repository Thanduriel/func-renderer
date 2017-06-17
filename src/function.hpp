#pragma once

#include <vector>
#include <functional>
#include <array>
#include <queue>
#include <algorithm>

#include "glm.hpp"
#include "random.hpp"
#include "argvec.hpp"
#include "config.hpp"

namespace Math{
	
	// common used function signatures
	typedef std::function<float(float)> Function1D;
	typedef std::function<float(ArgVec<float, 2>)> Function2D;


	//all used math functions should inherit from this type to make them identifiable in compile time
	class Function
	{};

	// f: R^DimIn -> R^DimOut
	template<int _DimIn, int _DimOout>
	class FunctionStructure
	{
	public:
		static int constexpr DimIn = _DimIn;
		static int constexpr DimOut = _DimOout;
	};

	// individual operations
	template<typename _T, int _Dim>
	class FunctionalScalar : public FunctionStructure<_Dim,_Dim>
	{
	public:
		FunctionalScalar(const _T& _func, float _scalar)
			: m_func(_func), m_scalar(_scalar)
		{}

	protected:
		const _T& m_func;
		float m_scalar;
	};

	template<typename _T, int _Dim>
	class FunctionalScalarAdd : public FunctionalScalar<_T, _Dim>
	{
	public:
		using FunctionalScalar<_T, _Dim>::FunctionalScalar;

		float operator()(ArgVec<float, _Dim> _val) const
		{
			return this->m_scalar + this->m_func(_val);
		}
	};

	template<typename _T, int _Dim>
	class FunctionalScalarMul : public FunctionalScalar<_T, _Dim>
	{
	public:
		using FunctionalScalar<_T, _Dim>::FunctionalScalar;

		float operator()(ArgVec<float, _Dim> _val) const
		{
			return this->m_scalar * this->m_func(_val);
		}
	};

	// ********************************************************************* //
	template<typename _T1, typename _T2, int _Dim>
	class FunctionalBinOp : public FunctionStructure<_Dim,_Dim>
	{
		static_assert(_T1::DimIn == _T2::DimIn
			&& _T1::DimOut == _T2::DimOut, "Only functions with the same number of arguments can be combined.");
	public:
		FunctionalBinOp(const _T1& _func1, const _T2& _func2)
			: m_func1(_func1), m_func2(_func2)
		{
		}

	protected:
		const _T1& m_func1;
		const _T2& m_func2;

	};

	// ********************************************************************* //
	template<typename _T1, typename _T2, int _Dim>
	class FunctionalAdd : public FunctionalBinOp < _T1, _T2, _Dim>
	{
	public:
		using FunctionalBinOp < _T1, _T2, _Dim>::FunctionalBinOp;

		float operator()(ArgVec<float, _Dim> _val) const
		{
			return this->m_func1(_val) + this->m_func2(_val);
		}
	};

	// ********************************************************************* //
	template<typename _T1, typename _T2, int _Dim>
	class FunctionalSub : public FunctionalBinOp < _T1, _T2, _Dim>
	{
	public:
		using FunctionalBinOp < _T1, _T2, _Dim>::FunctionalBinOp;

		float operator()(ArgVec<float, _Dim> _val) const
		{
			return this->m_func1(_val) - this->m_func2(_val);
		}
	};

	// ********************************************************************* //
	template<typename _T1, typename _T2, int _Dim>
	class FunctionalMul : public FunctionalBinOp < _T1, _T2, _Dim>
	{
	public:
		using FunctionalBinOp < _T1, _T2, _Dim>::FunctionalBinOp;

		float operator()(ArgVec<float, _Dim> _val) const
		{
			return this->m_func1(_val) * this->m_func2(_val);
		}
	};

	// ********************************************************************* //
	template<typename _T1, typename _T2, int _Dim>
	class FunctionalDiv : public FunctionalBinOp < _T1, _T2, _Dim>
	{

	public:
		using FunctionalBinOp < _T1, _T2, _Dim>::FunctionalBinOp;

		float operator()(ArgVec<float, _Dim> _val) const
		{
			return this->m_func1(_val) / this->m_func2(_val);
		}
	};

	// ********************************************************************* //
	template<typename _T1, typename _T2, int _Dim>
	class FunctionalComposition : public FunctionStructure<_T2::DimIn, _T1::DimOut>
	{
		static_assert(_T2::DimOut == _T1::DimIn, 
			"The Output of the inner function must be of the same dimension as the input of the outer function.");
	public:

		FunctionalComposition(const _T1& _func1, const _T2& _func2)
			: m_func1(_func1), m_func2(_func2)
		{
		}

		float operator()(ArgVec<float, _T2::DimIn> _val) const
		{
			return m_func1(m_func2(_val));
		}

	private:
		const _T1& m_func1;
		const _T2& m_func2;
	};

	// ********************************************************************* //
	// This type provides the given _Super type algebraic operations.
	// The name of this class should be short to avoid warning C4503
	// "decorated name length exceeded, name was truncated"
	template< typename _Super >
	class FuncOp : public _Super
	{
	public:
		using _Super::_Super;

		//scalars
		auto operator+(float _val) const
		{
			return FuncOp<FunctionalScalarAdd<_Super, _Super::DimIn>>(*this, _val);
		}

		auto operator*(float _val) const
		{
			return FuncOp<FunctionalScalarMul<_Super, _Super::DimIn>>(*this, _val);
		}

		//binary operations
		template<typename _T2>
		auto operator+(const _T2& _oth) const
		{
			return FuncOp<FunctionalAdd<_Super, _T2, _Super::DimIn>>(*this, _oth);
		}

		template<typename _T2>
		auto operator-(const _T2& _oth) const
		{
			return FuncOp<FunctionalSub<_Super, _T2, _Super::DimIn>>(*this, _oth);
		}

		template<typename _T2>
		auto operator*(const _T2& _oth) const
		{
			return FuncOp<FunctionalMul<_Super, _T2, _Super::DimIn>>(*this, _oth);
		}

		template<typename _T2>
		auto operator/(const _T2& _oth) const
		{
			return FuncOp<FunctionalDiv<_Super, _T2, _Super::DimIn>>(*this, _oth);
		}

		//composition
		template<typename _T2>
		auto operator[](const _T2& _oth) const
		{
			return FuncOp<FunctionalComposition<_Super, _T2, _T2::DimIn>>(*this, _oth);
		}

	};

	//operators for lhs float
	template <typename _T, typename = typename std::enable_if< std::is_base_of<Function, _T>::value >::type>
	static auto operator+(float _val, const _T& _super)
	{
		return FuncOp<FunctionalScalarAdd<_T, _T::DimIn>>(_super, _val);
	}
	template <typename _T, typename = typename std::enable_if< std::is_base_of<Function, _T>::value >::type>
	static auto operator*(float _val, const _T& _super)
	{
		return FuncOp<FunctionalScalarMul<_T, _T::DimIn>>(_super, _val);
	}

	// ********************************************************** //
	// noise functions
	// ********************************************************** //

	/* MemFunction *********************************************
	 * Function that keeps discrete values in memory.
	 * @param _Dimensions Data dimensions this function has.
	 *	One corresponds to a function of the style f(x)=y.
	 * @param _Int A type that implements:
	 *	float interpolate(ArgVec<float, D>, ArgVec<float, D>)
	 */
	template< int _Dimensions, typename _ValueT, typename _Int>
	class MemFunction : public _Int, public Function, public FunctionStructure<_Dimensions, 1>
	{
		typedef FunctionStructure<_Dimensions, 1> Fn;
	public:
		//argument type used to access stored values
		typedef ArgVec<int, _Dimensions> KeyType;

		// When a _size of 0 is given an appropriate size is choosen
		// based on the frequency and the global word size.
		// If you plan to further distort the input coords use a larger size.
		MemFunction(int _size, float _freq = 1.f):
			m_size(_size ? _size : ((int)((c_worldSize) * _freq)+2)), // requires at least 2 values in every dir
			m_frequency(_freq)
		{
			m_values.resize((int)pow(m_size, _Dimensions));
		}

		// value of this function is acquired by interpolation.
		float operator()(ArgVec<float, _Dimensions> _arg) const
		{
			for (int i = 0; i < Fn::DimIn; ++i)
				_arg[i] *= m_frequency;
			std::array<KeyType, 1 << Fn::DimIn> edgePoints;
			ArgVec<_ValueT, 1 << Fn::DimIn> values;
			ArgVec<float, Fn::DimIn> distances;

			int border[Fn::DimIn * 2];
			for (int i = 0; i < Fn::DimIn; ++i)
			{
				int i2 = i * 2;
				border[i2] = (int)floor(_arg[i]);
				border[i2+1] = (int)ceil(_arg[i]);
			}
			for (int i = 0; i < 1 << Fn::DimIn; ++i)
			{
				for (int j = 0; j < Fn::DimIn; ++j)
					edgePoints[i][j] = border[2 * j + (1 & (i >> j))];
			//	float f = _arg[0] - lower;

				values[i] = getStored(edgePoints[i]);
			}
			for(int i = 0; i < Fn::DimIn; ++i)
				distances[i] = _arg[i] - edgePoints[0][i];

			return _Int::interpolate(values, distances);
		}

	protected:
		// Calculates the index at which the element can be found in m_values.
		int getIndex(KeyType _arg) const
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
		_ValueT getStored(KeyType _arg) const
		{
			return m_values[getIndex(_arg)];
		}

		// Assigns all discrete values using _init.
		void setAllStored(std::function<_ValueT(KeyType)> _init)
		{
			for (int i = 0; i < (int)m_values.size(); ++i)
			{
				KeyType key;
				int s = i;
				for (int j = 0; j < Fn::DimIn; ++j)
				{
					key[j] = s % m_size;
					s /= m_size;
				}

				m_values[i] = _init(key);
			}
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
		ValueNoise(int _size, float _freq = 1.f, float _min = -1.f, float _max = 1.f, uint32_t _seed = 0x5FF3AC21)
			: MemFunction<_Dimensions, float, _Int>(_size, _freq)
		{
			Util::Random rng(_seed);
			this->setAllStored([&](typename MemFunction<_Dimensions, float, _Int>::KeyType _arg)
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
		GradientNoise(int _size = 0, float _freq = 1.f, uint32_t _seed = 0xB8EE2133)
			: MemFunction<_Dimensions, ArgVec<float, _Dimensions>, _Int>(_size, _freq)
		{
			Util::Random rng(_seed);
			this->setAllStored([&](typename MemFunction<_Dimensions, ArgVec<float, _Dimensions>, _Int>::KeyType _arg)
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
		float interpolate(ArgVec<float, 2> _values, ArgVec<float, 1> _distances) const
		{
			return _Int::interpolate(_values[0], _values[1], _distances[0]);
		}
	};

	template<typename _Int>
	using NoiseInt1D = FuncOp<ValueNoise<1, Interpolation1D<_Int>>>;

	// ********************************************************************* //
	// distance fields
	// ********************************************************************* //

	/* PointField ********************************************************** // 
	 * A set of points _NumPoints evenly distributed over [_Min,_Max][_Min, _Max].
	 * To have them distributed in a full square use only quadratic numbers for _NumPoints.
	 */
	template<int _D, int _NumPoints, int _Min, int _Max>
	class PointField : public FunctionStructure<_D,_D>
	{
	public:
		// generates points with the given seed in a grid.
		// Every point variates from it's grid location by [-_MaxVariance,_MaxVariance].
		PointField(uint32_t _seed = 0x529ef12c, float _MaxVariance = 0.3f)
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

	protected:
		std::array< ArgVec<float, _D>, _NumPoints> m_points;
		std::array< float, _NumPoints> m_heights;
	};

	// ********************************************************************* //

	class WorleyNoise : public PointField< 2, 64, 0, 100>
	{
	public:
		void calculate(AVec2 _arg)
		{
			//already calculated
			if (m_lastPoint == _arg) return;

			float minDist = 999999.f;

			for (int i = 0; i < (int)m_distances.size(); ++i)
			{
				m_distances[i] = m_points[i].distance(_arg);
			}
			std::sort(m_distances.begin(), m_distances.end());
		}

		float operator[](int _x) const
		{
			return m_distances[_x];
		}

	private:
		AVec2 m_lastPoint; // point to wich the distances where calculated
		std::array<float, 64> m_distances;
	};

	struct WorleyNoiseRead : public Function, public FunctionStructure<2,1>
	{
	public:

		WorleyNoiseRead(WorleyNoise& _noise, int _distNum)
			: m_distNum(_distNum),
			m_noise(_noise) {}

		float operator()(AVec2 _arg) const
		{
			m_noise.calculate(_arg);
			return m_noise[m_distNum];
		}
	private:
		int m_distNum;
		WorleyNoise& m_noise;
	};

	typedef FuncOp<WorleyNoiseRead> WorleyNoiseFunction;


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

	inline float pointHeight(glm::vec2 p, glm::vec2 v, glm::vec2 w, float len, float ph, float vh)
	{
		// minimum distance between line segment vw and point p
		float l2 = len * len;  
		float t = std::max(0.f, std::min(1.f, dot(p - v, w - v) / l2));
		glm::vec2 projection = v + t * (w - v);  // Projection falls on the segment

		return /*(1-t) * ph + t * vh +*/ glm::distance(projection, p);//((projection.y - p.y)*(projection.y - p.y) + (projection.x - p.x)*(projection.x - p.x));
	}

	inline float sqr(float x) { return x * x; }

	// ********************************************************************* //
	template<int _NumPoints = 25, int _Min = 0, int _Max = 100>
	class MSTDistanceFunction : public PointField<2, _NumPoints, _Min, _Max>
	{
		typedef PointField<2, _NumPoints, _Min, _Max> ST;
	public:
		// Creates a minimum spanning tree on generated data.
		MSTDistanceFunction():
			m_marks({true, false}) // first element is always marked as visited
		{
			std::priority_queue<Line> queue;
			
			//add all possible connections of the first point
			for (int i = 1; i < _NumPoints; ++i) {
				Line l;
				l.begin = 0;
				l.end = i;
				l.length = ST::m_points[l.begin].distance(ST::m_points[l.end]);
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
					queue.pop();
					const Line& lnew = m_lines.back(); // l is invalidated by pushes to the queue
					//mark point
					m_marks[lnew.end] = true;

					for (int i = 0; i < _NumPoints; ++i)
					{
						if (!m_marks[i])
						{
							Line next;
							next.begin = lnew.end;
							next.end = i;
							next.length = ST::m_points[next.begin].distance(ST::m_points[next.end]);
							queue.push(next);
						}
					}
				}
				else queue.pop();
			}
		}

		float operator()(ArgVec<float, 2> _arg) const
		{
			float minDist = 999999.f;
			for (auto& l : m_lines)
			{
				float d = pointHeight(_arg, ST::m_points[l.begin].operator glm::vec2(), ST::m_points[l.end].operator glm::vec2(), l.length, ST::m_heights[l.begin], ST::m_heights[l.end]);
				if (d < minDist) minDist = d;
			}

			//return 15.f / (minDist * 0.15f+1.f);
			return std::max(10.f - minDist, 0.f);
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