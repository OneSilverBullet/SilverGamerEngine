#include "Application.h"

Renderer::SGApplication & Renderer::SGApplication::GetInstance()
{
	static SGApplication app;
	return app;
}

void Renderer::SGApplication::Init(int argc, char ** argv)
{
    //初始化本地对象
    m_graphicPipline = new SGGraphics();
   // m_controller = new SGControllerFPS(m_graphicPipline->GetScene()->GetCamera(), APPLICATION_WIDTH, APLLICATION_HEIGHT);

    m_graphicPipline->Init(); //初始化当前渲染GLFW环境
}

void Renderer::SGApplication::Run()
{
    m_graphicPipline->Render(); //进行渲染
}

GLFWwindow* Renderer::SGApplication::GetGLFWWindow()
{
    RENDER_WARDER_NULL_ALERT(m_graphicPipline);
    return m_graphicPipline->GetGLFWWindow();
}


