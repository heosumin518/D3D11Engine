//#pragma enable_d3d11_debug_symbols

cbuffer Transform : register(b0)
{
    matrix m_world;
    matrix m_view;
    matrix m_projection;
}

cbuffer DirectionLight : register(b1)
{
    float4 m_lightDir;
    float4 m_lightAmbient;
    float4 m_lightDiffuse;
    float4 m_lightSpecular;
    float3 m_eyePos;
    bool m_useBlinnPhong;
    float m_lightSpecularPower;
    float3 m_lightPad0;     // dummy
}

struct VS_INPUT
{
    float4 m_position : POSITION;
    float2 m_uv : TEXCOORD0;
    float3 m_normal : NORMAL;   // NORMAL �� ������ ����Ű�� �ø�ƽ. ������ 3���� �������� ������ ��Ÿ���� ����
};

struct VS_OUTPUT
{
    float4 m_position : SV_POSITION;
    float2 m_uv : TEXCOORD0;
    float3 m_diffuse : TEXCOORD1;       // �ȼ��� RGB ������ ����� ���̹Ƿ� float3�� ���.
    float3 m_viewDir : TEXCOORD2;
    float3 m_reflection : TEXCOORD3;    // TODO : half �Ǵ� reflect �����ε� �� �̸��� ���°� ������..
    float3 m_worldNormal : NORMAL;
};

