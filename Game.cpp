#include "Game.h"
#include "Circe.h"
#include "MeshComponent.h"
#include "MeshComponentConstantBufffer.h"
#include "Labaratory_one_params.h"
#include "Lab2Params.h"

static uint16_t counter = 0;
std::vector<Circe*> circles;
std::vector<MeshComponent*> components;
std::vector<MeshComponentConstantBuffer*> constComponents;
std::chrono::time_point<std::chrono::steady_clock> prevTime;
std::chrono::time_point<std::chrono::steady_clock> lastFixedUpdateTime;
uint32_t objectsExist = 0;

Game::Game() noexcept {}
Game::~Game() noexcept {}

struct ConstData
{
    XMFLOAT4 offset;
    XMFLOAT4 color;
};

void Game::Exit() noexcept
{
    g_IsContinue = false;   
}

void Game::Run() noexcept
{
    g_TotalTime = 0;
    g_FrameCount = 0;

    lastFixedUpdateTime = std::chrono::steady_clock::now();
    prevTime = std::chrono::steady_clock::now();

    while (g_IsContinue)
    {
        const auto time = std::chrono::steady_clock::now();

        if (std::chrono::duration<double>(time - lastFixedUpdateTime).count() >= UPDATE_DELAY)
        {
            lastFixedUpdateTime = time;
            FixedUpdate();
        }
        Update();
    }  
    PostQuitMessage(0);
}

void Game::Update() noexcept
{  
    HandleMsg();
    PrepareFrames();
    Draw();
    EndFrame();
}

void Game::FixedUpdate()
{
    UpdateInternal();
}

void Game::EndFrame() noexcept
{
    const auto	curTime = std::chrono::steady_clock::now();
    const float deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(curTime - prevTime).count() / 1000000.0f;
    prevTime = curTime;
    g_TotalTime += deltaTime;
    g_FrameCount++;

    if (g_TotalTime >= 1.0f) {
        float fps = g_FrameCount / g_TotalTime;

        g_TotalTime = 0.0f;

        WCHAR text[256];
        swprintf_s(text, TEXT("FPS: %f"), fps);
        SetWindowText(g_Intance->g_HWnd, text);

        g_FrameCount = 0;
    }
}

void Game::Initialize(uint32_t mode) noexcept
{
    g_Intance = new DisplayWin32(this);
    g_Mode = mode;
    g_IsContinue = new bool(true);
    D3D_FEATURE_LEVEL g_FeatureLevel[] = { D3D_FEATURE_LEVEL_11_1 };

    DXGI_SWAP_CHAIN_DESC swapDesc = {};
    swapDesc.BufferCount = 2;
    swapDesc.BufferDesc.Width = WINDOW_WIDTH;
    swapDesc.BufferDesc.Height = WINDOW_HEIGHT;
    swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapDesc.BufferDesc.RefreshRate.Numerator = 200;
    swapDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapDesc.OutputWindow = g_Intance->g_HWnd;
    swapDesc.Windowed = true;
    swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    swapDesc.SampleDesc.Count = 1;
    swapDesc.SampleDesc.Quality = 0;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        D3D11_CREATE_DEVICE_DEBUG,
        g_FeatureLevel,
        1,
        D3D11_SDK_VERSION,
        &swapDesc,
        &g_SwapChain,
        &g_Device,
        nullptr,
        &g_Context
    );
}


std::vector<XMFLOAT4> DrawCircle()
{
    std::vector<XMFLOAT4> vertices;

    float angleStep = 1.0f / 360;

    for (uint16_t i = 0; i < 360; ++i)
    {
        float angle = angleStep * i;

        float x = 360 * cos(angle);
        float y = 360 * sin(angle);

        vertices.push_back(XMFLOAT4(x, y, 0.0f, 1.0f));
    }

    return vertices;
}

