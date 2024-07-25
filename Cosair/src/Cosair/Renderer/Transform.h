#pragma once

#include "Cosair/Core.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

namespace Cosair {

	class Transform {
	public:
		Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale = glm::vec3(1)) :
			m_Position(position), m_Rotation(rotation), m_Scale(scale) {
			CR_PROFILE_FUNCTION();

			if (HasRotation())
				RecalculateTransformMatrix();
		}

		Transform() : Transform(glm::vec3(0), glm::vec3(0), glm::vec3(1)) { }
		Transform(const glm::vec3& position) : Transform(position, glm::vec3(0), glm::vec3(1)) { }

		inline void SetScale(const glm::vec3& scale) {
			m_Scale = scale;
			RecalculateTransformMatrix();
		}

		inline void SetPosition(const glm::vec3& position) {
			m_Position = position;
			RecalculateTransformMatrix();
		}

		inline void SetRotation(const glm::vec3& rotation) {
			m_Rotation = rotation;
			RecalculateTransformMatrix();
		}

		inline bool HasRotation() const { return m_Rotation.x != 0 || m_Rotation.y != 0 || m_Rotation.z != 0; }
		inline bool HasPosition() const { return m_Position.x != 0 || m_Position.y != 0 || m_Position.z != 0; }
		inline bool HasScale() const { return m_Scale.x != 0 || m_Scale.y != 0 || m_Scale.z != 0; }

		inline const glm::vec3& GetScale() const { return m_Scale; }
		inline const glm::vec3& GetPosition() const { return m_Position; }
		inline const glm::vec3& GetRotation() const { return m_Rotation; }

		inline void SetScaleX(float x) { m_Scale.x = x; RecalculateTransformMatrix(); }
		inline void SetScaleY(float y) { m_Scale.y = y; RecalculateTransformMatrix(); }
		inline void SetScaleZ(float z) { m_Scale.z = z; RecalculateTransformMatrix(); }
		inline void SetPositionX(float x) { m_Position.x = x; RecalculateTransformMatrix(); }
		inline void SetPositionY(float y) { m_Position.y = y; RecalculateTransformMatrix(); }
		inline void SetPositionZ(float z) { m_Position.z = z; RecalculateTransformMatrix(); }
		inline void SetRotationX(float x) { m_Rotation.x = x; RecalculateTransformMatrix(); }
		inline void SetRotationY(float y) { m_Rotation.y = y; RecalculateTransformMatrix(); }
		inline void SetRotationZ(float z) { m_Rotation.z = z; RecalculateTransformMatrix(); }

		inline const float GetScaleX() const { return m_Scale.x; }
		inline const float GetScaleY() const { return m_Scale.y; }
		inline const float GetScaleZ() const { return m_Scale.z; }
		inline const float GetPositionX() const { return m_Position.x; }
		inline const float GetPositionY() const { return m_Position.y; }
		inline const float GetPositionZ() const { return m_Position.z; }
		inline const float GetRotationX() const { return m_Rotation.x; }
		inline const float GetRotationY() const { return m_Rotation.y; }
		inline const float GetRotationZ() const { return m_Rotation.z; }

		// The transform matrix is only updates, when the rotation changes
		inline const glm::mat4& GetTransformMatrix() const { return m_TransformMatrix; }
	private:
		inline void RecalculateTransformMatrix() {
			CR_PROFILE_FUNCTION();
			if (!HasRotation())
				return;

			m_TransformMatrix = glm::translate(glm::mat4(1), m_Position) * glm::scale(glm::mat4(1), m_Scale);
			if (m_Rotation.x != 0)
				m_TransformMatrix *= glm::rotate(glm::mat4(1), glm::radians(m_Rotation.x), glm::vec3(1, 0, 0));
			if (m_Rotation.y != 0)
				m_TransformMatrix *= glm::rotate(glm::mat4(1), glm::radians(m_Rotation.y), glm::vec3(0, 1, 0));
			if (m_Rotation.z != 0)
				m_TransformMatrix *= glm::rotate(glm::mat4(1), glm::radians(m_Rotation.z), glm::vec3(0, 0, 1));
		}

	private:
		glm::vec3 m_Scale;
		glm::vec3 m_Position;
		glm::vec3 m_Rotation;

		glm::mat4 m_TransformMatrix; // Cache calculation of the position, rotation and scale, only updated on rotation changes
	};

	using TransformRef = Ref<Transform>;
}
