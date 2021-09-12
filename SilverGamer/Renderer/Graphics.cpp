#include "Graphics.h"
#include "Scene.h"
#include "Shader.h"
#include "Timer.h"


Renderer::SGGraphics::SGGraphics()
{
}

Renderer::SGGraphics::~SGGraphics()
{
}

void Renderer::SGGraphics::Init()
{
    //��ʼ��GLFW����
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    m_window = glfwCreateWindow(APPLICATION_WIDTH, APLLICATION_HEIGHT, "SilverGamer", NULL, NULL);
    glfwMakeContextCurrent(m_window);
    if (m_window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    /*
    
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
    glfwSetCursorPosCallback(m_window, mouse_callback);
    glfwSetScrollCallback(m_window, scroll_callback);
    */
    // tell GLFW to capture our mouse
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);


	//���س���
	m_scene = new SGScene();
	SGModelBase* modelSphere = new SGModelBase("./Resource/Model/sphere.obj");
	m_scene->AddModel(modelSphere);

	SGPointLight* pointLight = new SGPointLight(glm::vec3(2, 2, 2));
	m_scene->AddLight(pointLight);
	m_scene->Init(); //��ʼ��
	//��ʼ����ӦShaderInstance
	m_shaderInstance = SGShaderFactory::GetInstance()->loadNormalShader()->GetShaderProgramId();
}

void Renderer::SGGraphics::Render()
{
    while (!glfwWindowShouldClose(m_window))
    {
        SGTimer::GetInstance()->UpdateTimer(glfwGetTime()); //���µ�ǰʱ���߼�

        m_scene->Render(m_shaderInstance); //���л���

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
}
