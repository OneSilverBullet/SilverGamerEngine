#ifndef SG_INPUT_MANAGER
#define SG_INPUT_MANAGER

#include "RenderSetting.h"

#define KEY_STATUS std::array<bool,1024>
#define MOUSE_BUTTON_STATUS std::array<bool,3>
#define MOUSE_LEFT_BUTTON 0
#define MOUSE_MID_BUTTON 1
#define MOUSE_RIGHT_BUTTON 2
#define CURSOR_STATUS std::array<double,2>
#define CURSOR_POSITION_X 0
#define CURSOR_POSITION_Y 1
#define CURSOR_OFFSET_STATUS std::array<double,2>
#define CURSOR_OFFSET_X 0
#define CURSOR_OFFSET_Y 1
#define SCROLL_OFFSET_STATUS std::array<double,2>
#define SCROLL_X 0
#define SCROLL_Y 1


namespace Renderer
{

	//用于构建输入回调的基类
	class SGInputManager :  public SGSingleton<SGInputManager>
	{
	public:
		void	Init(GLFWwindow* window);
		int RegistKeyCallbackFunc(std::function<void(int, int, int, int)> vkeyCallback);
		int RegistCursorCallback(std::function<void(double, double)> vcursorCallback);
		int RegistMouseButtonCallback(std::function<void(int, int, int)> vmouseCallback);
		int RegistScrollCallback(std::function<void(double, double)> vscrollCallback);

		void UnregistKeyCallbackFunc(std::function<void(int, int, int, int)> vkeyCallback);
		void UnregistCursorCallback(std::function<void(double, double)> vcursorCallback);
		void UnregistMouseButtonCallback(std::function<void(int, int, int)> vmouseCallback);
		void UnregistScrollCallback(std::function<void(double, double)> vscrollCallback);
		
		void UnregistKeyCallbackFunc(int vEraseIndex);
		void UnregistCursorCallback(int vEraseIndex);
		void UnregistMouseButtonCallback(int vEraseIndex);
		void UnregistScrollCallback(int vEraseIndex);

		bool GetCursorStatus() { return m_cursorStatus; }
		const KEY_STATUS& GetKeyStatus() const;
		const MOUSE_BUTTON_STATUS& GetMouseButtonStatus() const;
		const CURSOR_STATUS& GetCursorPos() const;
		const CURSOR_STATUS& GetCursorOffset() const;
		const SCROLL_OFFSET_STATUS& GetScrollJourney() const;

	private:
		static int m_cursorStatus;
		static KEY_STATUS m_keyStatus;
		static MOUSE_BUTTON_STATUS m_mouseButtonStatus;
		static CURSOR_STATUS m_cursorPos;
		static CURSOR_STATUS m_lastCursorPos;
		static CURSOR_OFFSET_STATUS m_cursorOffset;
		static SCROLL_OFFSET_STATUS m_scrollJourney;

		static std::vector<std::function<void(int, int, int)>> m_mouseButtonCallBackFuntions;
		static std::vector<std::function<void(double, double)>> m_cursorCallBackFunctions;
		static std::vector<std::function<void(double, double)>> m_scrollCallBackFunctions;
		static std::vector<std::function<void(int, int, int, int)>> m_keyCallbackFunctions;

		SGInputManager();
		void RegistCallbackFunc(GLFWwindow* window);
		static void KeyCallbackFunc(GLFWwindow* window, GLint vKey, GLint vScancode, GLint vAction, GLint vMode);
		static void CursorCallbackFunc(GLFWwindow* window, GLdouble vPosX, GLdouble vPosY);
		static void ScrollCallbackFunc(GLFWwindow* window, GLdouble vOffsetX, GLdouble vOffsetY);
		static void MouseButtonCallbackFunc(GLFWwindow* window, GLint vButton, GLint vAction, GLint vModes);
	};


}

#endif