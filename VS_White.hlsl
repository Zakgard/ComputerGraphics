cbuffer ConstantBuffer : register(b0)
{
    matrix world; // Мировая матрица
    matrix view; // Матрица вида
    matrix projection; // Матрица проекции
};

struct VS_INPUT
{
    float4 pos : POSITION; // Позиция вершины
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION; // Позиция вершины в пространстве экрана
};

VS_OUTPUT VSMain(VS_INPUT input)
{
    VS_OUTPUT output;
    output.pos = mul(input.pos, world); // Применение мировой матрицы
    output.pos = mul(output.pos, view); // Применение матрицы вида
    output.pos = mul(output.pos, projection); // Применение матрицы проекции
    return output;
}

struct PS_INPUT
{
    float4 pos : SV_POSITION; // Позиция пикселя в пространстве экрана
};

float4 PSMain(PS_INPUT input) : SV_TARGET
{
    return float4(1.0f, 1.0f, 1.0f, 1.0f); // Белый цвет (RGBA: 1, 1, 1, 1)
}