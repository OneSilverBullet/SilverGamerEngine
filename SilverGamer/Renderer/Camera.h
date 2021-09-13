#ifndef SG_CAMERA
#define SG_CAMERA

#include "RenderSetting.h"

namespace Renderer
{
	//初始Camera
	class SGCameraBase
	{
	public:
		SGCameraBase() : m_cameraFov(MATH_PI_DIV_4), m_cameraAspect(1.33), m_cameraNearZ(0.1f), m_cameraFarZ(100.0f) {}
		virtual glm::mat4 GetViewMatrix() = 0;
		virtual glm::mat4 GetViewMatrixForSkybox() = 0;
		virtual ~SGCameraBase() {}
		virtual void SetTarget(glm::vec3) {}
		virtual glm::vec3 GetPosition() = 0;
		virtual glm::mat4 GetProjMatrix();
		virtual void LoadToShader(GLuint program); //传入到Shader中

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
		float m_cameraFov; //摄像机的角度
		float m_cameraAspect; //摄像机的纵横比
		float m_cameraNearZ;
		float m_cameraFarZ;
	};

	//第三人称摄像机
	class SGCameraThirdRole : public SGCameraBase
	{
	public:
		SGCameraThirdRole() :SGCameraBase() {}

		void InitializeThirdRoleCamera();
		void RotateCamera(float xoffset, float yoffset);

		glm::vec3 GetPosition();

		glm::mat4 GetViewMatrix();
		glm::mat4 GetViewMatrixForSkybox();

		

		glm::vec3 GetForward();
		/* {
			glm::vec3 position = GetPosition();
			glm::vec3 targetPosition = m_targetPos;
			glm::vec3 forward = glm::normalize(glm::vec3(targetPosition - position));
			return forward;
		}*/
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
		/*
		{
			m_distance += dt;
			if (m_distance > 10.0f) thirdcamera_distance = 10.0f;
		}*/

		
		/*
		{
			thirdcamera_distance -= dt;
			if (thirdcamera_distance < 2.0f) thirdcamera_distance = 2.0f;
		}*/

	private:
		glm::vec3 m_targetPos;
		glm::vec3 m_dirUp;
		float m_sensity;
		float m_distance;
		float m_yaw;
		float m_pitch; 
		float m_maxDistance; //最大距离
		float m_minDistance; //最小距离
	};

	class FirstRoleCamera : public Camera
	{
	public:
		FirstRoleCamera() :Camera() {}
		void InitializeFirstRoleCamera();
		void SetYawAndPitch(float yaw, float pitch);
		void RotateCamera(float xoffset, float yoffset);
		void UpdateForward();
		XMMATRIX GetViewMatrix();
		XMMATRIX GetViewMatrixForSkybox();

	public:
		void SetTarget(XMFLOAT3 pos) { firstcamera_position = pos; }
		XMFLOAT3 GetPosition() { return firstcamera_position; }
		XMFLOAT3 GetForward() { return firstcamera_forward; }
	private:
		float firstcamera_pitch;
		float firstcamera_yaw;
		float firstcamera_rotateSpeed;
		XMFLOAT3 firstcamera_position;
		XMFLOAT3 firstcamera_forward;
		XMFLOAT3 firstcamera_up;
	};



	class SGCamera
	{
	public:
		SGCamera(glm::vec3 pos = glm::vec3(0, 0, 2), 
			glm::vec3 target = glm::vec3(0, 0, 0),
			glm::vec3 up = glm::vec3(0, 1, 0));
		void Upload(GLuint program);

		glm::vec3 GetTarget() { return m_pos + m_dir; }
		glm::mat4 getPerspectiveMatrix(int w, int h);
		glm::mat4 getViewMatrix();

		glm::vec3 GetForward() { return m_dir; }
		glm::vec3 GetUp() { return m_up;   }
		glm::vec3 GetPosition() { return m_pos; }

		void SetForward(glm::vec3 a) { m_dir = a; }
		void SetUp(glm::vec3 a) { m_up = a; }
		void SetPosition(glm::vec3 a) { m_pos = a; }

	private:
		glm::vec3 m_pos;
		glm::vec3 m_dir;
		glm::vec3 m_up;
	};
}

#endif
