#ifndef SG_GRAPHICS
#define SG_GRAPHICS

#include "RenderSetting.h"
#include "Scene.h"
#include "FrameBuffer.h"
#include "Controller.h" 

namespace Renderer
{
	//用于进行延迟渲染的Quad
	class DefferedQuad
	{
	public:
		DefferedQuad(std::vector<SGTexture2D*>textureVec);
		void SetScreenTextures(std::vector<SGTexture2D*> texturesVec);
		void AddScreenTextures(SGTexture2D* texture);
		void SwapScreenTexture();
		void ChangeRenderTextures(GLint vKey, GLint vScancode, GLint vAction, GLint vMode);
		void ShowScreenTexture(); //Render Target Texture
		void DefferedRendering(SGController* controller, SGScene* scene);
		
	public:
		SGQuad* m_quad;
		GLuint m_shader;
		SGDefferedLightingMaterialPBRWithEmit* m_material;
		std::vector<SGTexture2D*> m_screenTextures;
		int m_currentIndex = 0;
	};


	class SGGraphics
	{
	public:
		SGGraphics();
		~SGGraphics();

		virtual void Init();
		virtual void Render();
		void GetSupportExtensions();
		bool CheckExtension(const std::string& extensionName);

		GLFWwindow* GetGLFWWindow() { return m_window; }
		SGScene* GetScene() { return m_scene; }
		SGFrameBuffer* GetFrameBuffer() { return m_frameBuffer; }

	private:
		std::unordered_map<std::string, GLuint> m_shaderInstanceMap;
		GLuint m_shaderInstance;
		std::vector<std::string> m_supportExtensions; //当前图形Device所支持的拓展

		GLFWwindow* m_window;
		SGFrameBuffer* m_frameBuffer; 
		SGScene* m_scene;
		SGController* m_controller;
		int w, h;

		//About Render Pipeline
		DefferedQuad* m_defferedQuad;
		SGFrameBuffer* m_GBuffer; 

	};



}



#endif