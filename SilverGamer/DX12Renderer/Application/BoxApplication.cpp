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

	mCbvSrvUavDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	LoadTextures();
	BuildRootSignature();
	BuildDescriptorHeaps();
	BuildShaderAndInputLayout();
	BuildGeometry();
	BuildChuyin();
	BuildMaterials();
	BuildRenderItems();
	BuildFrameResources();
	BuildPSO();

	ThrowIfFailed(mCommandList->Close());
	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	FlushCommandQueue();

	return true;
}

void BoxApplication::LoadTextures()
{
	auto testImage = std::make_unique<SGDX12::Texture>();
	testImage->m_name = "test";
	testImage->m_filename = L"../Resource/Textures/WoodCrate01.dds";
	ThrowIfFailed(DirectX::CreateDDSTextureFromFile12(
		md3dDevice.Get(), mCommandList.Get(),
		testImage->m_filename.c_str(), 
		testImage->m_resource,
		testImage->m_uploadHeap));

	m_textures[testImage->m_name] = std::move(testImage);

	auto fenceTexture = std::make_unique<SGDX12::Texture>();
	fenceTexture->m_name = "fence";
	fenceTexture->m_filename = L"../Resource/Textures/WireFence.dds";
	ThrowIfFailed(DirectX::CreateDDSTextureFromFile12(
		md3dDevice.Get(), mCommandList.Get(),
		fenceTexture->m_filename.c_str(),
		fenceTexture->m_resource,
		fenceTexture->m_uploadHeap));
	m_textures[fenceTexture->m_name] = std::move(fenceTexture);



	//Load CHUYIN texture for sprite
	auto chuyinTexture = std::make_unique<SGDX12::Texture>();
	chuyinTexture->m_name = "chuyin";
	chuyinTexture->m_filename = L"../Resource/Textures/chuyin.dds";
	ThrowIfFailed(DirectX::CreateDDSTextureFromFile12(
		md3dDevice.Get(), mCommandList.Get(),
		chuyinTexture->m_filename.c_str(),
		chuyinTexture->m_resource,
		chuyinTexture->m_uploadHeap));
	m_textures[chuyinTexture->m_name] = std::move(chuyinTexture);
}

void BoxApplication::BuildCamera()
{
	//m_camera = new ThirdRoleCamera();
	//XMFLOAT3 targetPos(0, 0, 0);
	//m_camera->SetTarget(targetPos);
}

void BoxApplication::BuildDescriptorHeaps()
{
	//Create SRV Heap
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.NumDescriptors = 2;
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	ThrowIfFailed(md3dDevice->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&mSrvHeap)));

	//Fill out the actual descriptors
	CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(mSrvHeap->GetCPUDescriptorHandleForHeapStart());

	auto testImage = m_textures["fence"]->m_resource;
	auto spriteImage = m_textures["chuyin"]->m_resource;

	//Build Target Shader Resource View onto the srvHeap
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = testImage->GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = testImage->GetDesc().MipLevels;
	md3dDevice->CreateShaderResourceView(testImage.Get(), &srvDesc, hDescriptor);

	hDescriptor.Offset(1, mCbvSrvUavDescriptorSize);
	srvDesc.Format = spriteImage->GetDesc().Format;
	srvDesc.Texture2D.MipLevels = spriteImage->GetDesc().MipLevels;
	md3dDevice->CreateShaderResourceView(spriteImage.Get(), &srvDesc, hDescriptor);

	/* For Texture Array
	auto desc = treeArrayTex->GetDesc();
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc.Format = treeArrayTex->GetDesc().Format;
	srvDesc.Texture2DArray.MostDetailedMip = 0;
	srvDesc.Texture2DArray.MipLevels = -1;
	srvDesc.Texture2DArray.FirstArraySlice = 0;
	srvDesc.Texture2DArray.ArraySize = treeArrayTex->GetDesc().DepthOrArraySize;
	md3dDevice->CreateShaderResourceView(treeArrayTex.Get(), &srvDesc, hDescriptor);
	*/

}

