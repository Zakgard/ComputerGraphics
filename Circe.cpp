#include "Circe.h"

#include <cstring>
#include <DirectXMath.h>

using namespace DirectX;

struct ConstantBuffer {
    float width;
    float height;
};

Circe::Circe() noexcept
{

}

Circe::Circe(const Game* game, uint32_t index) noexcept
{
	m_game = game;
}


void Circe::Initialize(ID3D11VertexShader* const vSh, ID3D11PixelShader* const pSh, UINT strides[], UINT offsets[], ID3DBlob* pixelBC, ID3DBlob* vertexBC, std::vector<XMFLOAT4> vecotr) noexcept
{
    const auto strideSize = sizeof(strides) / sizeof(strides[0]);
    msh_Strides = new UINT[strideSize];
    std::memcpy(msh_Strides, strides, strideSize * sizeof(UINT));

    const auto offsetSize = sizeof(offsets) / sizeof(offsets[0]);
    msh_Offsets = new UINT[offsetSize];
    std::memcpy(msh_Offsets, offsets, offsetSize * sizeof(UINT));
    bbb = vecotr;
    msh_VertexShader = vSh;
    msh_PixelShader = pSh;
    msh_PixelShaderByteCode = pixelBC;
    msh_VertexShaderByteCode = vertexBC;


    const int segments = 360;
    const float radius = 1.0f;
    std::vector<XMFLOAT4> vertices;
    std::vector<UINT> indices;

    
    vertices.push_back(XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

    for (int i = 0; i <= segments; ++i) {
        float angle = XM_PI * 2.1f * i / segments;
        float x = radius * 1.0f * cos(angle);
        float y = radius * sin(angle);
        vertices.push_back(XMFLOAT4(x, y, 0.0f, 1.0f));
    }

    for (UINT i = 1; i <= segments; ++i) {
        indices.push_back(0);
        indices.push_back(i); 
        indices.push_back(i % segments + 1); 
    }


    D3D11_BUFFER_DESC vertexBufDesc = {};
    vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufDesc.CPUAccessFlags = 0;
    vertexBufDesc.MiscFlags = 0;
    vertexBufDesc.StructureByteStride = 0;
    vertexBufDesc.ByteWidth = sizeof(XMFLOAT4) * 360;

    D3D11_SUBRESOURCE_DATA vertexData = {};
    vertexData.pSysMem = vertices.data();

    m_game->g_Device->CreateBuffer(&vertexBufDesc, &vertexData, &m_VertexBuffer);


    D3D11_BUFFER_DESC indexBufDesc = {};
    indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufDesc.CPUAccessFlags = 0;
    indexBufDesc.MiscFlags = 0;
    indexBufDesc.StructureByteStride = 0;
    indexBufDesc.ByteWidth = sizeof(UINT) * indices.size();

    D3D11_SUBRESOURCE_DATA indexData = {};
    indexData.pSysMem = indices.data();

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
        { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    m_game->g_Device->CreateInputLayout(
        inputElements,
        2,
        msh_VertexShaderByteCode->GetBufferPointer(),
        msh_VertexShaderByteCode->GetBufferSize(),
        &msh_Layout
    );

    size = indices.size();
}

void Circe::DestroyComponentDesources() noexcept {

}

void Circe::Update() noexcept {
    RECT clientRect;

    GetClientRect(m_game->g_Intance->g_HWnd, &clientRect);

    int width = clientRect.right - clientRect.left;
    int height = clientRect.bottom - clientRect.top;

    auto w = static_cast<float>(width);
    auto h = static_cast<float>(height);

    ConstantBuffer cb{ width, height };

    auto newX = w > h ? h  / w : w / h;
    auto newY = h > w ? w / h : h / w;

    D3D11_MAPPED_SUBRESOURCE mappedResource = {};

    m_game->g_Context->Map(m_ConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    memcpy(mappedResource.pData, &cb, sizeof(ConstantBuffer));
    m_game->g_Context->Unmap(m_ConstantBuffer, 0);

    m_game->g_Context->IASetInputLayout(msh_Layout);
    m_game->g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    m_game->g_Context->VSSetConstantBuffers(0, 1, &m_ConstantBuffer);

    m_game->g_Context->IASetIndexBuffer(m_Indexbuffer, DXGI_FORMAT_R32_UINT, 0);
    m_game->g_Context->IASetVertexBuffers(0, 1, &m_VertexBuffer, msh_Strides, msh_Offsets);


    m_game->g_Context->VSSetShader(msh_VertexShader, nullptr, 0);
    m_game->g_Context->PSSetShader(msh_PixelShader, nullptr, 0);


    m_game->g_Context->DrawIndexed(size, 0, 0);
}

Circe* Circe::GetMeshComponent() noexcept {
    return this;
}