#include "Camera.h"
#include "RenderSetting.h"

Renderer::SGCameraBase::SGCameraBase() : m_cameraFov(CAMERA_DEFAULT_FOV),
m_cameraNearZ(CAMREA_DEFAULT_NEAR_Z),
m_cameraFarZ(CAMERA_DEFAULT_FAR_Z) 
{
	const int w = APPLICATION_WIDTH;
	const int  h = APLLICATION_HEIGHT;
	m_cameraAspect = float(w) / float(h);
}

glm::mat4 Renderer::SGCameraBase::GetProjMatrix()
{
	glm::mat4 projMatrix = glm::perspective(m_cameraFov, m_cameraAspect, m_cameraNearZ, m_cameraFarZ);
	return projMatrix;
}

void Renderer::SGCameraBase::LoadToShader(GLuint program)
{
	glm::mat4 V = GetViewMatrix();
	glm::mat4 P = GetProjMatrix();

	int P_loc = glGetUniformLocation(program, "P");
	if (P_loc != -1) {
		glUniformMatrix4fv(P_loc, 1, false, glm::value_ptr(P));
	}
	int V_loc = glGetUniformLocation(program, "V");
	if (V_loc != -1) {
		glUniformMatrix4fv(V_loc, 1, false, glm::value_ptr(V));
	}
}


Renderer::SGCameraThirdRole::SGCameraThirdRole() : SGCameraBase()
{
	InitializeCamera(); //初始化其他参数
}

void Renderer::SGCameraThirdRole::InitializeCamera()
{
	m_targetPos = CAMERA_THIRD_ROLE_DEFAULT_TARGET;
	m_dirUp = CAMERA_THIRD_ROLE_DEFAULT_UPDIR;
	m_sensity = CAMERA_THIRD_ROLE_DEFAULT_SENSITY;
	m_distance = CAMERA_THIRD_ROLE_DEFAULT_DISTANCE;
	m_yaw = -90.0f;
	m_pitch = 0.0f;
	m_minDistance = CAMERA_THIRD_ROLE_DEFAULT_MIN_DISTANCE;
	m_maxDistance = CAMERA_THIRD_ROLE_DEFAULT_MAX_DISTANCE;
}

void Renderer::SGCameraThirdRole::RotateCamera(float xoffset, float yoffset)
{
	m_pitch += yoffset * m_sensity;
	m_yaw += xoffset * m_sensity;
	if (m_pitch > CAMERA_THIRD_ROLE_LIMIT_PICTH) m_pitch = CAMERA_THIRD_ROLE_LIMIT_PICTH;
	if (m_yaw < CAMERA_THIRD_ROLE_LIMIT_YAW) m_yaw = CAMERA_THIRD_ROLE_LIMIT_YAW;
}

glm::vec3 Renderer::SGCameraThirdRole::GetPosition()
{
	glm::vec3 cameraForward;
	cameraForward.x = cos(glm::radians(m_pitch)) * cos(glm::radians(m_yaw));
	cameraForward.y = sin(glm::radians(m_pitch));
	cameraForward.z = cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw));
	cameraForward = glm::normalize(cameraForward);
	glm::vec3 position = m_targetPos - (cameraForward * m_distance);
	return position;
}

glm::mat4 Renderer::SGCameraThirdRole::GetViewMatrix()
{
	glm::vec3 position = GetPosition();
	return glm::lookAtLH(position, m_targetPos, m_dirUp);
}

glm::mat4 Renderer::SGCameraThirdRole::GetViewMatrixForSkybox()
{
	glm::vec3 position = GetPosition();
	glm::mat4 viewMat = glm::lookAtLH(position, m_targetPos, m_dirUp);
	viewMat[3][0] = 0;
	viewMat[3][1] = 0;
	viewMat[3][2] = 0;
	viewMat[3][3] = 1;
	return viewMat; //返回针对天空盒的ViewMatrix，将位移部分取消掉
}

glm::vec3 Renderer::SGCameraThirdRole::GetForward()
{
	glm::vec3 position = GetPosition();
	glm::vec3 targetPosition = m_targetPos;
	glm::vec3 forward = glm::normalize(glm::vec3(targetPosition - position));
	return forward;
}

void Renderer::SGCameraThirdRole::LoadToShader(GLuint program)
{
	SGCameraBase::LoadToShader(program); //调用基类的加载函数
	glm::vec3 position = GetPosition();
	glUniform3fv(glGetUniformLocation(program, "camPos"), 1, &position[0]);
}

void Renderer::SGCameraThirdRole::AddDistance(float dt)
{
	m_distance += dt * m_sensity;
	if (m_distance > m_maxDistance)
		m_distance = m_maxDistance;
}
void Renderer::SGCameraThirdRole::DecDistance(float dt)
{
	m_distance -= dt * m_sensity;
	if (m_distance < m_minDistance)
		m_distance = m_minDistance;
}


Renderer::SGCameraFirstRole::SGCameraFirstRole() : SGCameraBase()
{
	InitializeCamera();
}

void Renderer::SGCameraFirstRole::InitializeCamera()
{
	glm::vec3 target = glm::vec3(0.0F, 0.0F, 0.0F);
	m_position = CAMERA_FIRST_ROLE_DEFAULT_POSITION;
	m_forward = glm::normalize(target - m_position);
	m_up = CAMERA_FIRST_ROLE_DEFAULT_UPDIR;
	m_rotateSpeed = CAMERA_FIRST_ROLE_DEFAULT_ROTATE_SPEED;
	m_pitch = 0.0f;
	m_yaw = 0.0f;
	UpdateForward();
}

void Renderer::SGCameraFirstRole::SetYawAndPitch(float yaw, float pitch)
{
	m_yaw = yaw;
	m_pitch = pitch;
	UpdateForward();
}

void Renderer::SGCameraFirstRole::RotateCamera(float xoffset, float yoffset)
{
	m_pitch += yoffset * m_rotateSpeed;
	m_yaw += xoffset * m_rotateSpeed;
	if (m_pitch > CAMERA_FIRST_ROLE_LIMIT_MAX_PITCH) m_pitch = CAMERA_FIRST_ROLE_LIMIT_MAX_PITCH;
	if (m_pitch < CAMERA_FIRST_ROLE_LIMIT_MIN_PITCH) m_pitch = CAMERA_FIRST_ROLE_LIMIT_MIN_PITCH;
	UpdateForward();
}

void Renderer::SGCameraFirstRole::UpdateForward()
{
	m_forward.x = cos(glm::radians(m_pitch)) * cos(glm::radians(m_yaw));
	m_forward.y = sin(glm::radians(m_pitch));
	m_forward.z = cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw));
}

glm::mat4 Renderer::SGCameraFirstRole::GetViewMatrix()
{
	glm::mat4 viewMatrix = glm::lookAtLH(m_position, m_forward, m_up);
	return viewMatrix;
}

glm::mat4 Renderer::SGCameraFirstRole::GetViewMatrixForSkybox()
{
	glm::mat4 viewMatrix = glm::lookAtLH(m_position, m_forward, m_up);
	viewMatrix[3][0] = 0.0f;
	viewMatrix[3][1] = 0.0f;
	viewMatrix[3][2] = 0.0f;
	viewMatrix[3][3] = 1.0f;
	return viewMatrix;
}

void Renderer::SGCameraFirstRole::LoadToShader(GLuint program)
{
	SGCameraBase::LoadToShader(program); //调用基类的加载函数
	glUniform3fv(glGetUniformLocation(program, "camPos"), 1, &m_position[0]);
}



