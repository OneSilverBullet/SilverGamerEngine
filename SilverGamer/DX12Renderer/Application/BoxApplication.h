#pragma once

#include "Application.h"
#include "../Engine/Camera.h"
#include "../BasicFrame/Shader.h"
#include "../BasicFrame/PSO.h"
#include "../BasicFrame/DescriptorManager.h"
#include "../BasicFrame/CommandObject.h"
#include "../BasicFrame/UploadBuffer.h"
#include "../Engine/GeometryBase.h"
#include "Graphics.h"
#include <unordered_map>

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
	void BuildFrameResources();
	void BuildRenderItems();

	void DrawRenderItems();
	void UpdateMainPassCB(const SilverEngineLib::SGGeneralTimer& timer);
	void UpdateObjectCB(const SilverEngineLib::SGGeneralTimer& timer);
	void UpdateCamera(const SilverEngineLib::SGGeneralTimer& timer);

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
	//frame resource
	std::vector<std::unique_ptr<FrameResource>> m_frameResources;
	FrameResource* m_currentFrameResource = nullptr;
	int m_currentFrameResourceIndex = 0;

	//all render items
	std::vector<std::unique_ptr<RenderItem>> m_renderItems;

	//all geometries
	std::unordered_map<std::string, std::unique_ptr<MeshBase>> m_geometries;

	//render layer
	std::vector<RenderItem*> m_renderItemLayer[(int)RenderLayer::Count];

	//main pass constants
	RenderPassConstants m_mainPassConstants;


	//Shader* m_shader;
	//ThirdRoleCamera* m_camera;

	Microsoft::WRL::ComPtr<ID3DBlob> mvsByteCode = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> mpsByteCode = nullptr;
	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;

	float mTheta = 1.5f * XM_PI;
	float mPhi = XM_PIDIV4;
	float mRadius = 5.0f;

	XMFLOAT3 mEyePos = { 0.0f,0.0f,0.0f };
	XMFLOAT4X4 mWorld = MathHelper::Identity4x4();
	XMFLOAT4X4 mView = MathHelper::Identity4x4();
	XMFLOAT4X4 mProj = MathHelper::Identity4x4();


	Microsoft::WRL::ComPtr<ID3D12RootSignature> mRootSignature = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mCbvHeap = nullptr;
	std::unique_ptr<UploadBuffer<ObjectConstants>> mObjectCB = nullptr;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> mPSO = nullptr;

	//MeshBase* m_geometry;
	POINT m_lastPoint;

	XMFLOAT4X4 m_projMat;
};