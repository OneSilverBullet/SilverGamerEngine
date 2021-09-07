#include "Application.h"

void idle();

void render();

void keyboard(unsigned char key, int x, int y);

void keyboard_up(unsigned char key, int x, int y);



Renderer::SGApplication & Renderer::SGApplication::GetInstance()
{
	static SGApplication app;
	return app;
}

void Renderer::SGApplication::Init(int argc, char ** argv)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);




	m_graphicPipline = new SGGraphics();
	m_controller = new SGControllerFPS(m_graphicPipline->GetScene()->GetCamera(), APPLICATION_WIDTH, APLLICATION_HEIGHT);

	glutDisplayFunc(render);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboard_up);
	glutIdleFunc(idle);
}

void Renderer::SGApplication::Run()
{
	glutMainLoop();
	glutDestroyWindow(window);
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