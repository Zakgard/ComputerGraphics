#include "MeshComponentConstantBufffer.h"

#include <cstring>
#include <DirectXMath.h>
#include "DirectXCollision.h"

using namespace DirectX;

struct ConstantBuffer {
	float width;
	float height;
};

MeshComponentConstantBuffer::MeshComponentConstantBuffer() noexcept
{

}

MeshComponentConstantBuffer::MeshComponentConstantBuffer(const Game* game) noexcept
{
	m_game = game;
}


void MeshComponentConstantBuffer::Initialize(ID3D11VertexShader* const vSh, ID3D11PixelShader* const pSh, DirectX::XMFLOAT4 basePoints[], ID3DBlob* pixelBC, ID3DBlob* vertexBC, bool IsStatic, uint16_t iD) noexcept
{
	msh_IsStatic = IsStatic;
	msh_UseCollider = !IsStatic;
	m_Id = iD;
	//msh_Pos.x = basePoints[0].x;

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
	0, 2, 3 };

	D3D11_BUFFER_DESC indexBufDesc = {};
	indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufDesc.CPUAccessFlags = 0;
	indexBufDesc.MiscFlags = 0;
	indexBufDesc.StructureByteStride = 0;
	indexBufDesc.ByteWidth = sizeof(int) * 8;

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = indeces;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	m_game->g_Device->CreateBuffer(&indexBufDesc, &indexData, &m_Indexbuffer);

	D3D11_BUFFER_DESC constBufDesc = {};
	constBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufDesc.MiscFlags = 0;
	constBufDesc.StructureByteStride = 0;
	constBufDesc.ByteWidth = sizeof(ConstantBuffer);

	m_game->g_Device->CreateBuffer(&constBufDesc, nullptr, &m_ConstantBuffer);

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

	msh_CenterPos.x = basePoints[1].x - basePoints[0].x;
	msh_CenterPos.y = basePoints[2].y - basePoints[0].y;
}

void MeshComponentConstantBuffer::DestroyComponentDesources() noexcept
{
	//delete this->msh_VertexShader;
}

void MeshComponentConstantBuffer::Update(XMFLOAT4 color) noexcept
{
	/*const ConstantBuffer constData{msh_Pos, color, {0,0,0,0}};
	D3D11_MAPPED_SUBRESOURCE res = {};

	m_game->g_Context->Map(m_ConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &res);

	auto dataPtr = reinterpret_cast<float*>(res.pData);

	memcpy(dataPtr, &constData, sizeof(ConstantBuffer));

	m_game->g_Context->Unmap(m_ConstantBuffer, 0);
	m_game->g_Context->VSSetConstantBuffers(0, 1, &m_ConstantBuffer);

	constexpr uint32_t strides[] = { 16 };
	constexpr uint32_t offsets[] = { 0 };

	m_game->g_Context->IASetInputLayout(msh_Layout);
	m_game->g_Context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_game->g_Context->IASetIndexBuffer(m_Indexbuffer, DXGI_FORMAT_R32_UINT, 0);
	m_game->g_Context->IASetVertexBuffers(0, 1, &m_VertexBuffer, strides, offsets);

	m_game->g_Context->VSSetShader(msh_VertexShader, nullptr, 0); 

	m_game->g_Context->PSSetShader(msh_PixelShader, nullptr, 0);

	m_game->g_Context->DrawIndexed(6, 0, 0);*/
}

MeshComponentConstantBuffer* MeshComponentConstantBuffer::GetMeshComponent() noexcept
{
	return this;
}

void MeshComponentConstantBuffer::FixedUpdate(XMFLOAT4 offset) noexcept
{
	if (!msh_IsStatic)
	{
		msh_Pos.x += offset.x;
		msh_Pos.y += offset.y;
		msh_CenterPos.x += offset.x;
		msh_CenterPos.y += offset.y;
		//msh_Pos.w += offset.w;
		//msh_Pos.z += offset.z;
	}

	if (msh_UseCollider)
	{
		msh_PosCollider = XMFLOAT3{
			msh_Pos.x,
			msh_Pos.y,
			0
		};

		msh_Box.Center = msh_CenterPos;
		std::cout << msh_CenterPos.x  << " id " << m_Id << std::endl;
		msh_Box.Extents = XMFLOAT3{
			0.01f,
			0.01f,
			0.0f
		};
	}
//	if(msh_Box.Intersects())
}

XMFLOAT4 MeshComponentConstantBuffer::GetCurrentPos() noexcept
{
	return msh_Pos;
}