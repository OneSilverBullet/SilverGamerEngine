#pragma once

#include "Application.h"
#include "../Engine/Camera.h"
#include "../BasicFrame/Shader.h"
#include "../BasicFrame/PSO.h"
#include "../BasicFrame/DescriptorManager.h"
#include "../BasicFrame/CommandObject.h"
#include "../BasicFrame/UploadBuffer.h"
#include "../Engine/GeometryBase.h"

class BoxApplication : public IApplication
{
public:
	BoxApplication(HINSTANCE hInstance);
	~BoxApplication(){}

	virtual bool Initialize() override;

	void BuildCamera();
	void BuildDescriptorHeaps();
	void BuildConstantBuffers();
	void BuildRootSignature();
	void BuildShaderAndInputLayout();
	void BuildPSO();
	void BuildGeometry();

	//÷ÿππ
	virtual void Update(const SilverEngineLib::SGGeneralTimer& timer);
	virtual void Render(const SilverEngineLib::SGGeneralTimer& timer);
	// Convenience overrides for handling mouse input.
	virtual void OnMouseDown(WPARAM btnState, int x, int y);
	virtual void OnMouseUp(WPARAM btnState, int x, int y);
	virtual void OnMouseMove(WPARAM btnState, int x, int y);

protected:
	virtual void OnResize() override; //The Size changed function

private:
	Shader* m_shader;
	PSO* m_pixelStateObject;
	ThirdRoleCamera* m_camera;
	RootSignature* m_rootSignature;
	DescirptorHeap* m_descriptorHeap;
	ConstantBuffer<ObjectConstants>* m_objectBuffer;
	MeshBase* m_geometry;
	POINT m_lastPoint;

	XMFLOAT4X4 m_projMat;
};