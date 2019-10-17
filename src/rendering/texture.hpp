#pragma once

#include <string>

namespace Graphic {
	class Texture
	{
	public:
		Texture(const std::string& _path)
		{
			load(_path);
		}

		int load(const std::string& _path);

		int getId() const { return m_id; }
	private:
		int m_id;
	};
}