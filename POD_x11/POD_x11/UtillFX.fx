

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
	float2 Pedding			  : PEDDING;        // 패딩 값
	row_major float4x4 World  : WORLD;
	uint InstanceId		      : SV_InstanceID;
};

struct PNTVertexAniIn
{
	float2 Tex				  : TEXCOORD;
	float3 Weights            : WEIGHTS;		 // 애니메이션 가중치
	float3 VtxInfo            : VTXINFO;		 // 버택스 번호, 버택스 갯수, 패딩 값
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
	float2 AniData      : ANIDATA;
};

struct PNTVertexMapIn
{
	float2 Tex				  : TEXCOORD;
	float2 VtxInfo            : VTXINFO;		 // 버택스 번호, 버택스 갯수
	row_major float4x4 World  : WORLD;
	float3 TexData            : TEXDATA;		 // 맵 텍스처 번호, 텍스처 너비, 텍스처 높이
	uint InstanceId		      : SV_InstanceID;
};

struct PNTVertexMapOut
{
	float4 PosH         : SV_POSITION;
	float3 PosW         : POSITION0;
	float3 NormalW      : NORMAL;
	float2 Tex          : TEXCOORD0;   // 보정 용도
	float3 WT           : POSITION1;   // 매트릭스 만들기 용도
	float3 WB           : POSITION2;   // 매트릭스 만들기 용도
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
	float1 Pedding		      : PEDDING;
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
	float2 Pedding		      : PEDDING;
	row_major float4x4 World  : WORLD;
	uint InstanceId		      : SV_InstanceID;
};

struct PLVertexOut
{
	float4 PosH    : SV_POSITION;
	float3 PosW    : POSITION;
	float3 NormalW : NORMAL;
};

struct SkyBoxIn
{
	float3 PosL               : POSITION;
	float1 Pedding            : PEDDING;
	row_major float4x4 World  : WORLD;
	uint InstanceId		      : SV_InstanceID;
};

struct ShadowVertexIn
{
	float3 PosL				  : POSITION;
	float3 NormalL			  : NORMAL;
	float2 Tex				  : TEXCOORD;
	row_major float4x4 World  : WORLD;
	uint InstanceId		      : SV_InstanceID;
};

struct ShadowVertexOut
{
	float4 PosH : SV_POSITION;
	float2 Tex  : TEXCOORD;
};

struct SkyBoxOut
{
	float4 PosH : SV_POSITION;
	float3 PosL : POSITION;
};

struct GVertexIn
{
	float3 PosL	              : POSITION;
	float2 Tex	              : TEXCOORD;
	float3 Pedding		      : PEDDING;
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
Texture2D gAttackTex;

// 모델 스킨 텍스처
Texture2D gIdleModelTex;
Texture2D gDamageModelTex;
Texture2D gRunModelTex;
Texture2D gWalkModelTex;
Texture2D gDeathModelTex;
Texture2D gAttackModelTex;

// 맵 스킨 텍스처
Texture2D gMapTex;
Texture2D gHeightMapTex;

// 쉐도우 맵 텍스처
Texture2D gShadowMap;

// G버퍼 텍스처
Texture2D gGDepthTex;
Texture2D gGDiffuseTex;
Texture2D gGPositionTex;
Texture2D gGSpecularTex;
Texture2D gGNormalTex;

// 큐브맵
TextureCube gSkyBox;

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

// 쉐도우맵 변환
float4x4 gShadowTransform;
float4x4 gLightViewProj;

//------------------------------------------------------------------------------------//
// Mip Filter 는 '밉맵8을 처리할 때의 필터링 옵션' 이고,
// Min Filter 는 '오브젝트가 화면에서 멀어져서, 작아보이게 되었을 때 텍스쳐의 처리방법' 이며
// Mag Filter 는 '오브젝트가 화면에서 가까워져서, 커보이게 되었을 때 텍스쳐의 처리방법' 입니다.
//------------------------------------------------------------------------------------//
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

	AddressU = Wrap;
	AddressV = Wrap;
	AddressW = Wrap;
	MaxLOD = 0;
	MinLOD = 0;
};

SamplerComparisonState samShadow
{
	Filter = COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	AddressU = BORDER;
	AddressV = BORDER;
	AddressW = BORDER;
	BorderColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

	ComparisonFunc = LESS;
};

RasterizerState Depth
{
	// [MSDN에서 보낸]
	// 현재 출력 합병 단계에 바인딩 된 깊이 버퍼가 UNORM 형식이거나
	// 깊이 버퍼가 없다. 바이어스 값은 다음과 같이 계산된다.
	//
	// Bias = (float) DepthBias * r + SlopeScaledDepthBias * MaxDepthSlope;
	//
	// 여기서 r은 깊이 버퍼 형식에서 최소 표현 가능 값> 0이고 float32로 변환됩니다.
	// [/ MSDN 끝내기]
	//
	// 24 비트 깊이 버퍼의 경우 r = 1 / 2 ^ 24입니다.
	//
	// 예 : DepthBias = 100000 ==> 실제 DepthBias = 100000 / 2 ^ 24 = .006

	// 당신은 당신의 씬에 이 값들을 실험 할 필요가 있다.
	DepthBias = 100000;
	DepthBiasClamp = 0.0f;
	SlopeScaledDepthBias = 1.0f;
};

RasterizerState NoCull
{
	CullMode = None;
};

DepthStencilState LessEqualDSS
{
	// 깊이 함수가 LESS_EQUAL이고 LESS가 아닌지 확인하십시오.
	// 그렇지 않으면 z = 1 (NDC)에서 정규화 된 깊이 값이
	// 깊이 버퍼가 1로 클리어되면 깊이 테스트에 실패합니다.
	DepthFunc = LESS_EQUAL;
};

DepthStencilState LessDSS
{
	DepthFunc = LESS;
};

//// 테스트 전용
//cbuffer cbSkinned
//{
//	float4x4 gBoneTransforms[140];
//};
