#pragma once

#include <GL/GL.h>


namespace Graphic{

	class Effect
	{
	public:
		//@param path + name without file ending of the fragment and vertex shader
		Effect(const char* _name);
		~Effect();

		GLuint getProgId() { return m_id; }
	private:
		GLuint m_id;
	};

}