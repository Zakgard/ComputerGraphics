#pragma once

#include "Helpers.h"
#include "GameComponent.h"

#include <DirectXMath.h>

using namespace DirectX;

class MeshComponentConstantBuffer : public GameComponent
{
public:
	MeshComponentConstantBuffer() noexcept;
	MeshComponentConstantBuffer(const Game* game) noexcept;
	virtual ~MeshComponentConstantBuffer() = default;
	void Update(XMFLOAT4 color) noexcept;
	void FixedUpdate(XMFLOAT4 offset) noexcept;
	XMFLOAT4 GetCurrentPos() noexcept;
	void Initialize(ID3D11VertexShader* const vSh, ID3D11PixelShader* const pSh, DirectX::XMFLOAT4 basePoints[], ID3DBlob* pixelBC, ID3DBlob* vertexBC, bool IsStatic) noexcept;
	MeshComponentConstantBuffer* GetMeshComponent() noexcept;
	virtual void DestroyComponentDesources() noexcept override;
private:
	ID3D11InputLayout* msh_Layout;
	ID3D11PixelShader* msh_PixelShader;
	ID3DBlob* msh_PixelShaderByteCode;
	size_t msh_ByteSize = 0;
	ID3DBlob* msh_VertexShaderByteCode;

	ID3D11VertexShader* msh_VertexShader;
	ID3D11RasterizerState* msh_rastState;

	XMFLOAT4* msh_Points;
	XMFLOAT4 msh_Pos;

	const Game* m_game;

	ID3D11Buffer* m_Indexbuffer;
	ID3D11Buffer* m_ConstantBuffer;
	ID3D11Buffer* m_VertexBuffer;

	UINT* msh_Strides;
	UINT* msh_Offsets;
};