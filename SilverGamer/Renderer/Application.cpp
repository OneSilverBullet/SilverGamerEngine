#include "Application.h"

Renderer::SGApplication & Renderer::SGApplication::GetInstance()
{
	static SGApplication app;
	return app;
}

void Renderer::SGApplication::Init(int argc, char ** argv)
{
    //��ʼ�����ض���
    m_graphicPipline = new SGGraphics();
   // m_controller = new SGControllerFPS(m_graphicPipline->GetScene()->GetCamera(), APPLICATION_WIDTH, APLLICATION_HEIGHT);

    m_graphicPipline->Init(); //��ʼ����ǰ��ȾGLFW����
}

void Renderer::SGApplication::Run()
{
    m_graphicPipline->Render(); //������Ⱦ
}

GLFWwindow* Renderer::SGApplication::GetGLFWWindow()
{
    RENDER_WARDER_NULL_ALERT(m_graphicPipline);
    return m_graphicPipline->GetGLFWWindow();
}


