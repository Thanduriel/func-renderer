#include <cstdint>

namespace Util{
	class Random
	{
	public:
		Random(uint32_t _seed);

		/// \brief Create the next uniform distributed sample in the given
		///		range (inclusive both boundaries). Default is [0,1].
		float uniform(float _min = 0.0f, float _max = 1.0f);

		/// \brief Create the next uniform distributed sample in the given
		///		integer range (inclusive both boundaries).
		int32_t uniform(int32_t _min, int32_t _max);
	private:
		//actual generator
		uint32_t Xorshift128();
		uint32_t m_state[4];
	};
}