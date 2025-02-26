cbuffer ConstantBuffer : register(b0)
{
    matrix world; // ������� �������
    matrix view; // ������� ����
    matrix projection; // ������� ��������
};

struct VS_INPUT
{
    float4 pos : POSITION; // ������� �������
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION; // ������� ������� � ������������ ������
};

VS_OUTPUT VSMain(VS_INPUT input)
{
    VS_OUTPUT output;
    output.pos = mul(input.pos, world); // ���������� ������� �������
    output.pos = mul(output.pos, view); // ���������� ������� ����
    output.pos = mul(output.pos, projection); // ���������� ������� ��������
    return output;
}

struct PS_INPUT
{
    float4 pos : SV_POSITION; // ������� ������� � ������������ ������
};

float4 PSMain(PS_INPUT input) : SV_TARGET
{
    return float4(1.0f, 1.0f, 1.0f, 1.0f); // ����� ���� (RGBA: 1, 1, 1, 1)
}