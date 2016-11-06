#include "camera.hpp"
#include <gtc/matrix_transform.hpp>

using namespace glm;

#define MODE2D

namespace Graphic{
	Camera::Camera(glm::vec3 _position, glm::vec3 _origin, float _fov)
		: m_viewMatrix(glm::lookAt(_position, _origin, vec3(0.f, 1.f, 0.f))),
		m_projectionMatrix(glm::perspective(glm::radians(90.0f), 16.f / 9.f, 0.1f, 100.0f)),
		m_rotateX(0.f),
		m_rotateY(0.f),
		m_position(_position),
		m_initialPosition(_position),
		m_initialOrigin(_origin)
	{
		UpdateMatrix();
	}

	void Camera::update(float _dx, float _dy, vec2 _off)
	{
#ifndef MODE2D
		m_rotateX += _dx;
		m_rotateY += _dy;
#endif

		glm::vec3 m_direction(
			cos(m_rotateY) * sin(m_rotateX),
			sin(m_rotateY),
			cos(m_rotateY) * cos(m_rotateX)
			);

		glm::vec3 right = glm::vec3(
			sin(m_rotateX - 3.14f / 2.0f),
			0,
			cos(m_rotateX - 3.14f / 2.0f)
			);

		glm::vec3 up = glm::cross(right, m_direction);

		m_position += m_direction * _off.x + right * _off.y;
		m_viewMatrix = glm::lookAt(m_position, m_position + m_direction, up);

		UpdateMatrix();
	}


	void Camera::reset()
	{
		m_position = m_initialPosition;
		m_direction = m_initialOrigin - m_initialPosition;
	}

	void Camera::UpdateMatrix()
	{
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}
}