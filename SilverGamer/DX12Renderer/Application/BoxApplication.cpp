#include "BoxApplication.h"
#include "../BasicFrame/UploadBuffer.h"

BoxApplication::BoxApplication(HINSTANCE hInstance)
	: IApplication(hInstance)
{
}

bool BoxApplication::Initialize()
{
	if (!IApplication::Initialize())
		return false;

	ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr));

	BuildCamera();
	BuildDescriptorHeaps();
	BuildConstantBuffers();
	BuildRootSignature();
	BuildShaderAndInputLayout();
	BuildGeometry();
	BuildPSO();


	ThrowIfFailed(mCommandList->Close());
	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	FlushCommandQueue();

	return true;
}

void BoxApplication::BuildCamera()
{
	//m_camera = new ThirdRoleCamera();
	//XMFLOAT3 targetPos(0, 0, 0);
	//m_camera->SetTarget(targetPos);
}

void BoxApplication::BuildDescriptorHeaps()
{
	//Create Constant Shader View
	D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc;
	cbvHeapDesc.NumDescriptors = 1;
	cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	cbvHeapDesc.NodeMask = 0;
	ThrowIfFailed(md3dDevice->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&mCbvHeap)));
}

void BoxApplication::BuildConstantBuffers()
{
	mObjectCB = std::make_unique<UploadBuffer<ObjectConstants>>(md3dDevice.Get(), 1, true);
	UINT objCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));
	//Get the constant buffer's gpu addr
	D3D12_GPU_VIRTUAL_ADDRESS cbAddress = mObjectCB->GetResource()->GetGPUVirtualAddress();
	int boxCBufIndex = 0;
	cbAddress += boxCBufIndex * objCBByteSize;
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
	cbvDesc.BufferLocation = cbAddress;
	cbvDesc.SizeInBytes = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));
	md3dDevice->CreateConstantBufferView(
		&cbvDesc,
		mCbvHeap->GetCPUDescriptorHandleForHeapStart());
}

void BoxApplication::BuildRootSignature()
{
	CD3DX12_ROOT_PARAMETER slotRootParameter[1];

	// Create a single descriptor table of CBVs.
	CD3DX12_DESCRIPTOR_RANGE cbvTable;
	cbvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
	slotRootParameter[0].InitAsDescriptorTable(1, &cbvTable);

	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(1, slotRootParameter, 0, nullptr,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	Microsoft::WRL::ComPtr<ID3DBlob> serializedRootSig = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
		serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());
	if (errorBlob != nullptr)
	{
		::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
	}
	ThrowIfFailed(hr);
	//Create Root Signature
	ThrowIfFailed(md3dDevice->CreateRootSignature(
		0,
		serializedRootSig->GetBufferPointer(),
		serializedRootSig->GetBufferSize(),
		IID_PPV_ARGS(&mRootSignature)));
}

void BoxApplication::BuildShaderAndInputLayout()
{
	HRESULT hr = S_OK;

	mvsByteCode = d3dUtil::CompileShader(L"ShaderResouce\\color.hlsl", nullptr, "VS", "vs_5_0");
	mpsByteCode = d3dUtil::CompileShader(L"ShaderResouce\\color.hlsl", nullptr, "PS", "ps_5_0");

	mInputLayout =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};
}

void BoxApplication::BuildGeometry()
{
	std::array<Vertex, 8> vertices =
	{
		Vertex({ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::White) }),
		Vertex({ XMFLOAT3(-1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Black) }),
		Vertex({ XMFLOAT3(+1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Red) }),
		Vertex({ XMFLOAT3(+1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::Green) }),
		Vertex({ XMFLOAT3(-1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Blue) }),
		Vertex({ XMFLOAT3(-1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Yellow) }),
		Vertex({ XMFLOAT3(+1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Cyan) }),
		Vertex({ XMFLOAT3(+1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Magenta) })
	};

	std::array<std::uint16_t, 36> indices =
	{
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};

	const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

	std::unique_ptr<MeshBase> mBoxGeo = std::make_unique<MeshBase>();
	mBoxGeo->m_name = "boxGeo";

	ThrowIfFailed(D3DCreateBlob(vbByteSize, &mBoxGeo->m_vertexBufferCPU));
	CopyMemory(mBoxGeo->m_vertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

	ThrowIfFailed(D3DCreateBlob(ibByteSize, &mBoxGeo->m_indexBufferCPU));
	CopyMemory(mBoxGeo->m_indexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

	mBoxGeo->m_vertexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), vertices.data(), vbByteSize, mBoxGeo->m_vertexBufferUploader);

	mBoxGeo->m_indexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), indices.data(), ibByteSize, mBoxGeo->m_indexBufferUploader);

	mBoxGeo->m_vertexByteStride = sizeof(Vertex);
	mBoxGeo->m_vertexBufferByteSize = vbByteSize;
	mBoxGeo->m_indexFormat = DXGI_FORMAT_R16_UINT;
	mBoxGeo->m_indexBufferByteSize = ibByteSize;

	ISubMesh submesh;
	submesh.m_indexCount = (UINT)indices.size();
	submesh.m_startIndexLocation = 0;
	submesh.m_baseVertexLocation = 0;

	mBoxGeo->m_submeshes["box"] = submesh;

	m_geometries["box"] = std::move(mBoxGeo);
}