void BoxApplication::BuildRootSignature()
{
	CD3DX12_DESCRIPTOR_RANGE texTable;
	texTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	CD3DX12_ROOT_PARAMETER slotRootParameter[4];

	//create shader visible resource function param descriptor
	slotRootParameter[0].InitAsDescriptorTable(1, &texTable, D3D12_SHADER_VISIBILITY_PIXEL);
	slotRootParameter[1].InitAsConstantBufferView(0);
	slotRootParameter[2].InitAsConstantBufferView(1);
	slotRootParameter[3].InitAsConstantBufferView(2);

	//the shader samplers array
	auto staticSamplers = GetStaticSamplers(); //Get the textures samplers

	int samplerSize = (int)staticSamplers.size();

	//A root signature is an array of root parameters
	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(4, slotRootParameter, 
		(UINT)staticSamplers.size(),
		staticSamplers.data(),
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

	const D3D_SHADER_MACRO alphaTestDefines[] = {
		//"ALPHA_TEST", "0",
		"FOG", "1",
		NULL, NULL,
	};

	m_shaderCode["normalVS"] = d3dUtil::CompileShader(L"ShaderResouce\\color.hlsl", nullptr, "VS", "vs_5_0");
	m_shaderCode["normalPS"] = d3dUtil::CompileShader(L"ShaderResouce\\color.hlsl", alphaTestDefines, "PS", "ps_5_0");


	const D3D_SHADER_MACRO chuyinMacro[] = {
		"ALPHA_TEST", "1",
		//"FOG", "0",
		NULL, NULL,
	};
	m_shaderCode["spriteVS"] = d3dUtil::CompileShader(L"ShaderResouce\\sprite.hlsl", nullptr, "VS", "vs_5_0");
	m_shaderCode["spriteGS"] = d3dUtil::CompileShader(L"ShaderResouce\\sprite.hlsl", nullptr, "GS", "gs_5_0");
	m_shaderCode["spritePS"] = d3dUtil::CompileShader(L"ShaderResouce\\sprite.hlsl", chuyinMacro, "PS", "ps_5_0");

	mInputLayout =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	mSpriteLayout =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

}

void BoxApplication::BuildGeometry()
{
	MeshData* data = GeometryGenerator::Box(1.0f, 1.0f, 1.0f, 3);

	ISubMesh submesh;
	submesh.m_indexCount = (UINT)data->m_indices.size();
	submesh.m_startIndexLocation = 0;
	submesh.m_baseVertexLocation = 0;

	std::vector<VertexData> vertices(data->m_vertices.size());
	for (int i = 0; i < data->m_vertices.size(); ++i) {
		vertices[i].Pos = data->m_vertices[i].Position;
		vertices[i].Normal = data->m_vertices[i].Normal;
		vertices[i].TexC = data->m_vertices[i].TexC;
	}

	std::vector<UINT16> indices = data->GetIndices16();

	const UINT vbByteSize = (UINT)vertices.size() * sizeof(VertexData);
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

	mBoxGeo->m_vertexByteStride = sizeof(VertexData);
	mBoxGeo->m_vertexBufferByteSize = vbByteSize;
	mBoxGeo->m_indexFormat = DXGI_FORMAT_R16_UINT;
	mBoxGeo->m_indexBufferByteSize = ibByteSize;


	mBoxGeo->m_submeshes["box"] = submesh;

	m_geometries["box"] = std::move(mBoxGeo);
}

void BoxApplication::BuildFrameResources()
{
	for (int i = 0; i < 3; ++i) {
		m_frameResources.push_back(std::make_unique<FrameResource>(
			md3dDevice.Get(), 1, (UINT)m_renderItems.size(), (UINT)m_materials.size(), 1));
	}
}

void BoxApplication::BuildRenderItems()
{
	auto renderItem_box = std::make_unique<RenderItem>();
	renderItem_box->m_world = MathHelper::Identity4x4();
	renderItem_box->m_objectCBIndex = 0;
	renderItem_box->mat = m_materials["testMat"].get();
	renderItem_box->geo = m_geometries["box"].get();
	renderItem_box->m_primitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	renderItem_box->m_indexCount = renderItem_box->geo->m_submeshes["box"].m_indexCount;
	renderItem_box->m_startIndexLocation = renderItem_box->geo->m_submeshes["box"].m_startIndexLocation;
	renderItem_box->m_baseVertexLocation = renderItem_box->geo->m_submeshes["box"].m_baseVertexLocation;

	m_renderItemLayer[(int)RenderLayer::Opaque].push_back(renderItem_box.get());
	m_renderItems.push_back(std::move(renderItem_box));

	auto chuyinSpritesRitem = std::make_unique<RenderItem>();
	chuyinSpritesRitem->m_world = MathHelper::Identity4x4();
	chuyinSpritesRitem->m_objectCBIndex = 1;
	chuyinSpritesRitem->mat = m_materials["chuyinSprite"].get();
	chuyinSpritesRitem->geo = m_geometries["chuyinGeo"].get();
	chuyinSpritesRitem->m_primitiveType = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
	chuyinSpritesRitem->m_indexCount = chuyinSpritesRitem->geo->m_submeshes["chuyinGeo"].m_indexCount;
	chuyinSpritesRitem->m_startIndexLocation = chuyinSpritesRitem->geo->m_submeshes["chuyinGeo"].m_startIndexLocation;
	chuyinSpritesRitem->m_baseVertexLocation = chuyinSpritesRitem->geo->m_submeshes["chuyinGeo"].m_baseVertexLocation;

	m_renderItemLayer[(int)RenderLayer::Sprite].push_back(chuyinSpritesRitem.get());
	m_renderItems.push_back(std::move(chuyinSpritesRitem));
}

void BoxApplication::BuildMaterials()
{
	auto testMat = std::make_unique<SGDX12::Material>();
	testMat->m_name = "testMat";
	testMat->m_matCBIndex = 0;
	testMat->m_diffuseSRVHeapIndex = 0;
	testMat->m_diffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	testMat->m_fresnelR0 = XMFLOAT3(0.05f, 0.05f, 0.05f);
	testMat->roughness = 0.2f;
	testMat->matTransform = MathHelper::Identity4x4();
	m_materials[testMat->m_name] = std::move(testMat);

	auto spriteMat = std::make_unique<SGDX12::Material>();
	spriteMat->m_name = "chuyinSprite";
	spriteMat->m_matCBIndex = 1;
	spriteMat->m_diffuseSRVHeapIndex = 1;
	spriteMat->m_diffuseAlbedo = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	spriteMat->m_fresnelR0 = XMFLOAT3(0.01f, 0.01f, 0.01f);
	spriteMat->roughness = 0.125f;
	m_materials[spriteMat->m_name] = std::move(spriteMat);
}

void BoxApplication::BuildChuyin()
{
	struct SpriteVertex
	{
		XMFLOAT3 pos;
		XMFLOAT2 size;
	};

	static const int treeCount = 16;
	std::array<SpriteVertex, 16> vertices;
	for (UINT i = 0; i < treeCount; ++i) {
		float x = MathHelper::RandF(-10.0f, 10.0f);
		float z = MathHelper::RandF(-10.0f, 10.0f);
		float y = MathHelper::RandF(-0.0f, 1.0f);
		vertices[i].pos = XMFLOAT3(x, y, z);
		vertices[i].size = XMFLOAT2(3, 3);
	}
	std::array<std::uint16_t, 16> indices = {
		0, 1, 2, 3, 4, 5, 6, 7,
		8, 9, 10, 11, 12, 13, 14, 15
	};

	const UINT vbByteSize = (UINT)vertices.size() * sizeof(SpriteVertex);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

	//mesh base 
	auto geo = std::make_unique<MeshBase>();
	geo->m_name = "chuyinGeo";
	ThrowIfFailed(D3DCreateBlob(vbByteSize, &geo->m_vertexBufferCPU));
	CopyMemory(geo->m_vertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);
	ThrowIfFailed(D3DCreateBlob(ibByteSize, &geo->m_indexBufferCPU));
	CopyMemory(geo->m_indexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);
	// 
	geo->m_vertexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), vertices.data(), vbByteSize, geo->m_vertexBufferUploader);
	geo->m_indexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), indices.data(), ibByteSize, geo->m_indexBufferUploader);

	//Create geometry information
	geo->m_vertexByteStride = sizeof(SpriteVertex);
	geo->m_vertexBufferByteSize = vbByteSize;
	geo->m_indexFormat = DXGI_FORMAT_R16_UINT;
	geo->m_indexBufferByteSize = ibByteSize;

	//Build Submesh Render
	ISubMesh submesh;
	submesh.m_indexCount = (UINT)indices.size();
	submesh.m_startIndexLocation = 0;
	submesh.m_baseVertexLocation = 0;

	geo->m_submeshes["chuyinGeo"] = submesh;

	m_geometries["chuyinGeo"] = std::move(geo);
}

