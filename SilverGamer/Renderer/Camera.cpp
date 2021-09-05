#include "Camera.h"

Renderer::SGCamera::SGCamera(glm::vec3 pos,
	glm::vec3 target,
	glm::vec3 up)
{
	m_pos = pos;
	m_dir = glm::normalize(target - pos);
	m_up = up;
}

void Renderer::SGCamera::Upload(GLuint program)
{
	const int w = glutGet(GLUT_WINDOW_WIDTH);
	const int  h = glutGet(GLUT_WINDOW_HEIGHT);

	glm::mat4 V = getViewMatrix();
	glm::mat4 P = getPerspectiveMatrix(w, h);

	int P_loc = glGetUniformLocation(program, "P");
	if(P_loc != -1){
		glUniformMatrix4fv(P_loc, 1, false, glm::value_ptr(P));
	}
	int V_loc = glGetUniformLocation(program, "V");
	if (V_loc != -1) {
		glUniformMatrix4fv(V_loc, 1, false, glm::value_ptr(V));
	}
	glUniform3fv(glGetUniformLocation(program, "camPos"), 1, &m_pos[0]);
}

glm::mat4 Renderer::SGCamera::getPerspectiveMatrix(int w, int h)
{
	float aspect_ratio = float(w) / float(h);
	return glm::perspective(MATH_PI / 4.0f, aspect_ratio, 0.1f, 100.f);
}

glm::mat4 Renderer::SGCamera::getViewMatrix()
{
	return glm::lookAt(m_pos, GetTarget(), m_up);
}
