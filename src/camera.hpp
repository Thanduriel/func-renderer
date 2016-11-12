#pragma once

#define MODE2D

#include <glm.hpp>

namespace Graphic{

	class Camera
	{
	public:
		Camera(glm::vec3 _position, glm::vec3 _origin, float _fov);

		const glm::mat4& GetViewProjection() const { return m_viewProjectionMatrix; }

		void update(float _dx, float _dy, glm::vec2 _off);

		// Sets back this camera to the initial state.
		void reset();
	private:
		void UpdateMatrix();

		float m_rotateX, m_rotateY;
		glm::vec3 m_position, m_direction;
		glm::mat4 m_viewMatrix;
		glm::mat4 m_projectionMatrix;

		glm::mat4 m_viewProjectionMatrix;

		//inital state
		glm::vec3 m_initialPosition;
		glm::vec3 m_initialOrigin;
	};

}