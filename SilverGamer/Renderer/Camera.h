#ifndef SG_CAMERA
#define SG_CAMERA

#include "RenderSetting.h"

namespace Renderer
{
	//��ʼCamera
	class SGCameraBase
	{
	public:
		SGCameraBase();
		virtual void InitializeCamera() = 0; //��ʼ��������麯��
		virtual glm::mat4 GetViewMatrix() = 0;
		virtual glm::mat4 GetViewMatrixForSkybox() = 0;
		virtual void RotateCamera(float xoffset, float yoffset) = 0;
		virtual ~SGCameraBase() {}
		virtual void SetTarget(glm::vec3) {}
		virtual glm::vec3 GetPosition() = 0;
		virtual glm::mat4 GetProjMatrix();
		virtual void LoadToShader(GLuint program); //���뵽Shader��

	public:
		float SetFov(float fov) { m_cameraFov = fov; }
		float SetAspect(float aspect) { m_cameraAspect = aspect; }
		float SetNearZ(float nearZ) { m_cameraNearZ = nearZ; }
		float SetFarZ(float farZ) { m_cameraFarZ = farZ; }
		float GetFov() { return m_cameraFov; }
		float GetAspect() { return m_cameraAspect; }
		float GetNearZ() { return m_cameraNearZ; }
		float GetFarZ() { return m_cameraFarZ; }

	private:
		float m_cameraFov; //������ĽǶ�
		float m_cameraAspect; //��������ݺ��
		float m_cameraNearZ;
		float m_cameraFarZ;
	};

	//�����˳������
	class SGCameraThirdRole : public SGCameraBase
	{
	public:
		SGCameraThirdRole();
		virtual void InitializeCamera();
		void RotateCamera(float xoffset, float yoffset);
		glm::vec3 GetPosition();
		glm::mat4 GetViewMatrix();
		glm::mat4 GetViewMatrixForSkybox();
		glm::vec3 GetForward();
		virtual void LoadToShader(GLuint program);

		void SetTarget(glm::vec3 tar) { m_targetPos = tar; }
		glm::vec3 GetTargetPosition() { return m_targetPos; }
		glm::vec3 GetDirUp() { return m_dirUp; }
		float GetSensity() { return m_sensity; }
		float GetCameraYaw() { return m_yaw; }
		float GetCameraPitch() { return m_pitch; }
		float GetCameraMaxDistance() { return m_maxDistance; }
		float GetCameraMinDistance() { return m_minDistance; }
		void AddDistance(float dt);
		void DecDistance(float dt);

	private:
		glm::vec3 m_targetPos;
		glm::vec3 m_dirUp;
		float m_sensity;
		float m_distance;
		float m_yaw;
		float m_pitch; 
		float m_maxDistance; //������
		float m_minDistance; //��С����
	};

	class SGCameraFirstRole : public SGCameraBase
	{
	public:
		SGCameraFirstRole();
		virtual void InitializeCamera();
		void SetYawAndPitch(float yaw, float pitch);
		void RotateCamera(float xoffset, float yoffset);
		void UpdateForward();
		glm::mat4 GetViewMatrix();
		glm::mat4 GetViewMatrixForSkybox();
		virtual void LoadToShader(GLuint program);

		void Move(float xOffset, float yOffset);
		void SetTarget(glm::vec3 pos) { m_position = pos; }
		glm::vec3 GetPosition() { return m_position; }
		glm::vec3 GetForward() { return m_forward; }

	private:
		float m_pitch;
		float m_yaw;
		float m_rotateSpeed;
		float m_moveSpeed;
		glm::vec3 m_position;
		glm::vec3 m_forward;
		glm::vec3 m_up;
	};

}

#endif