void BoxApplication::DrawRenderItems(ID3D12GraphicsCommandList* cmdList, const std::vector<RenderItem*>& rItems)
{
	UINT objCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));
	UINT matCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(MaterialConstants));
	
	auto objectCB = m_currentFrameResource->m_objectCB->GetResource(); //Current Frame Constant Buffer
	auto matCB = m_currentFrameResource->m_materialCB->GetResource();
	
	for (int i = 0; i < rItems.size(); ++i) {
		auto ri = rItems[i];
		//Load Geometry
		cmdList->IASetVertexBuffers(0, 1, &ri->geo->VertexBufferView());
		cmdList->IASetIndexBuffer(&ri->geo->IndexBufferView());
		cmdList->IASetPrimitiveTopology(ri->m_primitiveType);

		CD3DX12_GPU_DESCRIPTOR_HANDLE tex(mSrvHeap->GetGPUDescriptorHandleForHeapStart());
		tex.Offset(ri->mat->m_diffuseSRVHeapIndex, mCbvSrvUavDescriptorSize);

		//Load Target Graphics Root Constant Buffer View
		D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = objectCB->GetGPUVirtualAddress() + ri->m_objectCBIndex * objCBByteSize;
		D3D12_GPU_VIRTUAL_ADDRESS matCBAddress = matCB->GetGPUVirtualAddress() + ri->mat->m_matCBIndex * matCBByteSize;
		
		cmdList->SetGraphicsRootDescriptorTable(0, tex);
		cmdList->SetGraphicsRootConstantBufferView(1, objCBAddress);
		cmdList->SetGraphicsRootConstantBufferView(3, matCBAddress);

		cmdList->DrawIndexedInstanced(ri->m_indexCount, 1, ri->m_startIndexLocation, ri->m_baseVertexLocation, 0);
	}
}

