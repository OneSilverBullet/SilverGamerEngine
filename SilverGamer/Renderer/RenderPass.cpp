#include "RenderPass.h"

Renderer::IRenderPass::IRenderPass()
{
}

Renderer::IRenderPass::IRenderPass(const std::string& passName, int excutionOrder)
	: m_passName(passName), m_executionOrder(m_executionOrder)
{
}

Renderer::IRenderPass::IRenderPass(const std::string& passName, int excutionOrder, SG_RENDER_PASS_TYPE type)
	: m_passName(passName), m_executionOrder(m_executionOrder), m_renderType(type)
{
}

Renderer::IRenderPass::~IRenderPass()
{
}
