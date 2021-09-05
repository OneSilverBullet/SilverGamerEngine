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
#if _DEBUG
	glutInitContextFlags(GLUT_DEBUG);
#endif
	glutInitContextVersion(4, 3);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(APPLICATION_WIDTH, APLLICATION_HEIGHT);
	window = glutCreateWindow("SilverGamer");

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