void BoxApplication::UpdateMainPassCB(const SilverEngineLib::SGGeneralTimer& timer)
{
	XMMATRIX view = XMLoadFloat4x4(&mView);
	XMMATRIX proj = XMLoadFloat4x4(&mProj);
	XMMATRIX viewProj = XMMatrixMultiply(view, proj);
	XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(view), view);
	XMMATRIX invProj = XMMatrixInverse(&XMMatrixDeterminant(proj), proj);
	XMMATRIX invViewProj = XMMatrixInverse(&XMMatrixDeterminant(viewProj), viewProj);
	XMStoreFloat4x4(&m_mainPassConstants.m_view, XMMatrixTranspose(view));
	XMStoreFloat4x4(&m_mainPassConstants.m_proj, XMMatrixTranspose(proj));
	XMStoreFloat4x4(&m_mainPassConstants.m_viewProj, XMMatrixTranspose(viewProj));
	XMStoreFloat4x4(&m_mainPassConstants.m_invView, XMMatrixTranspose(invView));
	XMStoreFloat4x4(&m_mainPassConstants.m_invProj, XMMatrixTranspose(invProj));
	XMStoreFloat4x4(&m_mainPassConstants.m_invViewProj, XMMatrixTranspose(invViewProj));
	m_mainPassConstants.m_eyePosW = mEyePos;
	m_mainPassConstants.m_renderTargetSize = XMFLOAT2((float)mClientWidth, (float)mClientHeight);
	m_mainPassConstants.m_renderTargetSize = XMFLOAT2(1.0f/(float)mClientWidth, 1.0f/(float)mClientHeight);
	m_mainPassConstants.m_nearZ = 1.0f;
	m_mainPassConstants.m_farZ = 1000.0f;
	m_mainPassConstants.m_totalTime = timer.TotalTime();
	m_mainPassConstants.m_deltaTime = timer.DeltaTime();
	m_mainPassConstants.m_ambientLight = { 0.25f, 0.25f, 0.35f, 1.0f };
	m_mainPassConstants.m_lights[0].m_direction = { -1, -1, -1 };
	m_mainPassConstants.m_lights[0].m_strength = { 0.6f, 0.6f, 0.6f };
	m_mainPassConstants.m_lights[1].m_direction = { -1, -1, -1 };
	m_mainPassConstants.m_lights[1].m_strength = { 0.3f, 0.3f, 0.3f };
	m_mainPassConstants.m_lights[2].m_direction = { -1, -1, -1 };
	m_mainPassConstants.m_lights[2].m_strength = { 0.15f, 0.15f, 0.15f };
	auto currentPassCB = m_currentFrameResource->m_passCB.get();
	currentPassCB->CopyData(0, m_mainPassConstants);
}

