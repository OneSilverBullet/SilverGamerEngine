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

	m_commandList->Reset();

	BuildCamera();
	BuildDescriptorHeaps();
	BuildConstantBuffers();
	BuildRootSignature();
	BuildShaderAndInputLayout();
	BuildGeometry();
	BuildPSO();

	m_commandQueue->ExecuteCommandList(m_commandList);

	m_fence->FlushCommandQueue();

	return true;
}

void BoxApplication::BuildCamera()
{
	m_camera = new ThirdRoleCamera();
	XMFLOAT3 targetPos(0, 0, 0);
	m_camera->SetTarget(targetPos);
}

void BoxApplication::BuildDescriptorHeaps()
{
	m_descriptorHeap = new DescirptorHeap(m_device);
}

void BoxApplication::BuildConstantBuffers()
{
	m_objectBuffer = new ConstantBuffer<ObjectConstants>(m_device, m_descriptorHeap);
}

void BoxApplication::BuildRootSignature()
{
	m_rootSignature = new RootSignature(m_device);
}

void BoxApplication::BuildShaderAndInputLayout()
{
	m_shader = new Shader();
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

	m_geometry = new MeshBase();
	m_geometry->m_name = "boxGeo";

	ThrowIfFailed(D3DCreateBlob(vbByteSize, &m_geometry->m_vertexBufferCPU));
	CopyMemory(m_geometry->m_vertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

	ThrowIfFailed(D3DCreateBlob(ibByteSize, &m_geometry->m_indexBufferCPU));
	CopyMemory(m_geometry->m_indexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);


	//Create Vertex Buffer GPU
	m_geometry->m_vertexBufferGPU = d3dUtil::CreateDefaultBuffer(m_device->GetDevice(),
		m_commandList->GetCommandList(), vertices.data(), vbByteSize, m_geometry->m_vertexBufferUploader);

	m_geometry->m_indexBufferGPU = d3dUtil::CreateDefaultBuffer(m_device->GetDevice(),
		m_commandList->GetCommandList(), indices.data(), ibByteSize, m_geometry->m_indexBufferUploader);

	//Fill the desc
	m_geometry->m_vertexByteStride = sizeof(Vertex);
	m_geometry->m_vertexBufferByteSize = vbByteSize;
	m_geometry->m_indexFormat = DXGI_FORMAT_R16_UINT;
	m_geometry->m_indexBufferByteSize = ibByteSize;

	ISubMesh submesh;
	submesh.m_indexCount = (UINT)indices.size();
	submesh.m_startIndexLocation = 0;
	submesh.m_baseVertexLocation = 0;

	m_geometry->m_submeshes["box"] = submesh;
}

void BoxApplication::BuildPSO()
{
	m_pixelStateObject = new PSO(m_device, m_swapChain, m_shader, m_rootSignature);
}

void BoxApplication::Update(const SilverEngineLib::SGGeneralTimer& timer)
{
	XMFLOAT4X4 m_modelMat = MathHelper::Identity4x4();

	XMMATRIX modelMat = XMLoadFloat4x4(&m_modelMat);
	XMMATRIX viewMat = m_camera->GetViewMatrix();
	XMMATRIX projMat = XMLoadFloat4x4(&m_projMat);
	XMMATRIX worldViewProj = modelMat * viewMat * projMat;
	
	//Upload to the Constant Buffer
	ObjectConstants constantBufferData;
	XMStoreFloat4x4(&constantBufferData.worldViewProj, XMMatrixTranspose(worldViewProj));
	m_objectBuffer->CopyData(0, constantBufferData);
}

void BoxApplication::Render(const SilverEngineLib::SGGeneralTimer& timer)
{
	m_commandList->Reset(m_pixelStateObject->GetPSO());

	//resuse the command list
	m_commandList->GetCommandList()->RSSetViewports(1, &m_screenViewport);
	m_commandList->GetCommandList()->RSSetScissorRects(1, &m_scissorRect);
	
	//indicate a state transition on the resource usage
	m_commandList->GetCommandList()->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(m_swapChain->GetCurrentChainBuffer(),
			D3D12_RESOURCE_STATE_PRESENT,
			D3D12_RESOURCE_STATE_RENDER_TARGET));

	//clear the backbuffer and depthstencile buffer
	m_commandList->GetCommandList()->ClearRenderTargetView(m_swapChain->CurrentBackBufferView(),
		DirectX::Colors::LightSteelBlue, 0, nullptr);
	m_commandList->GetCommandList()->ClearDepthStencilView(m_swapChain->DepthStencilView(), 
		D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	//render the target texture
	m_commandList->GetCommandList()->OMSetRenderTargets(
		1, &m_swapChain->CurrentBackBufferView(), true,
		&m_swapChain->DepthStencilView());

	//Set the descriptor
	ID3D12DescriptorHeap* descriptorHeaps[] = { m_descriptorHeap->GetHeapPtr() };
	m_commandList->GetCommandList()->SetDescriptorHeaps(1, descriptorHeaps);

	//Set the Root Signature
	m_commandList->GetCommandList()->SetGraphicsRootSignature(m_rootSignature->GetRootSig());

	//Upload the geometry
	m_commandList->GetCommandList()->IASetVertexBuffers(0, 1, &m_geometry->VertexBufferView());
	m_commandList->GetCommandList()->IASetIndexBuffer(&m_geometry->IndexBufferView());
	m_commandList->GetCommandList()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//
	m_commandList->GetCommandList()->SetGraphicsRootDescriptorTable(0, m_descriptorHeap->GetGPUStartDescriptorHandle());

	//Draw model instance
	m_commandList->GetCommandList()->DrawIndexedInstanced(
		m_geometry->m_submeshes["box"].m_indexCount, 1, 0, 0, 0
	);

	m_commandList->GetCommandList()->ResourceBarrier(
		1, &CD3DX12_RESOURCE_BARRIER::Transition(m_swapChain->GetCurrentChainBuffer(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PRESENT));
	
	//m_commandList->Close();
	m_commandQueue->ExecuteCommandList(m_commandList);

	m_swapChain->Present(0, 0);
	m_swapChain->ProcessLoop();

	//fence flush command queue
	m_fence->FlushCommandQueue();
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
	m_lastPoint.x = x;
	m_lastPoint.y = y;
	if ((btnState & MK_LBUTTON) != 0) {
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.25f * static_cast<float>(x - m_lastPoint.x));
		float dy = XMConvertToRadians(0.25f * static_cast<float>(y - m_lastPoint.y));

		m_camera->RotateCamera(dx, dy);
	}
	else if ((btnState & MK_RBUTTON) != 0) {
		// Make each pixel correspond to 0.005 unit in the scene.
		float dx = 0.005f * static_cast<float>(x - m_lastPoint.x);
		float dy = 0.005f * static_cast<float>(y - m_lastPoint.y);
		// Restrict the radius.
		m_camera->MoveDistance(dx - dy);
	}
}

void BoxApplication::OnResize()
{
	IApplication::OnResize();
	XMMATRIX projMatrix = XMMatrixPerspectiveFovLH(0.25f * PI, m_state.AspectRatio(), 1.0f, 1000.0f);
	XMStoreFloat4x4(&m_projMat, projMatrix);
}
