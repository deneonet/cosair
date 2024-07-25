#pragma once

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

namespace Cosair {

	class OrthographicCamera {
	public:
		OrthographicCamera(float left = -1.6f, float right = 1.6f, float bottom = -0.9f, float top = 0.9f, float zNear = -1, float zFar = 1)
			: m_Left(left), m_Right(right), m_Bottom(bottom), m_Top(top), m_zNear(zNear), m_zFar(zFar),
			m_ProjMatrix(glm::ortho(left, right, bottom, top, zNear, zFar)), m_ViewMatrix(1.f) {
			m_ViewProjMatrix = m_ProjMatrix * m_ViewMatrix;
		}

		inline void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }
		inline float GetRotation() const { return m_Rotation; }

		inline void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }
		inline const glm::vec3& GetPosition() const { return m_Position; }

		inline void SetPositionX(float x) { m_Position.x = x; RecalculateViewMatrix(); }
		inline void SetPositionY(float y) { m_Position.y = y; RecalculateViewMatrix(); }
		inline void SetPositionZ(float z) { m_Position.z = z; RecalculateViewMatrix(); }
		inline float GetPositionX() const { return m_Position.x; }
		inline float GetPositionY() const { return m_Position.y; }
		inline float GetPositionZ() const { return m_Position.z; }

		inline void SetLeft(float left) { m_Left = left; RecalculateProjMatrix(); }
		inline void SetRight(float right) { m_Right = right; RecalculateProjMatrix(); }
		inline void SetBottom(float bottom) { m_Bottom = bottom; RecalculateProjMatrix(); }
		inline void SetTop(float top) { m_Top = top; RecalculateProjMatrix(); }

		inline float GetLeft() const { return m_Left; }
		inline float GetRight() const { return m_Right; }
		inline float GetBottom() const { return m_Bottom; }
		inline float GetTop() const { return m_Top; }

		inline void SetZFar(float zFar) { m_zFar = zFar; RecalculateProjMatrix(); }
		inline void SetZNear(float zNear) { m_zNear = zNear; RecalculateProjMatrix(); }

		inline float GetZFar() const { return m_zFar; }
		inline float GetZNear() const { return m_zNear; }

		inline const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		inline const glm::mat4& GetProjMatrix() const { return m_ProjMatrix; }
		inline const glm::mat4& GetViewProjMatrix() const { return m_ViewProjMatrix; }

	private:
		void RecalculateViewMatrix();
		void RecalculateProjMatrix();

	private:
		glm::mat4 m_ProjMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjMatrix; // Cache of m_ProjMatrix and m_ViewMatrix multiplication

		glm::vec3 m_Position = { 0, 0, 0 };
		float m_Rotation = 0.f; // Z Rotation, X and Y are unnecessary

		float m_Left;
		float m_Right;
		float m_Bottom;
		float m_Top;

		float m_zFar;
		float m_zNear;
	};

	/* Perspective Camera (commented out for now)
	class PerspectiveCamera {
	public:
		PerspectiveCamera(float fov = 30.f, float aspectRatio = 0, float zNear = 0.1f, float zFar = 100.f);

		inline void SetRotation(float rotation);
		inline void SetPosition(const glm::vec3& position);

		inline float GetRotation() { return m_Rotation; }
		inline const glm::vec3& GetPosition() { return m_Position; }

		inline void SetFov(float fov);
		inline void SetAspectRatio(float aspectRatio);

		inline float GetFov() { return m_Fov; }
		inline float GetAspectRatio() { return m_AspectRatio; }

		inline void SetZFar(float zFar);
		inline void SetZNear(float zNear);

		inline float GetZFar() { return m_zFar; }
		inline float GetZNear() { return m_zNear; }

		inline const glm::mat4& GetViewMatrix() { return m_ViewMatrix; }
		inline const glm::mat4& GetProjMatrix() { return m_ProjMatrix; }
		inline const glm::mat4& GetViewProjMatrix() { return m_ViewProjMatrix; }
	private:
		void RecalculateViewMatrix();
		void RecalculateProjMatrix();

	private:
		glm::mat4 m_ProjMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjMatrix; // Cache of m_ProjMatrix and m_ViewMatrix multiplication

		glm::vec3 m_Position;
		float m_Rotation; // X Rotation, Y and Z are unnecessary

		float m_Fov = 30.f;
		float m_zFar = 100.f;
		float m_zNear = 0.1f;
		float m_AspectRatio = 0.f;
	};
	*/

}