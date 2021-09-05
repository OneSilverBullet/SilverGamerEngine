#include "Graphics.h"
#include "3rdlib/include/DebugCallback.h"
#include "Scene.h"
#include "Shader.h"

Renderer::SGGraphics::SGGraphics()
{
}

Renderer::SGGraphics::~SGGraphics()
{
}

void Renderer::SGGraphics::Init()
{
	glewInit();
	RegisterCallback();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	w = glutGet(GLUT_WINDOW_WIDTH);
	h = glutGet(GLUT_WINDOW_HEIGHT);

	//���س���
	m_scene = new SGScene();
	SGBaseMesh* sceneMesh = SGBaseMesh::SGBaseMeshCube();
	m_scene->AddMesh(sceneMesh);
	SGPointLight* pointLight = new SGPointLight(glm::vec3(2, 2, 2));
	m_scene->AddLight(pointLight);
	m_scene->Init(); //��ʼ��
	//��ʼ����ӦShaderInstance
	m_shaderInstance = SGShaderFactory::GetInstance()->loadNormalShader()->GetShaderProgramId();
}

void Renderer::SGGraphics::Render()
{
	m_scene->Render(m_shaderInstance); //���л���
}
