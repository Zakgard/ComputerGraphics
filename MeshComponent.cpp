#include "MeshComponent.h"

#include <cstring>
#include <DirectXMath.h>

using namespace DirectX;

uint32_t objectIndex;

MeshComponent::MeshComponent() noexcept
{

}

MeshComponent::MeshComponent(const Game* game, uint32_t index) noexcept
{
	m_game = game;
	objectIndex = index;
}


void MeshComponent::Initialize(ID3D11VertexShader* const vSh, ID3D11PixelShader* const pSh, DirectX::XMFLOAT4 basePoints[], UINT strides[], UINT offsets[], ID3DBlob* pixelBC, ID3DBlob* vertexBC) noexcept
{
	const auto strideSize = sizeof(strides) / sizeof(strides[0]);
	msh_Strides = new UINT[strideSize];

	std::memcpy(msh_Strides, strides, strideSize * sizeof(UINT));


	const auto offsetSize = sizeof(offsets) / sizeof(offsets[0]);
	msh_Offsets = new UINT[offsetSize];

	std::memcpy(msh_Offsets, offsets, offsetSize * sizeof(UINT));

	msh_VertexShader = vSh;
	msh_PixelShader = pSh;
	msh_Points = basePoints;
	msh_PixelShaderByteCode = pixelBC;
	msh_VertexShaderByteCode = vertexBC;
	
	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = 0;
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;
	vertexBufDesc.ByteWidth = sizeof(DirectX::XMFLOAT4) * 8;

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = msh_Points;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	m_game->g_Device->CreateBuffer(&vertexBufDesc, &vertexData, &m_VertexBuffer);

	int indeces[] = 
		{ 0,1,2, 
		1,0,3 };

	D3D11_BUFFER_DESC indexBufDesc = {};
	indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufDesc.CPUAccessFlags = 0;
	indexBufDesc.MiscFlags = 0;
	indexBufDesc.StructureByteStride = 0;
	indexBufDesc.ByteWidth = sizeof(int) * 6;

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = indeces;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	m_game->g_Device->CreateBuffer(&indexBufDesc, &indexData, &m_Indexbuffer);

	D3D11_INPUT_ELEMENT_DESC inputElements[] = {
	D3D11_INPUT_ELEMENT_DESC {
		"POSITION",
		0,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		0,
		0,
		D3D11_INPUT_PER_VERTEX_DATA,
		0},
	D3D11_INPUT_ELEMENT_DESC {
		"COLOR",
		0,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		0,
		D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA,
		0}
	};

	m_game->g_Device->CreateInputLayout(
		inputElements,
		2,
		msh_VertexShaderByteCode->GetBufferPointer(),
		msh_VertexShaderByteCode->GetBufferSize(),
		&msh_Layout
	);
}

void MeshComponent::DestroyComponentDesources() noexcept
{
	//delete this->msh_VertexShader;
}

void MeshComponent::Update() noexcept
{
	m_game->g_Context->IASetInputLayout(msh_Layout);
	m_game->g_Context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_game->g_Context->IASetIndexBuffer(m_Indexbuffer, DXGI_FORMAT_R32_UINT, 0);
	m_game->g_Context->IASetVertexBuffers(0, 1, &m_VertexBuffer, msh_Strides, msh_Offsets);

	m_game->g_Context->VSSetShader(msh_VertexShader, nullptr, 0);
	m_game->g_Context->PSSetShader(msh_PixelShader, nullptr, 0);

	m_game->g_Context->DrawIndexed(6, 0, 0);
}

MeshComponent* MeshComponent::GetMeshComponent() noexcept
{
	return this;
}