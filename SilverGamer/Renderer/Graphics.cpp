#include "Graphics.h"
#include "Scene.h"
#include "Shader.h"
#include "Timer.h"
#include "Material.h"

Renderer::SGGraphics::SGGraphics()
{
}

Renderer::SGGraphics::~SGGraphics()
{
}

void Renderer::SGGraphics::Init()
{
    //初始化GLFW环境
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

    //检查当前支持的扩展
    GetSupportExtensions();
    //检查当前是否支持shader include 
    if (!CheckExtension("GL_ARB_shading_language_include"))
    {
        std::cout << "GPU doesn't support GL_ARB_shading_language_include" << std::endl;
    }

    //加载当前控制器
    m_controller = new SGController(m_window); 
   
	//加载场景
	m_scene = new SGScene();
	//SGModelBase* modelSphere = new SGModelBase("../Resource/Model/sphere.obj");
	SGModelBase* modelSphere = new SGModelBase("../Resource/Model/space-ship/Intergalactic_Spaceship-(Wavefront).obj");
	//SGModelBase* modelSphere = new SGModelBase("../Resource/Model/crytek-sponza/sponza.obj");
	m_scene->AddModel(modelSphere);

	SGPointLight* pointLight = new SGPointLight(glm::vec3(1, 1, 1));
	m_scene->AddPointLight(pointLight);
    SGDirLight* dirLight = new SGDirLight();
    m_scene->SetDirLight(dirLight);
	m_scene->Init(); //初始化
	//初始化对应ShaderInstance
	m_shaderInstance = SGShaderFactory::Instance()->loadNormalShader()->GetShaderProgramId();
    
}


void Renderer::SGGraphics::GetSupportExtensions()
{
    if (!m_supportExtensions.empty())
        return;

    GLint n = 0, i = 0;
    glGetIntegerv(GL_NUM_EXTENSIONS, &n);
    for (i = 0; i < n; i++)
    {
        std::string extension = (char*)glGetStringi(GL_EXTENSIONS, i);
        //CEPHEI_LOGDEBUGF("%s ", extension.c_str());
        m_supportExtensions.push_back(extension);
    }
}

bool Renderer::SGGraphics::CheckExtension(const std::string& extensionName)
{
    for (int i = 0; i < m_supportExtensions.size(); i++)
    {
        if (m_supportExtensions[i] == extensionName)
            return true;
    }

    return false;
}

void Renderer::SGGraphics::Render()
{
    glUseProgram(m_shaderInstance);
    m_scene->UploadStaticLight(m_shaderInstance); //上传静态灯光
    SGMaterialPhongFlat testMat;
    testMat.Upload(m_shaderInstance);

    SGTimer frameTimer;
    while (!glfwWindowShouldClose(m_window))
    {
        frameTimer.Start();

        // render
        // ------
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(m_shaderInstance);
        m_controller->LoadToShader(m_shaderInstance);

        m_scene->Render(m_shaderInstance); //进行绘制


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(m_window);
        glfwPollEvents();

        frameTimer.Stop();
        std::cout << "Frame:" << frameTimer.GetFrameNum() << std::endl;
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
}
