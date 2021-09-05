#include "Controller.h"

Renderer::SGControllerFPS::SGControllerFPS(SGCamera * cam, int w, int h)
{
	m_camera = cam;
	m_width = w;
	m_height = h;
	m_mouseX = m_width / 2;
	m_mouseY = m_height / 2;
	Init();
}

Renderer::SGControllerFPS::~SGControllerFPS()
{
}

void Renderer::SGControllerFPS::Move()
{
}

void Renderer::SGControllerFPS::Init()
{
	SetCursorPos(m_mouseX, m_mouseY);
	glutSetCursor(GLUT_CURSOR_NONE);
}

void Renderer::SGControllerFPS::Update()
{
	if (!m_useCursor) {
		glutSetCursor(GLUT_CURSOR_NONE);
	}
	Move();
}

void Renderer::SGControllerFPS::OnMouseMove(int x, int y)
{
	glm::vec3 oldPosition = m_camera->GetPosition();
	glm::vec3 newPosition = m_horizontal * Right() + m_vertical * Forward() + m_gravity * Up();
	m_camera->SetPosition(newPosition * m_step);
}

void Renderer::SGControllerFPS::OnKeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		m_vertical = 1.0f;
		break;
	case 's':
		m_vertical = -1.0f;
		break;
	case 'a':
		m_horizontal = -1.0f;
		break;
	case 'd':
		m_horizontal = 1.0f;
		break;
	case 'q':
		m_gravity = -1.0f;
		break;
	case 'e':
		m_gravity = 1.0f;
		break;
	case 'm':
		m_useCursor = !m_useCursor;
		SetCursorPos(m_mouseX, m_mouseY);
		break;
	default:
		break;
	}
}

void Renderer::SGControllerFPS::OnKeyboardUp(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		m_vertical = 0.0f;
		break;
	case 's':
		m_vertical = 0.0f;
		break;
	case 'a':
		m_horizontal = 0.0f;
		break;
	case 'd':
		m_horizontal = 0.0f;
		break;
	case 'q':
		m_gravity = 0.0f;
		break;
	case 'e':
		m_gravity = 0.0f;
		break;
	default:
		break;
	}
}