float angle = 0.0f;

void BoxApplication::UpdateObjectCB(const SilverEngineLib::SGGeneralTimer& timer)
{
	//load the matrix from render items to the current frame resources
	auto currObjectCB = m_currentFrameResource->m_objectCB.get();
	for (auto& e : m_renderItems) {
		//if (e->m_numFrameDirty > 0) { //This object need to update
			XMMATRIX world = XMLoadFloat4x4(&e->m_world);
			XMMATRIX texTransform = XMLoadFloat4x4(&e->m_texTransform);

			angle += timer.DeltaTime() * 0.3f;
			if (angle > 360) angle -= 360;
			XMMATRIX rotateMat = XMMatrixRotationY(angle);
			world = XMMatrixMultiply(world, rotateMat);

			//store object constants
			ObjectConstants objConstants;
			XMStoreFloat4x4(&objConstants.world, XMMatrixTranspose(world));
			XMStoreFloat4x4(&objConstants.texTransform, XMMatrixTranspose(texTransform));

			currObjectCB->CopyData(e->m_objectCBIndex, objConstants);
		//	e->m_numFrameDirty--;//update 
		//}
	}
}

//Update Material Constant Buffer
void BoxApplication::UpdateMaterialCB(const SilverEngineLib::SGGeneralTimer& timer)
{
	auto currMaterialCB = m_currentFrameResource->m_materialCB.get();
	for (auto& materialItem : m_materials) {
		SGDX12::Material* mat = materialItem.second.get();
		//need update
		if (mat->m_numFramesDirty > 0) {
			XMMATRIX matTransform = XMLoadFloat4x4(&mat->matTransform);
			SGDX12::MaterialConstants matConstants;
			matConstants.diffuseAlbedo = mat->m_diffuseAlbedo;
			matConstants.fresnelR0 = mat->m_fresnelR0;
			matConstants.roughness = mat->roughness;
			XMStoreFloat4x4(&matConstants.matTransform, XMMatrixTranspose(matTransform));
			currMaterialCB->CopyData(mat->m_matCBIndex, matConstants);
			mat->m_numFramesDirty--;
		}
	}
}

