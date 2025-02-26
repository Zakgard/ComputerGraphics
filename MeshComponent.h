#pragma once

#include "Helpers.h"
#include "GameComponent.h"

class MeshComponent : public GameComponent
{
public:
	MeshComponent() noexcept;
	MeshComponent(const Game* game, uint32_t index) noexcept;
	virtual ~MeshComponent() = default;
	void Update() noexcept override;
	void FixedUpdate() noexcept {};
	void Initialize(ID3D11VertexShader* const vSh, ID3D11PixelShader* const pSh, DirectX::XMFLOAT4 basePoints[], UINT strides[], UINT offsets[],
		ID3DBlob* pixelBC, ID3DBlob* vertexBC) noexcept;
	
	MeshComponent* GetMeshComponent() noexcept;
	virtual void DestroyComponentDesources() noexcept override;
private:
	ID3D11InputLayout* msh_Layout;
	ID3D11PixelShader* msh_PixelShader;
	ID3DBlob* msh_PixelShaderByteCode;
	size_t msh_ByteSize = 0;
	ID3DBlob* msh_VertexShaderByteCode;

	ID3D11VertexShader* msh_VertexShader;
	ID3D11RasterizerState* msh_rastState;

	DirectX::XMFLOAT4* msh_Points;

	const Game* m_game;

	ID3D11Buffer* m_Indexbuffer;
	ID3D11Buffer* m_VertexBuffer;

	UINT* msh_Strides;
	UINT* msh_Offsets;
};