void Game::PrepareResources(uint32_t objectsAmount) noexcept
{
    auto res = g_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backTex);	// __uuidof(ID3D11Texture2D)

    g_Device->CreateRenderTargetView(backTex, nullptr, &g_RenderTargetView);

    CD3D11_RASTERIZER_DESC rastDesc = {};
    rastDesc.CullMode = D3D11_CULL_NONE;
    rastDesc.FillMode = D3D11_FILL_SOLID  /*D3D11_FILL_WIREFRAME*/;

    g_Device->CreateRasterizerState(&rastDesc, &g_RastState);

    g_Context->RSSetState(g_RastState);

    ShaderManager* shM = new ShaderManager();
    vector<std::string> paths = { "../ComputerGraphics/" };

    shM->CompileShaders(paths);

    const auto pixelBCs = shM->GetPixelShaderBC();
    const auto vertexBCs = shM->GetVertexShadersBC();

    for (const auto& shader : pixelBCs)
    {
        ID3D11PixelShader* tempPS;

        auto hr = g_Device->CreatePixelShader(
            shader->GetBufferPointer(),
            shader->GetBufferSize(),
            nullptr,
            &tempPS
        );

        g_PixelShaders.push_back(tempPS);
    }

    for (const auto& shader : vertexBCs)
    {
        ID3D11VertexShader* tempVS;

        auto result = g_Device->CreateVertexShader(
            shader->GetBufferPointer(),
            shader->GetBufferSize(),
            nullptr,
            &tempVS
        );

        g_VertexShaders.push_back(tempVS);
    }

    UINT strides[] = { 32 };
    UINT offsets[] = { 0 };

    DirectX::XMFLOAT4* points;

    if (g_Mode == LABARATORY_MODE::Architecture)
    {
        std::vector<XMFLOAT4> circleVertices = DrawCircle();

        // Здесь вы должны создать буфер вершин и загрузить в него circleVertices
        // Затем выполнить отрисовку с использованием контекста Direct3D

        // Пример кода для создания буфера вершин (не полный)

        
        const auto circle = new Circe(this, 0);
        circles.push_back(circle);
        UINT stride = sizeof(XMFLOAT4);
        UINT offset = 0;
       circles[0]->Initialize(g_VertexShaders[0], g_PixelShaders[0], strides, offsets, pixelBCs[0], vertexBCs[0], circleVertices);

        // Установка буфера вершин
        
        // Освобождение ресурсов
    }

    if (g_Mode == 3) {

        DirectX::XMFLOAT4* points = OBJECT_POINTS;

        float_t posModifier = 0.05f;

        for (uint32_t i = 0; i < objectsAmount; i++)
        {
            const auto component = new MeshComponent(this, i);
            components.push_back(component);
            components[i]->Initialize(g_VertexShaders[0], g_PixelShaders[0], points, strides, offsets, pixelBCs[0], vertexBCs[0]);

            points[0].x += posModifier;
            points[1].y += posModifier;
            points[2].z += posModifier;
            points[3].x += posModifier;
            points[4].y += posModifier;
            points[5].z += posModifier;
            points[6].x += posModifier;
            points[7].y += posModifier;
        }

        objectsExist = objectsAmount;
        }
        else if(g_Mode == 2)
        {
            DirectX::XMFLOAT4* points = DOT_POINTS;
            for (uint16_t i = 0; i < QUADS_IN_GRID; i++)
            {
                points[0].y += QUADS_IN_GRID_POS_OFFSET;
                points[1].y += QUADS_IN_GRID_POS_OFFSET;
                points[2].y += QUADS_IN_GRID_POS_OFFSET;
                points[3].y += QUADS_IN_GRID_POS_OFFSET;

                auto component = new MeshComponentConstantBuffer(this);
                component->Initialize(g_VertexShaders[1], g_PixelShaders[1], points, pixelBCs[1], vertexBCs[1], true, counter);
                constComponents.push_back(component);
                counter++;
            }

            points = PLAYERS_START_POINTS;
            for (uint16_t i = 0; i < 2; i++)
            {
                auto component = new MeshComponentConstantBuffer(this);           
                component->Initialize(g_VertexShaders[1], g_PixelShaders[1], points, pixelBCs[1], vertexBCs[1], false, counter);
                constComponents.push_back(component);

                points[0].x += PLAYER_POS_OFFSET;
                points[1].x += PLAYER_POS_OFFSET;
                points[2].x += PLAYER_POS_OFFSET;
                points[3].x += PLAYER_POS_OFFSET;
                counter++;
            }

            points = SQUARE_POINTS;
            auto component = new MeshComponentConstantBuffer(this);
            component->Initialize(g_VertexShaders[1], g_PixelShaders[1], points, pixelBCs[1], vertexBCs[1], false, counter);
            constComponents.push_back(component);
            counter++;
    }
}


void Game::PrepareFrames() const noexcept
{
    g_Context->ClearState();

    g_Context->RSSetState(g_RastState);

    D3D11_VIEWPORT viewport = {};
    viewport.Width = static_cast<float>(WINDOW_WIDTH);
    viewport.Height = static_cast<float>(WINDOW_HEIGHT);
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.MinDepth = 0;
    viewport.MaxDepth = 1.0f;

    constexpr float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };

    g_Context->ClearRenderTargetView(g_RenderTargetView, color);

    g_Context->OMSetRenderTargets(1, &g_RenderTargetView, nullptr);   

    g_Context->RSSetViewports(1, &viewport);
}

void Game::Draw() const noexcept
{
    for (const auto& component : components)
    {
        g_Context->OMSetRenderTargets(1, &g_RenderTargetView, nullptr);
        component->Update();
    }

    for (const auto& component : constComponents)
    {
        g_Context->OMSetRenderTargets(1, &g_RenderTargetView, nullptr);
        component->Update(COLORS["White"]);
    }

    for (const auto& component : circles)
    {
        std::cout << "Update" << std::endl;
        g_Context->OMSetRenderTargets(1, &g_RenderTargetView, nullptr);
        component->Update();
    }

    //g_Context->OMSetRenderTargets(0, nullptr, nullptr);
    g_SwapChain->Present(1, DXGI_PRESENT_DO_NOT_WAIT);
}

void Game::CreateBackBuffer() noexcept
{

}

void Game::RestoreTargets() noexcept
{

}

void Game::DestroyResources() noexcept
{

}

void Game::UpdateInternal() noexcept
{
    for (const auto& component : constComponents)
    {
       component->FixedUpdate({ 0.005f, 0.000f, 0.0f, 0.0f });

       for (const auto& other : constComponents)
       {
           if (component->msh_UseCollider && other->msh_UseCollider && component != other)
           {
               if (component->msh_Box.Intersects(other->msh_Box))
               {
                   //std::cout << "collision!" << std::endl; 
               }
           }
       }
    }
}

void Game::HandleMsg() noexcept
{
    MSG msg = {};

    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if (msg.message == WM_QUIT)
    {
        Exit();
    }
}

void Game::HandleInput(unsigned key) noexcept
{
    if (key == 87)
    {

        std::cout << "Up: " << key << std::endl;
    }

    if (key == 83)
    {
        std::cout << "Down: " << key << std::endl;
    }

    if (key == 38)
    {
        std::cout << "Up: " << key << std::endl;
    }

    if (key == 40)
    {
        std::cout << "Down: " << key << std::endl;
    }
}