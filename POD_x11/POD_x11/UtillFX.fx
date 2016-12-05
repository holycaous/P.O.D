

// ����Ʈ
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

// ����
struct Material
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular; // w = Specularer
	float4 Reflect;
};

// ���ؽ� ����
struct PNTVertexIn
{
	float3 PosL				  : POSITION;
	float3 NormalL			  : NORMAL;
	float2 Tex				  : TEXCOORD;
	float3 Tangent			  : TANGENT;
	float3 BiNormal			  : BINORMAL;
	float2 Pedding			  : PEDDING;        // �е� ��
	row_major float4x4 World  : WORLD;
	uint InstanceId		      : SV_InstanceID;
};

struct PNTVertexAniIn
{
	float2 Tex				  : TEXCOORD;
	float3 Weights            : WEIGHTS;		 // �ִϸ��̼� ����ġ
	float3 VtxInfo            : VTXINFO;		 // ���ý� ��ȣ, ���ý� ����, �е� ��
	uint4  BoneIndices        : BONEINDICES;	 // �ִϸ��̼� ���� �� �ε���
	row_major float4x4 World  : WORLD;
	float4 AniData            : ANIDATA;		 // �ִ� �ؽ�ó ��ȣ, ���� ������, �ؽ�ó �ʺ�, �ؽ�ó ����
	uint InstanceId		      : SV_InstanceID;
};

struct PNTVertexAniOut
{
	float4 PosH         : SV_POSITION;
	float3 PosW         : POSITION0;
	float3 NormalW      : NORMAL;
	float2 Tex          : TEXCOORD0;   // ���� �뵵
	float3 WT           : POSITION1;   // ��Ʈ���� ����� �뵵
	float3 WB           : POSITION2;   // ��Ʈ���� ����� �뵵
	float2 AniData      : ANIDATA;
};

struct PNTVertexMapIn
{
	float2 Tex				  : TEXCOORD;
	float2 VtxInfo            : VTXINFO;		 // ���ý� ��ȣ, ���ý� ����
	row_major float4x4 World  : WORLD;
	float3 TexData            : TEXDATA;		 // �� �ؽ�ó ��ȣ, �ؽ�ó �ʺ�, �ؽ�ó ����
	uint InstanceId		      : SV_InstanceID;
};

struct PNTVertexMapOut
{
	float4 PosH         : SV_POSITION;
	float3 PosW         : POSITION0;
	float3 NormalW      : NORMAL;
	float2 Tex          : TEXCOORD0;   // ���� �뵵
	float3 WT           : POSITION1;   // ��Ʈ���� ����� �뵵
	float3 WB           : POSITION2;   // ��Ʈ���� ����� �뵵
};

struct PNTVertexOut
{
	float4 PosH    : SV_POSITION;
	float3 PosW    : POSITION0;
	float3 NormalW : NORMAL;
	float2 Tex     : TEXCOORD0; // ���� �뵵
	float3 WT      : POSITION1; // ��Ʈ���� ����� �뵵
	float3 WB      : POSITION2; // ��Ʈ���� ����� �뵵
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
	float2 Tex     : TEXCOORD0; // ���� �뵵
};

// G����
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

// ����Ʈ
DirectionalLight gDirLight;
PointLight       gPointLight;
SpotLight        gSpotLight;


// ���¸���
Material gMaterial;

// �Ÿ�
float gNearFarlength;
float gNear;
float gFar;

// ī�޶�
float3 gEyePosW;

// �Ȱ�
float  gFogStart;
float  gFogRange;
float4 gFogColor;

// �ؽ�ó
Texture2D gDiffuseTex;
Texture2D gSpecularTex;
Texture2D gNormalTex;

// ��Ų �ؽ�ó
Texture2D gIdleTex;
Texture2D gDamageTex;
Texture2D gRunTex;
Texture2D gWalkTex;
Texture2D gDeathTex;
Texture2D gAttackTex;

// �� ��Ų �ؽ�ó
Texture2D gIdleModelTex;
Texture2D gDamageModelTex;
Texture2D gRunModelTex;
Texture2D gWalkModelTex;
Texture2D gDeathModelTex;
Texture2D gAttackModelTex;

// �� ��Ų �ؽ�ó
Texture2D gMapTex;
Texture2D gHeightMapTex;

// ������ �� �ؽ�ó
Texture2D gShadowMap;

// G���� �ؽ�ó
Texture2D gGDepthTex;
Texture2D gGDiffuseTex;
Texture2D gGPositionTex;
Texture2D gGSpecularTex;
Texture2D gGNormalTex;

// ť���
TextureCube gSkyBox;

//-----------------------------------//
// ��Ʈ����
//-----------------------------------//
// ����
float4x4 gWorld;
float4x4 gWorldInvTranspose;

// ��
float4x4 gView;
float4x4 gViewInvTranspose;

// ��������
float4x4 gProj;
float4x4 gProjInvTranspose;

// �ռ�
float4x4 gViewProj;
float4x4 gWorldViewProj;

// ��Ÿ
float4x4 gTexTFMtx;
float4x4 gLocTMMtx;
float4x4 gWdTMMtx;

// ������� ��ȯ
float4x4 gShadowTransform;
float4x4 gLightViewProj;

//------------------------------------------------------------------------------------//
// Mip Filter �� '�Ӹ�8�� ó���� ���� ���͸� �ɼ�' �̰�,
// Min Filter �� '������Ʈ�� ȭ�鿡�� �־�����, �۾ƺ��̰� �Ǿ��� �� �ؽ����� ó�����' �̸�
// Mag Filter �� '������Ʈ�� ȭ�鿡�� ���������, Ŀ���̰� �Ǿ��� �� �ؽ����� ó�����' �Դϴ�.
//------------------------------------------------------------------------------------//
// �ؽ�ó ���ø�
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
	// [MSDN���� ����]
	// ���� ��� �պ� �ܰ迡 ���ε� �� ���� ���۰� UNORM �����̰ų�
	// ���� ���۰� ����. ���̾ ���� ������ ���� ���ȴ�.
	//
	// Bias = (float) DepthBias * r + SlopeScaledDepthBias * MaxDepthSlope;
	//
	// ���⼭ r�� ���� ���� ���Ŀ��� �ּ� ǥ�� ���� ��> 0�̰� float32�� ��ȯ�˴ϴ�.
	// [/ MSDN ������]
	//
	// 24 ��Ʈ ���� ������ ��� r = 1 / 2 ^ 24�Դϴ�.
	//
	// �� : DepthBias = 100000 ==> ���� DepthBias = 100000 / 2 ^ 24 = .006

	// ����� ����� ���� �� ������ ���� �� �ʿ䰡 �ִ�.
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
	// ���� �Լ��� LESS_EQUAL�̰� LESS�� �ƴ��� Ȯ���Ͻʽÿ�.
	// �׷��� ������ z = 1 (NDC)���� ����ȭ �� ���� ����
	// ���� ���۰� 1�� Ŭ����Ǹ� ���� �׽�Ʈ�� �����մϴ�.
	DepthFunc = LESS_EQUAL;
};

DepthStencilState LessDSS
{
	DepthFunc = LESS;
};

//// �׽�Ʈ ����
//cbuffer cbSkinned
//{
//	float4x4 gBoneTransforms[140];
//};
