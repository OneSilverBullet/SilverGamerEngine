#include "InputManager.h"

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



void Renderer::SGInputManager::Init()
{
}

int Renderer::SGInputManager::RegistKeyCallbackFunc(std::function<void(int, int, int, int)> vkeyCallback)
{
	return 0;
}

int Renderer::SGInputManager::RegistCursorCallback(std::function<void(double, double)> vcursorCallback)
{
	return 0;
}

int Renderer::SGInputManager::RegistMouseButtonCallback(std::function<void(int, int, int)> vmouseCallback)
{
	return 0;
}

int Renderer::SGInputManager::RegistScrollCallback(std::function<void(double, double)> vscrollCallback)
{
	return 0;
}

int Renderer::SGInputManager::UnregistKeyCallbackFunc(std::function<void(int, int, int, int)> vkeyCallback)
{
	return 0;
}

int Renderer::SGInputManager::UnregistCursorCallback(std::function<void(double, double)> vcursorCallback)
{
	return 0;
}

int Renderer::SGInputManager::UnregistMouseButtonCallback(std::function<void(int, int, int)> vmouseCallback)
{
	return 0;
}

int Renderer::SGInputManager::UnregistScrollCallback(std::function<void(double, double)> vscrollCallback)
{
	return 0;
}

int Renderer::SGInputManager::UnregistKeyCallbackFunc(int vEraseIndex)
{
	return 0;
}

int Renderer::SGInputManager::UnregistCursorCallback(int vEraseIndex)
{
	return 0;
}

int Renderer::SGInputManager::UnregistMouseButtonCallback(int vEraseIndex)
{
	return 0;
}

int Renderer::SGInputManager::UnregistScrollCallback(int vEraseIndex)
{
	return 0;
}
