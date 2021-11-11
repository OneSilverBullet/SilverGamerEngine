#ifndef SG_RENDER_PASS
#define SG_RENDER_PASS

#include "RenderSetting.h"
#include "Shader.h"


namespace Renderer
{
	/*
	* RenderPass: To collect the same render data, and render one pass.
	*/
	class IRenderPass
	{
	public:
		//Initializer
		IRenderPass();
		IRenderPass(const std::string& passName, int excutionOrder);
		IRenderPass(const std::string& passName, int excutionOrder, SG_RENDER_PASS_TYPE type);
		virtual ~IRenderPass();

		//RenderPass Public Functions

		//Interface about render pass
		const std::string& GetPassName() const { return m_passName; }
		int GetExecutionOrder() { return m_executionOrder; }
		SG_RENDER_PASS_TYPE GetRenderPassType() { return m_renderType; }
		void SetRenderPassShader(SGShader* shader) { m_shader = shader; }

	protected:
		SGShader* m_shader; // shaderPass

	private:
		std::string m_passName = "";
		SG_RENDER_PASS_TYPE m_renderType = SG_RENDER_PASS_TYPE::NOMAL;
		int m_executionOrder = -1;
	};





}






#endif