void BoxApplication::UpdateCamera(const SilverEngineLib::SGGeneralTimer& timer)
{
	mEyePos.x = mRadius * sinf(mPhi) * cosf(mTheta);
	mEyePos.z = mRadius * sinf(mPhi) * sinf(mTheta);
	mEyePos.y = mRadius * cosf(mPhi);
	
	// Build the view matrix.
	XMVECTOR pos = XMVectorSet(mEyePos.x, mEyePos.y, mEyePos.z, 1.0f);
	XMVECTOR target = XMVectorZero();
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMMATRIX view = XMMatrixLookAtLH(pos, target, up);
	XMStoreFloat4x4(&mView, view);
}

void BoxApplication::BuildPSO()
{
	//Origin Shader PSO
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	psoDesc.InputLayout = { mInputLayout.data(), (UINT)mInputLayout.size() };
	psoDesc.pRootSignature = mRootSignature.Get();
	psoDesc.VS =
	{
		reinterpret_cast<BYTE*>(m_shaderCode["normalVS"]->GetBufferPointer()),
		m_shaderCode["normalVS"]->GetBufferSize()
	};
	psoDesc.PS =
	{
		reinterpret_cast<BYTE*>(m_shaderCode["normalPS"]->GetBufferPointer()),
		m_shaderCode["normalPS"]->GetBufferSize()
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
	ThrowIfFailed(md3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&mPSOs["opaque"])));

	//PSO with Wireframe
	D3D12_GRAPHICS_PIPELINE_STATE_DESC wireFramePSODesc = psoDesc;
	wireFramePSODesc.RasterizerState.FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_WIREFRAME;
	ThrowIfFailed(md3dDevice->CreateGraphicsPipelineState(&wireFramePSODesc, IID_PPV_ARGS(&mPSOs["wire"])));

	//PSO with blend test
	D3D12_GRAPHICS_PIPELINE_STATE_DESC transportPSODesc = psoDesc;
	D3D12_RENDER_TARGET_BLEND_DESC transparencyBlendDesc;
	transparencyBlendDesc.BlendEnable = true;
	transparencyBlendDesc.LogicOpEnable = false;
	transparencyBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	transparencyBlendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	transparencyBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
	transparencyBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	transparencyBlendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
	transparencyBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	transparencyBlendDesc.LogicOp = D3D12_LOGIC_OP_NOOP;
	transparencyBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	transportPSODesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	transportPSODesc.BlendState.AlphaToCoverageEnable = true;
	transportPSODesc.BlendState.RenderTarget[0] = transparencyBlendDesc;
	ThrowIfFailed(md3dDevice->CreateGraphicsPipelineState(&transportPSODesc, IID_PPV_ARGS(&mPSOs["trans"])));

	//PSO with alpha test
	D3D12_GRAPHICS_PIPELINE_STATE_DESC alphaTestPSO = psoDesc;
	alphaTestPSO.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	ThrowIfFailed(md3dDevice->CreateGraphicsPipelineState(&alphaTestPSO, IID_PPV_ARGS(&mPSOs["alphaTested"])));

	//PSO with sprite
	D3D12_GRAPHICS_PIPELINE_STATE_DESC spritePSO = psoDesc;
	spritePSO.VS =
	{
		reinterpret_cast<BYTE*>(m_shaderCode["spriteVS"]->GetBufferPointer()),
		m_shaderCode["spriteVS"]->GetBufferSize()
	};
	spritePSO.GS =
	{
		reinterpret_cast<BYTE*>(m_shaderCode["spriteGS"]->GetBufferPointer()),
		m_shaderCode["spriteGS"]->GetBufferSize()
	};
	spritePSO.PS =
	{
		reinterpret_cast<BYTE*>(m_shaderCode["spritePS"]->GetBufferPointer()),
		m_shaderCode["spritePS"]->GetBufferSize()
	};
	spritePSO.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	spritePSO.InputLayout = { mSpriteLayout.data(), (UINT)mSpriteLayout.size() };
	spritePSO.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	ThrowIfFailed(md3dDevice->CreateGraphicsPipelineState(&spritePSO, IID_PPV_ARGS(&mPSOs["sprite"])));
}

