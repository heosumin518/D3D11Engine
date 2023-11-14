#ifndef _GLOBAL_FX_
#define _GLOBAL_FX_

/////////////////
// ConstBuffer //
/////////////////

cbuffer GlobalBuffer
{
    matrix V;
    matrix P;
    matrix VP;
};

cbuffer TransformBuffer
{
    matrix W;
};

////////////////
// VertexData //
////////////////

struct Vertex
{
    float4 position : POSITION;
};

struct VertexTexture
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
};

struct VertexColor
{
    float4 Position : POSITION;
    float4 Color : COLOR;
};

struct VertexTextureNormal
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};

//////////////////
// VertexOutput //
//////////////////

struct VertexOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};

//////////////////
// SamplerState //
//////////////////

SamplerState LinearSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

SamplerState PointSampler
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Wrap;
    AddressV = Wrap;
};

/////////////////////
// RasterizerState //
/////////////////////

RasterizerState FillModeWireFrame
{
    FillMode = WireFrame;
};

///////////
// Macro //
///////////

#define PASS_VP(name, vs, ps)						\
pass name											\
{													\
	SetVertexShader(CompileShader(vs_5_0, vs()));	\
	SetPixelShader(CompileShader(ps_5_0, ps()));	\
}

//////////////
// Function //
//////////////

#endif

