#pragma once

#include <gl/GL.h>


namespace Graphic{

	class Effect
	{
	public:
		//@param path + name without file ending of the fragment and vertex shader
		Effect(const char* _name);

		GLuint getProgId() { return m_id; }
	private:
		GLuint m_id;
	};

}