void BoxApplication::BuildFrameResources()
{
	for (int i = 0; i < gNumFrameResources; ++i) {
		m_frameResources.push_back(std::make_unique<FrameResource>(
			md3dDevice.Get(), 1, (UINT)m_renderItems.size(), 1));
	}
}

void BoxApplication::BuildRenderItems()
{
	auto renderItem_box = std::make_unique<RenderItem>();
	renderItem_box->m_world = MathHelper::Identity4x4();
	renderItem_box->m_objectCBIndex = 0;
	renderItem_box->geo = m_geometries["box"].get();
	renderItem_box->m_primitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	renderItem_box->m_indexCount = renderItem_box->geo->m_submeshes["box"].m_indexCount;
	renderItem_box->m_startIndexLocation = renderItem_box->geo->m_submeshes["box"].m_startIndexLocation;
	renderItem_box->m_baseVertexLocation = renderItem_box->geo->m_submeshes["box"].m_baseVertexLocation;

	m_renderItemLayer[(int)RenderLayer::Opaque].push_back(renderItem_box.get());
	m_renderItems.push_back(renderItem_box);
}

void BoxApplication::UpdateMainPassCB(const SilverEngineLib::SGGeneralTimer& timer)
{
	XMMATRIX view = XMLoadFloat4x4(&mView);
	XMMATRIX proj = XMLoadFloat4x4(&mProj);
	XMMATRIX viewProj = XMMatrixMultiply(view, proj);
	XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(view), view);
	XMMATRIX invProj = XMMatrixInverse(&XMMatrixDeterminant(proj), proj);
	XMMATRIX invViewProj = XMMatrixInverse(&XMMatrixDeterminant(viewProj), viewProj);
	XMStoreFloat4x4(&m_mainPassConstants.m_view, view);
	XMStoreFloat4x4(&m_mainPassConstants.m_proj, proj);
	XMStoreFloat4x4(&m_mainPassConstants.m_viewProj, viewProj);
	XMStoreFloat4x4(&m_mainPassConstants.m_invView, invView);
	XMStoreFloat4x4(&m_mainPassConstants.m_invProj, invProj);
	XMStoreFloat4x4(&m_mainPassConstants.m_invViewProj, invViewProj);
	m_mainPassConstants.m_eyePosW = mEyePos;
	m_mainPassConstants.m_renderTargetSize = XMFLOAT2((float)mClientWidth, (float)mClientHeight);
	m_mainPassConstants.m_renderTargetSize = XMFLOAT2(1.0f/(float)mClientWidth, 1.0f/(float)mClientHeight);
	m_mainPassConstants.m_nearZ = 1.0f;
	m_mainPassConstants.m_farZ = 1000.0f;
	m_mainPassConstants.m_totalTime = timer.TotalTime();
	m_mainPassConstants.m_deltaTime = timer.DeltaTime();

	auto currentPassCB = m_currentFrameResource->m_passCB.get();
	currentPassCB->CopyData(0, m_mainPassConstants);
}

void BoxApplication::UpdateObjectCB(const SilverEngineLib::SGGeneralTimer& timer)
{
	//load the matrix from render items to the current frame resources
	auto currObjectCB = m_currentFrameResource->m_objectCB.get();
	for (auto& e : m_renderItems) {
		if (e->m_numFrameDirty > 0) { //This object need to update
			XMMATRIX world = XMLoadFloat4x4(&e->m_world);
			//store object constants
			ObjectConstants objConstants;
			XMStoreFloat4x4(&objConstants.world, XMMatrixTranspose(world));
			currObjectCB->CopyData(e->m_objectCBIndex, objConstants);
			e->m_numFrameDirty--;//update 
		}
	}
}

void BoxApplication::UpdateCamera(const SilverEngineLib::SGGeneralTimer& timer)
{
	float x = mRadius * sinf(mPhi) * cosf(mTheta);
	float z = mRadius * sinf(mPhi) * sinf(mTheta);
	float y = mRadius * cosf(mPhi);

	// Build the view matrix.
	XMVECTOR pos = XMVectorSet(x, y, z, 1.0f);
	XMVECTOR target = XMVectorZero();
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMMATRIX view = XMMatrixLookAtLH(pos, target, up);
	XMStoreFloat4x4(&mView, view);
}

