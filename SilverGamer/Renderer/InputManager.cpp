#include "InputManager.h"

int Renderer::SGInputManager::m_cursorStatus = 1;
KEY_STATUS Renderer::SGInputManager::m_keyStatus = { false };
MOUSE_BUTTON_STATUS Renderer::SGInputManager::m_mouseButtonStatus = { false };
CURSOR_STATUS Renderer::SGInputManager::m_cursorPos = { 0.0 };
CURSOR_STATUS Renderer::SGInputManager::m_lastCursorPos = { 0.0 };
CURSOR_OFFSET_STATUS Renderer::SGInputManager::m_cursorOffset = { 0.0 };
SCROLL_OFFSET_STATUS Renderer::SGInputManager::m_scrollJourney = { 0.0 };

std::vector<std::function<void(int, int, int)>> m_mouseButtonCallBackFuntions;
std::vector<std::function<void(double, double)>> m_cursorCallBackFunctions;
std::vector<std::function<void(double, double)>> m_scrollCallBackFunctions;
std::vector<std::function<void(int, int, int, int)>> m_keyCallbackFunctions;

void Renderer::SGInputManager::Init(GLFWwindow* window)
{
	RegistCallbackFunc(window);
}

int Renderer::SGInputManager::RegistKeyCallbackFunc(std::function<void(int, int, int, int)> vkeyCallback)
{
	m_keyCallbackFunctions.push_back(vkeyCallback);
	return static_cast<int>(m_keyCallbackFunctions.size()) - 1; //返回当前call back函数的索引
}

int Renderer::SGInputManager::RegistCursorCallback(std::function<void(double, double)> vcursorCallback)
{
	m_cursorCallBackFunctions.push_back(vcursorCallback);
	return static_cast<int>(m_cursorCallBackFunctions.size()) - 1; //返回当前call back函数的索引
}

int Renderer::SGInputManager::RegistMouseButtonCallback(std::function<void(int, int, int)> vmouseCallback)
{
	m_mouseButtonCallBackFuntions.push_back(vmouseCallback);
	return static_cast<int>(m_mouseButtonCallBackFuntions.size()) - 1; //返回当前call back函数的索引
}

int Renderer::SGInputManager::RegistScrollCallback(std::function<void(double, double)> vscrollCallback)
{
	m_scrollCallBackFunctions.push_back(vscrollCallback);
	return static_cast<int>(m_scrollCallBackFunctions.size()) - 1; //返回当前call back函数的索引
}

void Renderer::SGInputManager::UnregistKeyCallbackFunc(std::function<void(int, int, int, int)> vkeyCallback)
{
	
}

void Renderer::SGInputManager::UnregistCursorCallback(std::function<void(double, double)> vcursorCallback)
{

}

void Renderer::SGInputManager::UnregistMouseButtonCallback(std::function<void(int, int, int)> vmouseCallback)
{

}

void Renderer::SGInputManager::UnregistScrollCallback(std::function<void(double, double)> vscrollCallback)
{

}

void Renderer::SGInputManager::UnregistKeyCallbackFunc(int vEraseIndex)
{
	m_keyCallbackFunctions.erase(m_keyCallbackFunctions.begin() + vEraseIndex);
}

void Renderer::SGInputManager::UnregistCursorCallback(int vEraseIndex)
{
	m_cursorCallBackFunctions.erase(m_cursorCallBackFunctions.begin() + vEraseIndex);
}

void Renderer::SGInputManager::UnregistMouseButtonCallback(int vEraseIndex)
{
	m_mouseButtonCallBackFuntions.erase(m_mouseButtonCallBackFuntions.begin() + vEraseIndex);
}

void Renderer::SGInputManager::UnregistScrollCallback(int vEraseIndex)
{
	m_scrollCallBackFunctions.erase(m_scrollCallBackFunctions.begin() + vEraseIndex);
}

const KEY_STATUS& Renderer::SGInputManager::GetKeyStatus() const
{
	return m_keyStatus;
}

const MOUSE_BUTTON_STATUS& Renderer::SGInputManager::GetMouseButtonStatus() const
{
	return m_mouseButtonStatus;
}

const CURSOR_STATUS& Renderer::SGInputManager::GetCursorPos() const
{
	return m_cursorPos;
}

const CURSOR_STATUS& Renderer::SGInputManager::GetCursorOffset() const
{
	return m_cursorOffset;
}

const SCROLL_OFFSET_STATUS& Renderer::SGInputManager::GetScrollJourney() const
{
	return m_scrollJourney;
}

Renderer::SGInputManager::SGInputManager()
{
}

//According to window*, bind callback function
void Renderer::SGInputManager::RegistCallbackFunc(GLFWwindow* window)
{
	glfwSetKeyCallback(window, KeyCallbackFunc);
	glfwSetMouseButtonCallback(window, MouseButtonCallbackFunc);
	glfwSetCursorPosCallback(window, CursorCallbackFunc);
	glfwSetScrollCallback(window, ScrollCallbackFunc);
}

void Renderer::SGInputManager::KeyCallbackFunc(GLFWwindow* window, GLint vKey, GLint vScancode, GLint vAction, GLint vMode)
{
	//immediately check the close state
	if (vKey == GLFW_KEY_ESCAPE && vAction == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	m_keyStatus[vKey] = vAction;

	if (m_keyStatus[GLFW_KEY_LEFT_ALT] == GLFW_PRESS && m_keyStatus[GLFW_KEY_C] == GLFW_PRESS) {
		m_cursorStatus = !m_cursorStatus;
		glfwSetInputMode(window, GLFW_CURSOR, m_cursorStatus ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
		//cursor initialize flag is 2 firstly
		if (m_cursorStatus == 1)
			m_cursorStatus = 2;
	}

	//run the binding functions
	for (auto func : m_keyCallbackFunctions) {
		func(vKey, vScancode, vAction, vMode);
	}
}

void Renderer::SGInputManager::CursorCallbackFunc(GLFWwindow* window, GLdouble vPosX, GLdouble vPosY)
{
	//Log the cursor information
	m_cursorPos[0] = vPosX;
	m_cursorPos[1] = vPosY;
	if (m_cursorStatus == 2)
	{
		m_cursorOffset[0] = m_cursorOffset[1] = 0;
		m_cursorStatus = 1;
	}
	else
	{
		m_cursorOffset[0] = m_cursorPos[0] - m_lastCursorPos[0];
		m_cursorOffset[1] = m_lastCursorPos[1] - m_cursorPos[1];
	}
	m_lastCursorPos = m_cursorPos;
	for (auto func : m_cursorCallBackFunctions) {
		func(vPosX, vPosY);
	}
}

void Renderer::SGInputManager::ScrollCallbackFunc(GLFWwindow* window, GLdouble vOffsetX, GLdouble vOffsetY)
{
	m_scrollJourney[0] += vOffsetX;
	m_scrollJourney[1] += vOffsetY;
	for (auto func : m_scrollCallBackFunctions) {
		func(vOffsetX, vOffsetY);
	}
}

void Renderer::SGInputManager::MouseButtonCallbackFunc(GLFWwindow* window, GLint vButton, GLint vAction, GLint vModes)
{
	m_mouseButtonStatus[vButton] = vAction;
	for (auto func : m_mouseButtonCallBackFuntions) {
		func(vButton, vAction, vModes);
	}
}
