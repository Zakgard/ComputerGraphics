struct VS_IN
{
    float4 pos : POSITION;
    float4 col : COLOR0;
};

struct PS_IN
{
    float4 pos : SV_POSITION0;
    float4 col : COLOR0;
};

struct Addition
{
    float4 offset;
    float4 color;
};

cbuffer AdditionBuffer : register(b0)
{
    Addition AdditionData;
}

PS_IN VSMain(VS_IN input)
{
    PS_IN output;
	
    output.pos = float4(input.pos.xyz + AdditionData.offset.xyz, 1.0);
    output.col = AdditionData.color;
    return output;
}

float4 PSMain(PS_IN input) : SV_Target
{
    float4 col = input.col;
    return col;
}