void BoxApplication::Update(const SilverEngineLib::SGGeneralTimer& timer)
{
	//Exchange current frame resource
	m_currentFrameResourceIndex = (m_currentFrameResourceIndex + 1) % 3;
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
	UpdateMaterialCB(timer);
	UpdateCamera(timer);
}

void BoxApplication::Render(const SilverEngineLib::SGGeneralTimer& timer)
{
	//Reset Current CmdListAllocator 
	auto cmdListAlloc = m_currentFrameResource->m_commandListAllocator;	
	ThrowIfFailed(cmdListAlloc->Reset());

	if (!m_wireFrameRenderState) {
		ThrowIfFailed(mCommandList->Reset(cmdListAlloc.Get(), mPSOs["trans"].Get()));
	}
	else
	{
		ThrowIfFailed(mCommandList->Reset(cmdListAlloc.Get(), mPSOs["wire"].Get()));
	}

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

	//Bind the descriptor heaps
	ID3D12DescriptorHeap* descriptorHeaps[] = { mSrvHeap.Get() };
	mCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	//Set the Root Signature
	mCommandList->SetGraphicsRootSignature(mRootSignature.Get());

	//Update Pass Constant Buffer
	auto passCB = m_currentFrameResource->m_passCB->GetResource();
	mCommandList->SetGraphicsRootConstantBufferView(2, passCB->GetGPUVirtualAddress());

	mCommandList->SetPipelineState(mPSOs["trans"].Get());
	DrawRenderItems(mCommandList.Get(), m_renderItemLayer[(int)RenderLayer::Opaque]);

	mCommandList->SetPipelineState(mPSOs["sprite"].Get());
	DrawRenderItems(mCommandList.Get(), m_renderItemLayer[(int)RenderLayer::Sprite]);


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

	//current frame resource's fence update
	//complete one frame's calculation
	m_currentFrameResource->m_fence = ++mCurrentFence; 

	//fence flush command queue
	mCommandQueue->Signal(mFence.Get(), mCurrentFence);
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

std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> BoxApplication::GetStaticSamplers()
{
	const CD3DX12_STATIC_SAMPLER_DESC pointWrap(
		0,
		D3D12_FILTER_MIN_MAG_MIP_POINT,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP
	);
	const CD3DX12_STATIC_SAMPLER_DESC pointClamp(
		1,
		D3D12_FILTER_MIN_MAG_MIP_POINT,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP
	);
	const CD3DX12_STATIC_SAMPLER_DESC linearWrap(
		2,
		D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP
	);
	const CD3DX12_STATIC_SAMPLER_DESC linearClamp(
		3,
		D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP
	);
	const CD3DX12_STATIC_SAMPLER_DESC anisotropicWrap(
		4,
		D3D12_FILTER_ANISOTROPIC,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP
	);
	const CD3DX12_STATIC_SAMPLER_DESC anisotropicClamp(
		5,
		D3D12_FILTER_ANISOTROPIC,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP
	);

	return {
		pointWrap, pointClamp,
		linearWrap, linearClamp,
		anisotropicWrap, anisotropicClamp
	};
}

void BoxApplication::OnResize()
{
	IApplication::OnResize();
	XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f * MathHelper::Pi, m_state.AspectRatio(), 1.0f, 1000.0f);
	XMStoreFloat4x4(&mProj, P);
}
