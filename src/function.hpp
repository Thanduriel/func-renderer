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
	 * Inherit from this type to specify interpolation behavior.
	 */
	template< int _Dimensions>
	class MemFunction
	{
	public:

		const int Dimensions = _Dimensions;

		MemFunction(int _size, int _freq = 1)
		{
			m_values.resize(pow(_size, _Dimensions));
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
	class NoiseFunction : public MemFunction<1>
	{
	public:
		NoiseFunction(int _size, int _freq = 1)
			: MemFunction(_size, _freq)
		{
			Util::Random rng(0x1a23f);
			setAllStored([&](MemFunction<1>::KeyType _arg)
			{
				return rng.uniform();
			});
		}

		float operator()(float _arg)
		{
			return (getStored((int)floor(_arg)) + getStored((int)ceil(_arg))) / 2.f;
		}
	};
}