void BoxApplication::BuildPSO()
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	psoDesc.InputLayout = { mInputLayout.data(), (UINT)mInputLayout.size() };
	psoDesc.pRootSignature = mRootSignature.Get();
	psoDesc.VS =
	{
		reinterpret_cast<BYTE*>(mvsByteCode->GetBufferPointer()),
		mvsByteCode->GetBufferSize()
	};
	psoDesc.PS =
	{
		reinterpret_cast<BYTE*>(mpsByteCode->GetBufferPointer()),
		mpsByteCode->GetBufferSize()
	};
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = mBackBufferFormat;
	psoDesc.SampleDesc.Count = m4xMsaaState ? 4 : 1;
	psoDesc.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
	psoDesc.DSVFormat = mDepthStencilFormat;
	ThrowIfFailed(md3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&mPSO)));
}

void BoxApplication::Update(const SilverEngineLib::SGGeneralTimer& timer)
{
	UpdateCamera(timer);

	//Exchange current frame resource
	m_currentFrameResourceIndex = (m_currentFrameResourceIndex + 1) % gNumFrameResources;
	m_currentFrameResource = m_frameResources[m_currentFrameResourceIndex].get();

	//If GPU not finish the work, then waite
	if (m_currentFrameResource->m_fence && mFence->GetCompletedValue() < m_currentFrameResource->m_fence) {
		HANDLE eventHandle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);
		ThrowIfFailed(mFence->SetEventOnCompletion(m_currentFrameResource->m_fence, eventHandle));
		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);

	}

	// Update the constant buffer with the latest worldViewProj matrix.
	UpdateObjectCB(timer);
	UpdateMainPassCB(timer);
}

void BoxApplication::Render(const SilverEngineLib::SGGeneralTimer& timer)
{
	ThrowIfFailed(mDirectCmdListAlloc->Reset());
	ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), mPSO.Get()));

	//resuse the command list
	mCommandList->RSSetViewports(1, &m_screenViewport);
	mCommandList->RSSetScissorRects(1, &m_scissorRect);
	
	//indicate a state transition on the resource usage
	mCommandList->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
			D3D12_RESOURCE_STATE_PRESENT,
			D3D12_RESOURCE_STATE_RENDER_TARGET));

	//clear the backbuffer and depthstencile buffer
	mCommandList->ClearRenderTargetView(CurrentBackBufferView(),
		DirectX::Colors::LightSteelBlue, 0, nullptr);
	mCommandList->ClearDepthStencilView(DepthStencilView(),
		D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	//render the target texture
	mCommandList->OMSetRenderTargets(
		1, &CurrentBackBufferView(), true,
		&DepthStencilView());

	//Set the descriptor
	ID3D12DescriptorHeap* descriptorHeaps[] = { mCbvHeap.Get() };
	mCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	//Set the Root Signature
	mCommandList->SetGraphicsRootSignature(mRootSignature.Get());

	//Upload the geometry
	mCommandList->IASetVertexBuffers(0, 1, &mBoxGeo->VertexBufferView());
	mCommandList->IASetIndexBuffer(&mBoxGeo->IndexBufferView());
	mCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//
	mCommandList->SetGraphicsRootDescriptorTable(0, mCbvHeap->GetGPUDescriptorHandleForHeapStart());

	//Draw model instance
	mCommandList->DrawIndexedInstanced(
		mBoxGeo->m_submeshes["box"].m_indexCount, 1, 0, 0, 0
	);

	mCommandList->ResourceBarrier(
		1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PRESENT));
	
	ThrowIfFailed(mCommandList->Close());
	// Add the command list to the queue for execution.
	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	ThrowIfFailed(mSwapChain->Present(0, 0));
	mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;

	//fence flush command queue
	FlushCommandQueue();
}

void BoxApplication::OnMouseDown(WPARAM btnState, int x, int y)
{
	m_lastPoint.x = x;
	m_lastPoint.y = y;
	SetCapture(m_mainWnd);
}

void BoxApplication::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void BoxApplication::OnMouseMove(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0) {
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.25f * static_cast<float>(x - m_lastPoint.x));
		float dy = XMConvertToRadians(0.25f * static_cast<float>(y - m_lastPoint.y));

		mTheta -= dx;
		mPhi -= dy;

		// Restrict the angle mPhi.
		mPhi = MathHelper::Clamp(mPhi, 0.1f, MathHelper::Pi - 0.1f);
	}
	else if ((btnState & MK_RBUTTON) != 0) {
		// Make each pixel correspond to 0.005 unit in the scene.
		float dx = 0.005f * static_cast<float>(x - m_lastPoint.x);
		float dy = 0.005f * static_cast<float>(y - m_lastPoint.y);
		// Restrict the radius.
		mRadius += dx - dy;

		// Restrict the radius.
		mRadius = MathHelper::Clamp(mRadius, 3.0f, 15.0f);
	}
	m_lastPoint.x = x;
	m_lastPoint.y = y;
}

void BoxApplication::OnResize()
{
	IApplication::OnResize();
	XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f * MathHelper::Pi, m_state.AspectRatio(), 1.0f, 1000.0f);
	XMStoreFloat4x4(&mProj, P);
}
