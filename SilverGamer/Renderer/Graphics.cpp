#include "Graphics.h"
#include "Scene.h"
#include "Shader.h"
#include "Timer.h"
#include "Material.h"
#include "ResourceLoad.h"

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
    IEntity* newEntity = new IEntity("../Resource/Model/space-ship/Intergalactic_Spaceship-(Wavefront).obj");
    newEntity->SetEntityScale(glm::vec3(0.5f));
    //Create GBuffer Shader
    //SGGBufferMaterialPBRWithEmit* gBufferGenShader = new SGGBufferMaterialPBRWithEmit("../Resource/Model/space-ship/textures/");
    //newEntity->SetMaterial(gBufferGenShader); //Change GBuffer Shader of entity
    m_scene->AddEntity(newEntity);

	SGPointLight* pointLight = new SGPointLight(glm::vec3(3, 3, 3));
	m_scene->AddPointLight(pointLight);
    SGDirLight* dirLight = new SGDirLight();
    m_scene->SetDirLight(dirLight);
	m_scene->Init(); //初始化    

   
    //Initialize GBuffer
    m_GBuffer = new SGFrameBuffer(5);
    //Initialize Screen Quad
    m_defferedQuad = new DefferedQuad(m_GBuffer->GetFBOTextures());
    m_subQuad = new DefferedQuad(m_GBuffer->GetFBOTextures(), 0.4f, 1.0f, 0.46f, 1.0f);

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
    m_scene->UploadStaticLight(m_shaderInstance); //上传静态灯光

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    SGTimer frameTimer;
    while (!glfwWindowShouldClose(m_window))
    {
        frameTimer.Start();

        m_GBuffer->Enable(); //Active GBuffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_scene->Render(m_controller); //Render Virtual Scene
        m_GBuffer->Disable(); //Disable GBuffers
        //m_defferedQuad->ShowScreenTexture(); //Render Quad
        m_defferedQuad->DefferedRendering(m_controller, m_scene);
        m_subQuad->ShowScreenTexture(); //Show Sub Textures
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(m_window);
        glfwPollEvents();

        frameTimer.Stop();
        //std::cout << "Frame:" << frameTimer.GetFrameNum() << std::endl;
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
}


/*
* DefferedQuad
*/
//Attention: This initializer is for sub quad
Renderer::DefferedQuad::DefferedQuad(std::vector<SGTexture2D*> textureVec, float minX, float maxX, float minY, float maxY)
{
    m_quad = new SGQuad(minX, minY, maxX, maxY);
    m_screenTextures = textureVec;
    m_shader = SGShaderFactory::Instance()->LoadShader("shader_quad", "shader_quad");
    std::function<void(int, int, int, int)> rotateFunc = [=](int key, int scancode, int action, int mode)
    {
        this->ChangeRenderTextures(key, scancode, action, mode);
    };
    SGInputManager::Instance()->RegistKeyCallbackFunc(rotateFunc);
    m_useFullScreen = false;
}

//Attention: This initializer is for full screen quad. So there is no callback func
Renderer::DefferedQuad::DefferedQuad(std::vector<SGTexture2D*> texturesVec)
{
    m_quad = new SGQuad();
    m_material = new SGDefferedLightingMaterialPBRWithEmit(texturesVec);
    m_shader = -1;
    m_useFullScreen = true;
}

void Renderer::DefferedQuad::SetScreenTextures(std::vector<SGTexture2D*> texturesVec)
{
    m_screenTextures = texturesVec;
}

void Renderer::DefferedQuad::AddScreenTextures(SGTexture2D* texture)
{
    m_screenTextures.push_back(texture);
}

void Renderer::DefferedQuad::SwapScreenTexture()
{
    RENDER_WARDER(m_currentIndex >= 0 && m_currentIndex < m_screenTextures.size());
    glUseProgram(m_shader);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_screenTextures[m_currentIndex]->GetTextureID());
    int screenTextureLoc = glGetUniformLocation(m_shader, "screenTexture"); 
    if (screenTextureLoc != -1)
    {
        glUniform1i(screenTextureLoc, 0);
    }
    else
    {
        std::cout << "Deffered Quad Error: Find Screen Texture Failed!" << std::endl;
    }
}

void Renderer::DefferedQuad::ChangeRenderTextures(GLint vKey, GLint vScancode, GLint vAction, GLint vMode)
{
    if (vKey == GLFW_KEY_1 && vAction == GLFW_PRESS) {
        m_currentIndex = 0;
    }
    if (vKey == GLFW_KEY_2 && vAction == GLFW_PRESS) {
        m_currentIndex = 1;
    }
    if (vKey == GLFW_KEY_3 && vAction == GLFW_PRESS) {
        m_currentIndex = 2;
    }
    if (vKey == GLFW_KEY_4 && vAction == GLFW_PRESS) {
        m_currentIndex = 3;
    }
    if (vKey == GLFW_KEY_5 && vAction == GLFW_PRESS) {
        m_currentIndex = 4;
    }
}

void Renderer::DefferedQuad::ShowScreenTexture()
{
    assert(m_useFullScreen == false);
    glDisable(GL_DEPTH_TEST);
    //Update Screen Texture
    SwapScreenTexture();
    //Draw Quad
    m_quad->Draw(m_shader);
    glEnable(GL_DEPTH_TEST);
}

void Renderer::DefferedQuad::DefferedRendering(SGController* controller, SGScene* scene)
{
    assert(m_useFullScreen == true);
    glDisable(GL_DEPTH_TEST);
    m_material->Load();
    controller->LoadToShader(m_material->GetShaderInstance());
    scene->UploadStaticLight(m_material->GetShaderInstance());
    m_quad->Draw(m_material->GetShaderInstance());
    glEnable(GL_DEPTH_TEST);
}
