
// 라이트
struct DirectionalLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float3 Direction;
	float  pad;
};

struct PointLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;

	float3 Position;
	float  Range;

	float3 Att;
	float  pad;
};

struct SpotLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;

	float3 Position;
	float  Range;

	float3 Direction;
	float  Spot;

	float3 Att;
	float  pad;
};

// 재질
struct Material
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular; // w = Specularer
	float4 Reflect;
};

// 버텍스 종류
struct PNTVertexIn
{
	float3 PosL				  : POSITION;
	float3 NormalL			  : NORMAL;
	float2 Tex				  : TEXCOORD;
	float3 Tangent			  : TANGENT;
	float3 BiNormal			  : BINORMAL;
	row_major float4x4 World  : WORLD;
	uint InstanceId		      : SV_InstanceID;
};

struct PNTVertexAniIn
{
	float3 PosL				  : POSITION;
	float3 NormalL			  : NORMAL;
	float2 Tex				  : TEXCOORD;
	float3 Tangent			  : TANGENT;
	float3 BiNormal			  : BINORMAL;
	float3 Weights            : WEIGHTS;		 // 애니메이션 가중치
	uint4  BoneIndices        : BONEINDICES;	 // 애니메이션 영향 본 인덱스
	row_major float4x4 World  : WORLD;
	float4 AniData            : ANIDATA;		 // 애니 텍스처 번호, 현재 프레임, 텍스처 너비, 텍스처 높이
	uint InstanceId		      : SV_InstanceID;
};

struct PNTVertexAniOut
{
	float4 PosH         : SV_POSITION;
	float3 PosW         : POSITION0;
	float3 NormalW      : NORMAL;
	float2 Tex          : TEXCOORD0;   // 보정 용도
	float3 WT           : POSITION1;   // 매트릭스 만들기 용도
	float3 WB           : POSITION2;   // 매트릭스 만들기 용도
	float2 AniData      : ANIDATA;	   // 애니 텍스처 번호, 현재 프레임
};

struct PNTVertexOut
{
	float4 PosH    : SV_POSITION;
	float3 PosW    : POSITION0;
	float3 NormalW : NORMAL;
	float2 Tex     : TEXCOORD0; // 보정 용도
	float3 WT      : POSITION1; // 매트릭스 만들기 용도
	float3 WB      : POSITION2; // 매트릭스 만들기 용도
};

struct PCVertexIn
{
	float3 PosL			      : POSITION;
	float4 Color		      : COLOR;
	row_major float4x4 World  : WORLD;
	uint InstanceId		      : SV_InstanceID;
};

struct PCVertexOut
{
	float4 PosH  : SV_POSITION;
	float4 Color : COLOR;
};

struct PLVertexIn
{
	float3 PosL				  : POSITION;
	float3 NormalL			  : NORMAL;
	row_major float4x4 World  : WORLD;
	uint InstanceId		      : SV_InstanceID;
};

struct PLVertexOut
{
	float4 PosH    : SV_POSITION;
	float3 PosW    : POSITION;
	float3 NormalW : NORMAL;
};

struct GVertexIn
{
	float3 PosL	: POSITION;
	float2 Tex	: TEXCOORD;
	row_major float4x4 World  : WORLD;
	uint InstanceId		      : SV_InstanceID;
};

struct GVertexOut
{
	float4 PosL    : SV_POSITION;
	float2 Tex     : TEXCOORD0; // 보정 용도
};

// G버퍼
struct PS_GBUFFER_OUT
{
	float4 Depth        : SV_TARGET0;
	float4 Color		: SV_TARGET1;
	float4 Normal       : SV_TARGET2;
	float4 Position     : SV_TARGET3;
	float4 Specular     : SV_TARGET4;
};

struct SURFACE_DATA
{
	float4 DepthTex;
	float4 DiffuseTex;
	float4 SpecularTex;
	float4 TanNormalTex;
	float4 PositionTex;
};

// 라이트
DirectionalLight gDirLight;
PointLight       gPointLight;
SpotLight        gSpotLight;

// 매태리얼
Material gMaterial;

// 거리
float gNearFarlength;
float gNear;
float gFar;

// 카메라
float3 gEyePosW;

// 안개
float  gFogStart;
float  gFogRange;
float4 gFogColor;

// 텍스처
Texture2D gDiffuseTex;
Texture2D gSpecularTex;
Texture2D gNormalTex;

// 스킨 텍스처
Texture2D gIdleTex;
Texture2D gDamageTex;
Texture2D gRunTex;
Texture2D gWalkTex;
Texture2D gDeathTex;
Texture2D gDeathWaitTex;
Texture2D gAttack1Tex;
Texture2D gAttack2Tex;
Texture2D gAttack3Tex;
Texture2D gStunTex;

// G버퍼 텍스처
Texture2D gGDepthTex;
Texture2D gGDiffuseTex;
Texture2D gGPositionTex;
Texture2D gGSpecularTex;
Texture2D gGNormalTex;

//-----------------------------------//
// 매트릭스
//-----------------------------------//
// 월드
float4x4 gWorld;
float4x4 gWorldInvTranspose;

// 뷰
float4x4 gView;
float4x4 gViewInvTranspose;

// 프로젝션
float4x4 gProj;
float4x4 gProjInvTranspose;

// 합성
float4x4 gViewProj;
float4x4 gWorldViewProj;

// 기타
float4x4 gTexTFMtx;
float4x4 gLocTMMtx;
float4x4 gWdTMMtx;


// 텍스처 샘플링
SamplerState samAnisotropic
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;

	AddressU = WRAP;
	AddressV = WRAP;
};

SamplerState samInputImage
{
	Filter = MIN_MAG_LINEAR_MIP_POINT;

	AddressU = CLAMP;
	AddressV = CLAMP;
};

SamplerState samLinear
{
	Filter = MIN_MAG_MIP_LINEAR;

	AddressU = Wrap;
	AddressV = Wrap;
};

SamplerState samPoint
{
	Filter = MIN_MAG_MIP_POINT;

	AddressU = CLAMP;
	AddressV = CLAMP;
};

