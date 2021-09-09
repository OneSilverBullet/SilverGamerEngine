#include "Application.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

Renderer::SGApplication & Renderer::SGApplication::GetInstance()
{
	static SGApplication app;
	return app;
}

void Renderer::SGApplication::Init(int argc, char ** argv)
{
    //初始化本地对象
    m_graphicPipline = new SGGraphics();
    m_controller = new SGControllerFPS(m_graphicPipline->GetScene()->GetCamera(), APPLICATION_WIDTH, APLLICATION_HEIGHT);

    m_graphicPipline->Init(); //初始化当前渲染GLFW环境
}

void Renderer::SGApplication::Run()
{
    while (!glfwWindowShouldClose(m_graphicPipline->GetGLFWWindow()))
    {
        SGTimer::GetInstance()->UpdateTimer(glfwGetTime()); //更新当前时间逻辑
        

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
}

GLFWwindow* Renderer::SGApplication::GetGLFWWindow()
{
    RENDER_WARDER_NULL_ALERT(m_graphicPipline);
    return m_graphicPipline->GetGLFWWindow();
}


void idle() {
	glutPostRedisplay();

}

void render() {
	auto&  app = Renderer::SGApplication::GetInstance();
	app.GetGraphics()->Render();
	app.GetController()->Update();
}

void keyboard(unsigned char key, int x, int y) {
	auto& app = Renderer::SGApplication::GetInstance();
	app.GetController()->OnKeyboard(key, x, y);
}

void keyboard_up(unsigned char key, int x, int y) {
	auto& app = Renderer::SGApplication::GetInstance();
	app.GetController()->OnKeyboardUp(key, x, y);
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}