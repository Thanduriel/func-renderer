#include "random.hpp"
#include <assert.h>

namespace Util{
	Random::Random(uint32_t _seed)
	{
		m_state[0] = 0xaffeaffe ^ _seed;
		m_state[1] = 0xf9b2a750 ^ (_seed * 0x804c8a24 + 0x68f699be);
		m_state[2] = 0x485eac66 ^ (_seed * 0x0fe56638 + 0xc917c8ce);
		m_state[3] = 0xcbd02714 ^ (_seed * 0x57571dae + 0xce2b3bd1);
		// Warmup
		Xorshift128();
		Xorshift128();
		Xorshift128();
		Xorshift128();
	}

	// ********************************************************************* //
	float Random::uniform(float _min, float _max)
	{
		double scale = (_max - _min) / 0xffffffff;
		return float(_min + scale * Xorshift128());
	}

	// ********************************************************************* //
	int32_t Random::uniform(int32_t _min, int32_t _max)
	{
		uint32_t interval = uint32_t(_max - _min + 1);
		// Do not use integer maximum bounds!
		assert(interval != 0, "Do not use integer maximum bounds!");

		uint32_t value = Xorshift128();
		return _min + value % interval;
	}

	// ********************************************************************* //
	uint32_t Random::Xorshift128()
	{
		uint32_t tmp = m_state[0] ^ (m_state[0] << 11);
		m_state[0] = m_state[1];
		m_state[1] = m_state[2];
		m_state[2] = m_state[3];
		m_state[3] ^= (m_state[3] >> 19) ^ tmp ^ (tmp >> 8);

		return m_state[3];
	}


}