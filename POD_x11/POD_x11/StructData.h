#pragma once

#define BUF_SIZE 1024

// Enum ��
// ��������
typedef enum
{
	e_MapTool   = 0,
	e_MainState = 1,
	e_PlayState = 2
}GAME_STATE_TYPE;

// �⺻ ����
typedef enum
{
	e_MatWhite = 0,
	e_MatBlack = 1,
	e_MatRed   = 2,
	e_MatGreen = 3,
	e_MatBlue  = 4
}MAT_TYPE;

// ���չ��
typedef enum
{
	e_ShaderColor	   = 0,
	e_ShaderLight	   = 1,
	e_ShaderPongTex	   = 2,
	e_ShaderCartoonTex = 3,
	e_ShaderDeferred   = 4,
	e_ShaderPongTexAni = 5,
	e_ShaderPongTexMap = 6,
	e_ShaderSkyBox     = 7
}SHADER_TYPE;

// ���̴� ���� �ʱ�ȭ
typedef enum
{
	e_ShaderValMtx      = 0,
	e_ShaderValVtx      = 1,
	e_ShaderVal         = 2,
	e_ShaderValResource = 3,
	e_ShaderValMtxArray = 4
}SHADER_VAL_TYPE;

typedef enum
{
	e_Basic  = 0,
	e_Shadow = 1
}TECH_TYPE;

// �� FSM
typedef enum
{
	e_Idle      = 0,
	e_Damage    = 1,
	e_Run       = 2,
	e_Walk      = 3,
	e_Death     = 4,
	e_Attack    = 5
}FSM_TYPE;

// � �ؽ�ó�� �ε��� ���ΰ�
typedef enum
{
	e_DiffuseMap  = 0,
	e_SpecularMap = 1,
	e_NomalMap    = 2
}TEXTURE_TYPE;

// ������� �����Ͷ���¡�� �Ұ��ΰ�
typedef enum
{
	e_Wire  = 0,
	e_Solid = 1
}RES_STATE;

// �⺻���� ��, �Ľ��� ���ΰ�
typedef enum
{
	e_BasicModel   = 0,
	e_ParsingModel = 1

}MODEL_TYPE;

// ���� ī�޶��� ��尡 �����ΰ�
typedef enum
{
	e_1Person = 0,
	e_3Person = 1
}CAM_MODE;

// ������Ʈ�� �����ΰ�, �����ΰ�
typedef enum
{
	e_StaticObj  = 0,
	e_DynamicObj = 1

}OBJ_MOVEABLE;


// ���ؽ� ����
struct VertexPC
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
	float   Pedding;			// �е� ��
};

struct VertexPN
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT2 Pedding;			// �е� ��
};

struct VertexPNT
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT2 Tex;
	XMFLOAT3 Tangent;
	XMFLOAT3 BiNormal;
	XMFLOAT2 Pedding; // �е� ��
};

struct VertexPNTAni
{
	XMFLOAT2 Tex;
	XMFLOAT3 Weights;
	XMFLOAT3 VtxInfo; // ���ؽ� ���� �־���
	UINT     BoneIndices[4];
};

struct VertexPNTMap
{
	XMFLOAT2 Tex;
	XMFLOAT2 VtxInfo; // ���ؽ� ���� �־���
};

struct VertexSkyBox
{
	XMFLOAT3 Pos;
	float    Pedding; // �е� ��
};

struct VertexG
{
	XMFLOAT3 Pos;
	XMFLOAT2 Tex;
	XMFLOAT3 Pedding;
};

// �ν��Ͻ� ����
struct InsAni
{
	XMFLOAT4X4 mMtx;
	XMFLOAT4   mInsAni; // ����� �ִ��� �ؽ�ó��ȣ, ������ ����, �׽�ó �ʺ�, ����
};

// �ν��Ͻ� ����
struct InsMap
{
	XMFLOAT4X4 mMtx;
	XMFLOAT3   mInsTex; // ����� �ؽ�ó��ȣ,  �׽�ó �ʺ�, ����
};

// ����ü ����
struct Vertex
{
	// ������ 0
	Vertex() : TexUV(FLT_MAX, FLT_MAX), BiNormal(0.0f, 0.0f, 0.0f), Color(1.0f, 0.0f, 0.0f, 1.0f)
	{}

	// ������ 1
	Vertex(const XMFLOAT3& p, const XMFLOAT3& n, const XMFLOAT3& t, const XMFLOAT2& uv)
		: Position(p), Normal(n), TangentU(t), TexUV(FLT_MAX, FLT_MAX), BiNormal(0.0f, 0.0f, 0.0f), Color(1.0f, 0.0f, 0.0f, 1.0f)
	{}

	// ������ 2
	Vertex(
		float px, float py, float pz,
		float nx, float ny, float nz,
		float tx, float ty, float tz)
		: Position(px, py, pz), Normal(nx, ny, nz),
		TangentU(tx, ty, tz), TexUV(FLT_MAX, FLT_MAX), BiNormal(0.0f, 0.0f, 0.0f), Color(1.0f, 0.0f, 0.0f, 1.0f)
	{}

	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT4 Color;
	XMFLOAT3 TangentU;
	XMFLOAT3 BiNormal;
	XMFLOAT2 TexUV;
};

// �ٿ�� �ڽ�
class BoundBox
{
public:
	XMFLOAT3 Max;
	XMFLOAT3 Min;
	XMFLOAT3 Center;
};

// ����
class MyMat
{
public:
	// ���ҽ� �� ( ��ġ )
	char mDiffuseSRV [BUF_SIZE];
	char mSpecularSRV[BUF_SIZE];
	char mNomalSRV   [BUF_SIZE];

	// ���� (��ǻ�� ��)
	float mOpacity;
public:
	MyMat()
	{
		InitClass();
	}
	~MyMat()
	{
		ClearClass();
	}

	void InitClass()
	{
		strcpy(mDiffuseSRV , "NULL\0");
		strcpy(mSpecularSRV, "NULL\0");
		strcpy(mNomalSRV   , "NULL\0");
		mOpacity = 100.0f;
	}

	void ClearClass()
	{
		memset(&mDiffuseSRV , '\0', sizeof(mDiffuseSRV));
		memset(&mSpecularSRV, '\0', sizeof(mSpecularSRV));
		memset(&mNomalSRV   , '\0', sizeof(mNomalSRV));
		mOpacity = 0.0f;
	}
};

// �ĺ��ڵ带 ���� ���ؽ� ����Ʈ
class KeyVtx
{
public:
	int Key;      // Ű��
	XMFLOAT3 Vtx; // ���ؽ�
};

class RotKeyVtx
{
public:
	int Key;      // Ű��
	XMFLOAT4 Vtx; // ���ؽ�
};

// �� ������ (����ġ)
class BoneData
{
public:
	int   ID;
	float Weight;
	char  Name[BUF_SIZE];
public:
	BoneData()
	{
		ID = -1;
		strcpy(Name, "NULL\0");
	}
	~BoneData()
	{
		memset(&Name, '\0', sizeof(Name));
	}
};


// ���ؽ� ����ġ ������
class WeightVtx
{
public:
	int TgVtxIdx;
	vector<BoneData> Bone;

public:
	~WeightVtx()
	{
		Bone.clear();
	}
};

// �ִ� ������
class AniData
{
public:
	vector<KeyVtx>    Position;
	vector<RotKeyVtx> Quaternion;
	vector<KeyVtx>    Scale;
public:
	~AniData()
	{
		ClearClass();
	}

	void ClearClass()
	{
		Position  .clear();
		Quaternion.clear();
		Scale     .clear();
	}
};

// ��Ű�� �ؽ�ó
class SkinTexture
{
public:
	string mName;
	ID3D11Texture2D* mTexture;
	ID3D11ShaderResourceView* mTexSRV;

	// �ִ� ����
	float mAniType;  // Ÿ��
	float mStPoint;  // ����
	float mEdPoint;  // ��

	float mTexWidth;  // �ؽ�ó �ʺ�
	float mTexHeight; // �ؽ�ó ����

public:
	SkinTexture()
	{
		mTexture = nullptr;
		mTexSRV  = nullptr;
		mName.clear();

		mAniType = (float)e_Idle;
		mStPoint = mEdPoint = 0;
		mTexWidth = mTexHeight = 0;
	}

	~SkinTexture()
	{
		ReleaseCOM(mTexture);
		ReleaseCOM(mTexSRV);
		mName.clear();
	}

	void ClearTex()
	{
		ReleaseCOM(mTexture);
	}
};

// ����
struct CharBuf
{
	char Data[BUF_SIZE];
};

// Ű�� ���� ���ڿ�
class KeyString
{
public:
	int  Key;
	char Name[BUF_SIZE];
	char objClass[BUF_SIZE];
};

// ���� �� ������ ��� ���� ����
class ObjData
{
public:
	int          mUniqueCode;
	XMFLOAT4X4   mWdMtx;
	OBJ_MOVEABLE mObjMoveAble;

	// ������Ʈ ���°�
	float mHP;
	float mMP;

	// �ִ� ����
	float mAniType;  // Ÿ��
	float mStPoint;  // ����
	float mEdPoint;  // ��
	float mFrame;    // ������

	float mTexWidth;  // �ؽ�ó �ʺ�
	float mTexHeight; // �ؽ�ó ����

	float mAniSpeed; // �ִ� ���ǵ�

private:
	// �� (�߷�)
	float mForce;

	float mGround;
public:
	ObjData()
	{
		// Ű (���� Ű ��ȣ�� �ο����ְ� ����)
		mUniqueCode = -1; 

		// ���� ��Ʈ���� �ʱ�ȭ
		XMMATRIX I = XMMatrixIdentity();
		XMStoreFloat4x4(&mWdMtx, I);

		// ������Ʈ�� �������� ��������
		mObjMoveAble = e_StaticObj;

		mAniType = (float)e_Idle;
		mStPoint = mEdPoint = mFrame = 0.0f;

		// �ִϸ��̼� �⺻ ���ǵ�
		mAniSpeed = 25.0f;

		// �߷�
		mForce = 0.0f;
		mGround = 0.0f;
	}
	~ObjData()
	{

	}

	// ����
	void Jump()
	{
		if (mWdMtx._42 == mGround)
		{
			mForce += 350.0f;
		}
	}

	// ü�� ����
	void SetHp(float _hp)
	{
		mHP = _hp;
	}

	// ���� ����
	void SetMp(float _mp)
	{
		mMP = _mp;
	}

	// �ִϸ��̼� ���ǵ� ����
	void SetAniSpeed(float _aniSpeed)
	{
		// �ִϸ��̼� �ӵ��� 1���� Ŀ���Ѵ�.
		if (_aniSpeed <= 1)
			mAniSpeed = 1;
		else
			mAniSpeed = _aniSpeed;
	}

	// �ִ�Ÿ�� ����
	void SetFSM(float _modelFsm, float _StPoint, float _mEdPoint, float _TexWidth, float _TexHeight)
	{
		// �ִ�Ÿ�� ����
		mAniType = _modelFsm;

		// ���°� �������� ������ �ʱ�ȭ �� �ִϸ��̼��� ���۰� ���� �����;���.
		mFrame   = 0.0f;
		mStPoint = _StPoint;
		mEdPoint = _mEdPoint;

		// �ؽ�ó ũ�� ����
		mTexWidth  = _TexWidth;
		mTexHeight = _TexHeight;
	}

	// �ִ�Ÿ�� ����
	void SetFSM(float _modelFsm, float _StPoint, float _mEdPoint, float _TexWidth, float _TexHeight, float _Frame)
	{
		// �ִ�Ÿ�� ����
		mAniType = _modelFsm;

		// ���°� �������� ������ �ʱ�ȭ �� �ִϸ��̼��� ���۰� ���� �����;���.
		mFrame   = _Frame;
		mStPoint = _StPoint;
		mEdPoint = _mEdPoint;

		// �ؽ�ó ũ�� ����
		mTexWidth  = _TexWidth;
		mTexHeight = _TexHeight;
	}

	// ������ ���ϱ�
	void SetFrame(float _frame)
	{
		mFrame = _frame;
	}

	// �ִ� �ð� ������Ʈ
	void Update(float& dt, float _y)
	{
		// �ð� �긣��
		mFrame += (dt * mAniSpeed);

		// ���������� ũ�ٸ�, �ʱ�ȭ
		if (mEdPoint <= mFrame)
			mFrame = mStPoint;

		// �ٴ� ��ġ ����
		mGround = _y;

		// �߷� ����
		mForce -= GRAVITY * dt;
		mWdMtx._42 += mForce * dt;

		// ���ٴ� �浹üũ
		mWdMtx._42 = max(mGround, mWdMtx._42);

		// �ٴڿ� ������ �� ����
		if (mWdMtx._42 <= mGround)
			mForce = 0.0f;
	}
	
	// ��ġ ��ȯ
	XMFLOAT3 getPos() const
	{
		return XMFLOAT3(mWdMtx._41, mWdMtx._42, mWdMtx._43);
	}

	// ��ġ ��ȯ
	XMFLOAT3 getPosXZ() const
	{
		return XMFLOAT3(mWdMtx._41, 0.0f, mWdMtx._43);
	}

	// ��ġ ����
	void setPos(float _x, float _y, float _z)
	{
		mWdMtx._41 = _x;
		mWdMtx._42 = _y;
		mWdMtx._43 = _z;
	}

	// ��ġ ����
	void setPosXZ(float _x, float _z)
	{
		mWdMtx._41 = _x;
		mWdMtx._43 = _z;
	}

	// ��ġ ����
	void setPosY(float _y)
	{
		mWdMtx._42 = _y;
	}

	// ������ ����
	void setScale(float _x, float _y, float _z)
	{
		mWdMtx._11 *= _x;
		mWdMtx._22 *= _y;
		mWdMtx._33 *= _z;
	}

	// ���� ��Ʈ���� ���
	XMFLOAT4X4 getWdMtx() const
	{
		return mWdMtx;
	}

	// X ���� ���
	XMFLOAT3 getRightDir() const
	{
		return XMFLOAT3(mWdMtx._31, mWdMtx._32, mWdMtx._33);
	}

	// Y ���� ���
	XMFLOAT3 getUpDir() const
	{
		return XMFLOAT3(mWdMtx._21, mWdMtx._22, mWdMtx._23);
	}

	// Z ���� ���
	XMFLOAT3 getLookDir() const
	{
		return XMFLOAT3(mWdMtx._11, mWdMtx._12, mWdMtx._13);
	}


	// ���� ��Ʈ���� ����
	void SetWdMtx(XMFLOAT4X4 _WdMtx)
	{
		mWdMtx = _WdMtx;
	}

	// X ���� ����
	void setRightDir(float _x, float _y, float _z) 
	{
		mWdMtx._31 = _x;
		mWdMtx._32 = _y;
		mWdMtx._33 = _z;
	}

	// Y ���� ����
	void setUpDir(float _x, float _y, float _z) 
	{
		mWdMtx._21 = _x;
		mWdMtx._22 = _y;
		mWdMtx._23 = _z;
	}

	// Z ���� ����
	void setLookDir(float _x, float _y, float _z) 
	{
		mWdMtx._11 = _x;
		mWdMtx._12 = _y;
		mWdMtx._13 = _z;
	}
};

// �÷��̾�
class PlayerInfo
{
public:
	XMFLOAT3 mPos;
	int mkey;
	string mModelName;
public:
	PlayerInfo()
	{
		mkey = -1;
		mModelName.clear();
	}
	~PlayerInfo()
	{
		mkey = -1;
		mModelName.clear();
	}
};

// �Ľ��� �Ž� ������
class InitMetaData
{
public:
	//--------------------------------------------------//
	// �� ����
	//--------------------------------------------------//
	vector<float> mHeightmap;

	//--------------------------------------------------//
	// �Ϲ� ������Ʈ
	//--------------------------------------------------//
	// ���ؽ�, �ε���
	vector<Vertex>   Vertices;
	vector<UINT>     Indices;

	// �����ø��� �뵵(�Ľ� ������ �������� �Ⱦ�)
	vector<UINT>     TexIndices;
	vector<XMFLOAT3> TexVertices;

	// ���� ��Ʈ���� (�� ĳ���� ���п�)
	map<int, ObjData> mObjData;

	// �ؽ�ó ��ȯ ��Ʈ����
	XMFLOAT4X4    mTexMtx;

	// TM ��Ʈ����
	XMFLOAT4X4    mLocTMMtx;
	XMFLOAT4X4    mWdTMMtx;
	
	// ���ҽ� �� (��������)
	ID3D11ShaderResourceView* mDiffuseSRV;
	ID3D11ShaderResourceView* mSpecularSRV;
	ID3D11ShaderResourceView* mNomalSRV;

	// ���� (��ǻ�� ��)
	float mOpacity;

	// ������
	UINT mVertexOffset;
	UINT mIndexOffset;

	// ī��Ʈ
	UINT mIndexCount;

	// �ִ� ������ (��Ʈ)
	AniData mAniData;

	// �ִϵ����Ͱ� �ִ� ���ΰ�
	bool mAniModel;

	// ��Ų �ؽ�ó (������)
	map<int, SkinTexture*> mSkinTex;

	// ��Ų �� (��Ų �ؽ�ó�� �ѽ��̵Ǵ� �� ������)
	map<int, SkinTexture*> mSkinModelTex;

	// ����ġ ������
	vector<WeightVtx> weightVtx;

	// ���� ������� ������ ���ΰ�
	D3D_PRIMITIVE_TOPOLOGY _PRIMITIVE_TOPOLOGY;

	// ���� ����
	MODEL_TYPE mModelType;

	// ���̴� ���
	SHADER_TYPE mShaderMode;

	// �ٿ�� �ڽ�
	BoundBox mBoundingBox;

	//--------------------------------------------------//
	// ������Ʈ �ĺ� ����
	//--------------------------------------------------//

	// �����̸�
	string mCreateName;

	// �̸�
	char mMainName[BUF_SIZE];

	// ������Ʈ�̸�
	int  mObjID;
	char mObjName[BUF_SIZE];
	char mObjClass[BUF_SIZE];

	// ���� Ŭ���� �̸�
	int  mParentID;
	char mParentName[BUF_SIZE];
	
private:
	// ����ũ�ڵ� �Ҵ��.
	int _inputcode;

private:
	cCoreStorage* mCoreStorage = cCoreStorage::GetInstance();

public:
	InitMetaData(const char* _Name, SHADER_TYPE _ShaderMode, D3D_PRIMITIVE_TOPOLOGY _D3D_PRIMITIVE_TOPOLOGY)
	{
		// ���� �ʱ�ȭ
		initValue();

		// �̸� ���
		strcpy(mMainName, _Name);

		// ���̴� ���
		mShaderMode = _ShaderMode;

		// ������� ���� ������ ���ΰ�
		_PRIMITIVE_TOPOLOGY = _D3D_PRIMITIVE_TOPOLOGY;

		// ���� �ʱ�ȭ
		strcpy(mObjName   , "NULL\0");
		strcpy(mObjClass  , "NULL\0");
		strcpy(mParentName, "NULL\0");
	}

	InitMetaData(SHADER_TYPE _ShaderMode, D3D_PRIMITIVE_TOPOLOGY _D3D_PRIMITIVE_TOPOLOGY)
	{
		// ���� �ʱ�ȭ
		initValue();
		
		// ���̴� ���
		mShaderMode = _ShaderMode;

		// ������� ���� ������ ���ΰ�
		_PRIMITIVE_TOPOLOGY = _D3D_PRIMITIVE_TOPOLOGY;

	}

	~InitMetaData()
	{
		printf("Ŭ���� ��ü��: %17s ---> ������: %12s, �̸�: %12s\n", mObjName, mCreateName.c_str(), mMainName);

		// ��Ų �� ����
		for (auto itor = mSkinModelTex.begin(); itor != mSkinModelTex.end(); ++itor)
			SafeDelete(itor->second);
		mSkinModelTex.clear();

		// �� �ؽ�ó ����
		ReleaseCOM(mDiffuseSRV);
		ReleaseCOM(mSpecularSRV);
		ReleaseCOM(mNomalSRV);

		initValue();
	}

	// ���� �ʱ�ȭ
	void initValue()
	{
		// ���̵� ��
		mObjID    = -1;
		mParentID = -1;

		// ����ũ �ڵ� �Ҵ��
		_inputcode = -1;

		// �ִϰ� �ִ� ���ΰ�
		mAniModel = false;

		// ���ҽ�
		mDiffuseSRV = mSpecularSRV = mNomalSRV = nullptr;

		// ��ǻ��� ����
		mOpacity = 100.0f;

		// ��Ʈ���� �ʱ�ȭ
		XMMATRIX I = XMMatrixIdentity();

		// �ؽ�ó ��ȯ ��Ʈ���� �ʱ�ȭ
		XMStoreFloat4x4(&mTexMtx, I);
		XMStoreFloat4x4(&mLocTMMtx, I);
		XMStoreFloat4x4(&mWdTMMtx, I);

		// ���� �ʱ�ȭ
		memset(&mMainName  , '\0', sizeof(mMainName));
		memset(&mObjName   , '\0', sizeof(mObjName));
		memset(&mObjClass  , '\0', sizeof(mObjClass));
		memset(&mParentName, '\0', sizeof(mParentName));

		// STL �ʱ�ȭ
		for (unsigned int i = 0; i < weightVtx.size(); ++i)
			weightVtx[i].Bone.clear();
		weightVtx.clear();

		Vertices   .clear();
		Indices    .clear();
		TexIndices .clear();
		TexVertices.clear();
		mObjData   .clear();
		mCreateName.clear();
		mSkinTex   .clear();
		mHeightmap .clear();
	}

	// �ؽ�ó �ε�
	void LoadTex(string _TexName, TEXTURE_TYPE e_InitTex)
	{
		// string --> wString���� ��ȯ
		wstring _WsTexName;
		StringToWchar_t(_TexName, _WsTexName);

		// �ؽ�ó �ε�
		switch (e_InitTex)
		{
		case e_DiffuseMap:
			HR(D3DX11CreateShaderResourceViewFromFile(mCoreStorage->md3dDevice, _WsTexName.c_str(), 0, 0, &mDiffuseSRV, 0));
			break;
		case e_NomalMap:
			HR(D3DX11CreateShaderResourceViewFromFile(mCoreStorage->md3dDevice, _WsTexName.c_str(), 0, 0, &mNomalSRV, 0));
			break;
		case e_SpecularMap:
			HR(D3DX11CreateShaderResourceViewFromFile(mCoreStorage->md3dDevice, _WsTexName.c_str(), 0, 0, &mSpecularSRV, 0));
			break;
		}
	}

	// ��ȯ �Լ�
	void StringToWchar_t(string& _string, wstring& _wstring)
	{
		// ��ȯ
		for (unsigned int i = 0; i < _string.length(); ++i)
			_wstring += wchar_t(_string[i]);

		_wstring += wchar_t('\0');
	}

	// �� (���� ��Ʈ����) �߰��ϱ�
	void AddModel(ObjData _ObjData)
	{
		mObjData[_ObjData.mUniqueCode] = _ObjData;
	}

	// �� (���� ��Ʈ����) �߰��ϱ�
	void AddModel(float _x, float _y, float _z, OBJ_MOVEABLE _moveAble)
	{
		// ������Ʈ �ʱ�ȭ
		ObjData _ObjData;

		// ����ũ �ڵ� �ο�
		_ObjData.mUniqueCode = ++_inputcode;

		// ��ǥ ����
		_ObjData.mWdMtx._41 = _x;
		_ObjData.mWdMtx._42 = _y;
		_ObjData.mWdMtx._43 = _z;

		// ������Ʈ �̵� �����Ѱ�
		_ObjData.mObjMoveAble = _moveAble;

		// ������Ʈ ����
		mObjData[_ObjData.mUniqueCode] = _ObjData;
	}

	// �� (���� ��Ʈ����) �߰��ϱ�
	void AddModel(XMFLOAT4X4 _mWdMtx, OBJ_MOVEABLE _moveAble)
	{
		// ������Ʈ �ʱ�ȭ
		ObjData _ObjData;

		// ����ũ �ڵ� �ο�
		_ObjData.mUniqueCode = ++_inputcode;

		// ��ǥ ����
		_ObjData.mWdMtx = _mWdMtx;

		// ������Ʈ �̵� �����Ѱ�
		_ObjData.mObjMoveAble = _moveAble;

		// ������Ʈ ����
		mObjData[_ObjData.mUniqueCode] = _ObjData;
	}

	// �� (���� ��Ʈ����) �߰��ϱ�
	void AddModel(int _key, float _x, float _y, float _z, OBJ_MOVEABLE _moveAble)
	{
		// ������Ʈ �ʱ�ȭ
		ObjData _ObjData;

		// ����ũ �ڵ� �ο�
		_ObjData.mUniqueCode = _key;

		// ��ǥ ����
		_ObjData.mWdMtx._41 = _x;
		_ObjData.mWdMtx._42 = _y;
		_ObjData.mWdMtx._43 = _z;

		// ������Ʈ �̵� �����Ѱ�
		_ObjData.mObjMoveAble = _moveAble;

		// ������Ʈ ����
		mObjData[_ObjData.mUniqueCode] = _ObjData;
	}

	// �� (���� ��Ʈ����) �߰��ϱ�
	void AddModel(int _key, XMFLOAT4X4 _mWdMtx, OBJ_MOVEABLE _moveAble)
	{
		// ������Ʈ �ʱ�ȭ
		ObjData _ObjData;

		// ����ũ �ڵ� �ο�
		_ObjData.mUniqueCode = _key;

		// ��ǥ ����
		_ObjData.mWdMtx = _mWdMtx;

		// ������Ʈ �̵� �����Ѱ�
		_ObjData.mObjMoveAble = _moveAble;

		// ������Ʈ ����
		mObjData[_ObjData.mUniqueCode] = _ObjData;
	}

	// �� (���� ��Ʈ����) �����ϱ�
	void EraseModel(int _key)
	{
		mObjData.erase(_key);
	}

	// ������Ʈ �̵�
	void SetPos(int _uniqueCode, float _x, float _y, float _z)
	{
		// ������Ʈ �̵�
		mObjData[_uniqueCode].setPos(_x, _y, _z);
	}

	// ������Ʈ �̵�
	void SetPosXZ(int _uniqueCode, float _x, float _z)
	{
		// ������Ʈ �̵�
		mObjData[_uniqueCode].setPosXZ(_x, _z);
	}

	// �ش� ��ġ�� ���� ���͸� ��´�
	XMVECTOR GetPointDir(int _uniqueCode, float _x, float _y, float _z)
	{
		// ��ġ�� �־�����.
		XMVECTOR tPoint  = XMLoadFloat3(&XMFLOAT3(_x, 0.0f, _z));
		XMVECTOR tObjPos = XMLoadFloat3(&mObjData[_uniqueCode].getPosXZ());

		// ��ġ�� ���ϴ� ���͸� ��´�.
		return XMVECTOR(XMVector3Normalize(tPoint - tObjPos));
	}

	// �ش� ��ġ�� ���� ���͸� ��´�
	XMVECTOR GetPointDir(int _uniqueCode, XMFLOAT3 _Pos)
	{
		// ��ġ�� �־�����.
		XMVECTOR tPoint  = XMLoadFloat3(&XMFLOAT3(_Pos.x, 0.0f, _Pos.z));
		XMVECTOR tObjPos = XMLoadFloat3(&mObjData[_uniqueCode].getPosXZ());

		// ��ġ�� ���ϴ� ���͸� ��´�.
		return XMVECTOR(XMVector3Normalize(tPoint - tObjPos));
	}
	
	// ������Ʈ ȸ��
	void SetRotate(int _uniqueCode, float _x, float _y, float _z)
	{
		// ȸ�� ���� �� (Ŭ���� ���� ����.)
		float InterpolationAmount = 12 * mCoreStorage->mTimer->DeltaTime();

		// ȸ�� ��������
		float Rotatelimit = 0.05f;

		// ��ġ�� ���ϴ� ���͸� ��´�.
		XMVECTOR tfromPointLookDir = GetPointDir(_uniqueCode, _x, _y, _z);

		// ������Ʈ ������ ��´�.
		XMVECTOR tObjLook  = XMLoadFloat3(&mObjData[_uniqueCode].getLookDir());
		XMVECTOR tObjUp    = XMLoadFloat3(&mObjData[_uniqueCode].getUpDir());
		XMVECTOR tObjRight = XMLoadFloat3(&mObjData[_uniqueCode].getRightDir());
		XMVECTOR tEpsilon = { 0.05f, 0.05f, 0.05f };

		// ȸ�� ������ ��´�(��, ��) - ��Į�� ������
		float tRadius = XMVectorGetX(XMVector3Dot(tObjUp, XMVector3Cross(tObjLook, tfromPointLookDir)));

		// ȸ���Ҹ��� ��ġ�� �ִ°�?
		if ((tRadius > Rotatelimit || tRadius < -Rotatelimit) || !XMVector3NearEqual(tfromPointLookDir, tObjLook, tEpsilon))
		{
			// �������� �������� ȸ�� ����� �����.
			XMMATRIX tRotMtx;
			tRotMtx = XMMatrixRotationAxis(tObjUp, tRadius * InterpolationAmount);

			// ȸ�� ��� ��
			XMFLOAT3 tFinLookDir;
			XMStoreFloat3(&tFinLookDir, XMVector3TransformNormal(tObjLook, tRotMtx));

			XMFLOAT3 tFiRightDir;
			XMStoreFloat3(&tFiRightDir, XMVector3TransformNormal(tObjRight, tRotMtx));

			// �� ���⺤�Ϳ� �����Ѵ�.
			mObjData[_uniqueCode].setLookDir (tFinLookDir.x, tFinLookDir.y, tFinLookDir.z);
			mObjData[_uniqueCode].setRightDir(tFiRightDir.x, tFiRightDir.y, tFiRightDir.z);
		}
	}

	// ������Ʈ ������
	void SetScale(int _uniqueCode, float _x, float _y, float _z)
	{
		// ������Ʈ �̵�
		mObjData[_uniqueCode].setScale(_x, _y, _z);
	}

	// ������Ʈ ����
	void ClearWdMtx()
	{
		_inputcode = -1;
		mObjData.clear();
	}

	// ���� ����ϱ�
	void CalValue()
	{
		mVertexOffset = Vertices.size();
		mIndexOffset  = Indices.size();
		mIndexCount   = Indices.size();
	}

	// ������Ʈ ������ 1�� �������
	ObjData& getObj(int _uniqueCode)
	{
		return mObjData[_uniqueCode];
	}

	// ������Ʈ ������ ��� ��������
	map<int, ObjData>& getAllObj()
	{
		return mObjData;
	}

	XMFLOAT3 getPos(int _uniqueCode)
	{
		return mObjData[_uniqueCode].getPos();
	}

	// HP ����
	void SetHp(int _unicode, float _hp)
	{
		mObjData[_unicode].SetHp(_hp);
	}
	
	// MP ����
	void SetMp(int _unicode, float _Mp)
	{
		mObjData[_unicode].SetMp(_Mp);
	}

	// ����
	void Jump(int _unicode)
	{
		mObjData[_unicode].Jump();
	}

	// ������Ʈ FSM ����
	void SetFSM(int _unicode, FSM_TYPE _modelFsm)
	{
		float _newType   = mSkinTex[_modelFsm]->mAniType;
		float _StPoint   = mSkinTex[_modelFsm]->mStPoint;
		float _EdPoint   = mSkinTex[_modelFsm]->mEdPoint;
		float _TexWidth  = mSkinTex[_modelFsm]->mTexWidth;
		float _TexHeight = mSkinTex[_modelFsm]->mTexHeight;

		// ���ο� �𵨷� ����
		mObjData[_unicode].SetFSM(_newType, _StPoint, _EdPoint, _TexWidth, _TexHeight);
	}

	// ������Ʈ FSM ����
	void SetFSM(int _unicode, FSM_TYPE _modelFsm, float _Frame)
	{
		float _newType   = mSkinTex[_modelFsm]->mAniType;
		float _StPoint   = mSkinTex[_modelFsm]->mStPoint;
		float _EdPoint   = mSkinTex[_modelFsm]->mEdPoint;
		float _TexWidth  = mSkinTex[_modelFsm]->mTexWidth;
		float _TexHeight = mSkinTex[_modelFsm]->mTexHeight;

		// ���ο� �𵨷� ����
		mObjData[_unicode].SetFSM(_newType, _StPoint, _EdPoint, _TexWidth, _TexHeight, _Frame);
	}
};

// �� ����
class MapINFO
{
public:
	InitMetaData* mModel;
	float mXwidth;
	float mZdepth;
	float mCellSize;
	float mHeightScale;

	MapINFO()
	{
		mModel = nullptr;
		mXwidth = mZdepth = mCellSize = mHeightScale = 0.0f;
	}

	float GetWidth()
	{
		// �� ���� ��.
		return (mXwidth - 1) * mCellSize;
	}
	float GetDepth()
	{
		// �� ���� ����.
		return (mZdepth - 1) * mCellSize;
	}

	// ���� ��� (�ӽ÷� ó��)
	float GetHeight(float& x, float& z)
	{
		// �� ��ǥ��� �ű��
		float c = (x + 0.5f * GetWidth()) /  mCellSize;
		float d = (z - 0.5f * GetDepth()) / -mCellSize;

		// �Ҽ��� ������
		int row = (int)floorf(d);
		int col = (int)floorf(c);

		// ������ ����Ʈ�� ����
		// A*--*B
		//  | /|
		//  |/ |
		// C*--*D
		vector<float>& _heightMap = mModel->mHeightmap;

		float A = _heightMap[row * (int)mXwidth + col];
		float B = _heightMap[row * (int)mXwidth + col + 1];
		float C = _heightMap[(row + 1) * (int)mXwidth + col];
		float D = _heightMap[(row + 1) * (int)mXwidth + col + 1];

		// ���� ���õ� ������� ��ġ.
		float s = c - (float)col;
		float t = d - (float)row;

		// ���� �ﰢ�� ABC �� ���.
		if (s + t <= 1.0f)
		{
			float uy = B - A;
			float vy = C - A;
			return A + s*uy + t*vy;
		}
		else // �Ʒ��� �ﰢ�� DCB
		{
			float uy = C - D;
			float vy = B - D;
			return D + (1.0f - s)*uy + (1.0f - t)*vy;
		}
	}
};

// ����Ʈ ����
class EffectStorage
{
public:
	ID3DX11Effect* mFX;			   // ����Ʈ
	map<int, ID3DX11EffectTechnique*> mTech; // ��ũ��
	map<string, ID3DX11EffectMatrixVariable*> mfxMtx; // ���
	map<string, ID3DX11EffectVectorVariable*> mfxVtx; // ����
	map<string, ID3DX11EffectVariable*>     mfxValue; // ����

	map<string, ID3DX11EffectShaderResourceVariable*>  mfxResource; // ���ҽ�
	
	map<int, ID3D11InputLayout*> mInputLayout;
public:
	EffectStorage()
	{
		mFX = nullptr;
	}

	~EffectStorage()
	{
		for (auto itor = mfxMtx.begin(); itor != mfxMtx.end(); ++itor)
			ReleaseCOM(itor->second);

		for (auto itor = mfxVtx.begin(); itor != mfxVtx.end(); ++itor)
			ReleaseCOM(itor->second);

		for (auto itor = mfxValue.begin(); itor != mfxValue.end(); ++itor)
			ReleaseCOM(itor->second);

		for (auto itor = mfxResource.begin(); itor != mfxResource.end(); ++itor)
			ReleaseCOM(itor->second);

		for (auto itor = mTech.begin(); itor != mTech.end(); ++itor)
			ReleaseCOM(itor->second);

		for (auto itor = mInputLayout.begin(); itor != mInputLayout.end(); ++itor)
			ReleaseCOM(itor->second);

		mfxMtx.clear();
		mfxVtx.clear();
		mfxValue.clear();
		mfxResource.clear();
		mTech.clear();
		mInputLayout.clear();

		ReleaseCOM(mFX);
	}
};

// ���� ����
class BufferType
{
	cCoreStorage* mCoreStorage = cCoreStorage::GetInstance();
public:
	// ����
	ID3D11Buffer* mVB;
	ID3D11Buffer* mIB;

	// �ν��Ͻ���
	map<string, ID3D11Buffer*> mInstancedBuffer;

	// ���̴� ���
	SHADER_TYPE mShaderMode;

	// �ش� ���ۿ� �� �� ����Ʈ
	map<string, InitMetaData*> mModelList;

public:
	BufferType()
	{
		initValue();

		// ����
		mVB = mIB = nullptr;
	}

	BufferType(SHADER_TYPE _ShaderMode)
	{
		initValue();

		// ����
		mVB = mIB = nullptr;

		// ���̴� ��� ����
		mShaderMode = _ShaderMode;
	}
	~BufferType()
	{
		initValue();

		ReleaseCOM(mVB);
		ReleaseCOM(mIB);

		// ����
		mVB = mIB = nullptr;
	}

	void initValue()
	{
		// �ν��Ͻ� ���� ����
		ClearInsBuf();

		// �� ����Ʈ ����
		mModelList.clear();
	}

	void ClearInsBuf()
	{
		// �ν��Ͻ� ���۰� �ִٸ�
		if (mInstancedBuffer.size())
		{
			// �ν��Ͻ� ���� ����
			for (auto itor = mInstancedBuffer.begin(); itor != mInstancedBuffer.end(); ++itor)
				ReleaseCOM(itor->second);
			mInstancedBuffer.clear();
		}
	}

	void ClearInsBuf(string& _name)
	{
		// �ν��Ͻ� ���۰� �ִٸ�
		if (mInstancedBuffer[_name])
		{
			// �ν��Ͻ� ���� ����
			ReleaseCOM(mInstancedBuffer[_name]);
		}
	}

	// ���� �ٽø����
	void ReMakeBuf(string& _name)
	{
		ClearInsBuf(_name);
		MakeInsBuf (_name);
	}

	// �� �߰�
	void AddModel(string _ModelName, InitMetaData* _ModelData)
	{
		mModelList[_ModelName] = _ModelData;
	}

	// �� �ٽ� �߰�
	void ReAddModel(string _ModelName, InitMetaData* _ModelData)
	{
		// ���� ��, �߰�
		mModelList.erase(_ModelName);
		mModelList[_ModelName] = _ModelData;
	}

	// ����
	void Build()
	{
		switch (mShaderMode)
		{
		case e_ShaderColor:
			Build_PC();
			break;

		case e_ShaderLight:
			Build_PN();
			break;
		case e_ShaderPongTex:
		case e_ShaderCartoonTex:
			Build_PNT();
			break;

		case e_ShaderPongTexAni:
			Build_PNT_Ani();
			break;

		case e_ShaderPongTexMap:
			Build_PNT_Map();
			break;

		case e_ShaderSkyBox:
			Build_SkyBox();
			break;

		default:
			break;
		}
	}

	//--------------------------------------------------------------------------------------------------//
	// ��ũ�� ����
	//--------------------------------------------------------------------------------------------------//
	// ��ũ�� �ν��Ͻ� ���� ����
	void MakeScreenInsBuf(InitMetaData* _Screen)
	{
		// �ν��Ͻ� ���� ����
		// ���� ��Ʈ������ �־�߸� ����
		if (_Screen->mObjData.size())
		{
			D3D11_BUFFER_DESC vbd;
			vbd.Usage               = D3D11_USAGE_DYNAMIC;
			vbd.ByteWidth           = sizeof(XMFLOAT4X4) * _Screen->mObjData.size();
			vbd.BindFlags           = D3D11_BIND_VERTEX_BUFFER; // ���ؽ�
			vbd.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
			vbd.MiscFlags           = 0;
			vbd.StructureByteStride = 0;

			if (_Screen->mObjData.size() != 1)
				cout << "��ũ�� ���۰� 1���� �ƴմϴ�!!" << endl;

			// �����Ҵ�
			//mInstancedBuffer[itor->second->mCreateName] = NULL;
			HR(mCoreStorage->md3dDevice->CreateBuffer(&vbd, 0, &mInstancedBuffer[_Screen->mCreateName]));
		}
	}

	// �ν��Ͻ� �� ������Ʈ
	void UpdateScreenIns(InitMetaData* _Screen)
	{
		// �ν��Ͻ� ���� ���� ( ���̳��� ���� )
		D3D11_MAPPED_SUBRESOURCE mappedData;

		// �ش� ���� ���� ��Ʈ������ ������
		if (_Screen->mObjData.size())
		{
			mCoreStorage->md3dImmediateContext->Map(mInstancedBuffer[_Screen->mCreateName], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);

			// << �ν��Ͻ� ���� >> �� "�������̽�"�� ���´�.
			XMFLOAT4X4* dataView = reinterpret_cast<XMFLOAT4X4*>(mappedData.pData);

			// �ø� ����
			// ��Ʈ���� ����ŭ ���
			auto tobjMtx = _Screen->mObjData;
			UINT i = -1;
			for (auto itor = tobjMtx.begin(); itor != tobjMtx.end(); ++itor)
			{
				dataView[++i] = itor->second.mWdMtx;
			}
			// �ν��Ͻ� ���� �ݱ�
			mCoreStorage->md3dImmediateContext->Unmap(mInstancedBuffer[_Screen->mCreateName], 0);
		}
	}

	//--------------------------------------------------------------------------------------------------//
	// �Ϲ� �ν��Ͻ� ����
	//--------------------------------------------------------------------------------------------------//
	// �ν��Ͻ� ���� ����
	void MakeInsBuf()
	{
		// �ش� ���̴��� �ִ� ��� �𵨵��� �ν��Ͻ� ���۸� ������ �Ѵ�.
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// ���� ��Ʈ������ �־�߸� ����
			if (itor->second->mObjData.size())
			{
				D3D11_BUFFER_DESC vbd;

				// �ִ� �� ����
				if (itor->second->mShaderMode == e_ShaderPongTexAni)
				{
					vbd.Usage               = D3D11_USAGE_DYNAMIC;
					vbd.ByteWidth           = sizeof(InsAni) * itor->second->mObjData.size();
					vbd.BindFlags           = D3D11_BIND_VERTEX_BUFFER; // ���ؽ�
					vbd.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
					vbd.MiscFlags           = 0;
					vbd.StructureByteStride = 0;
				}
				// �� �� ����
				else if (itor->second->mShaderMode == e_ShaderPongTexMap)
				{
					vbd.Usage               = D3D11_USAGE_DYNAMIC;
					vbd.ByteWidth           = sizeof(InsMap) * itor->second->mObjData.size();
					vbd.BindFlags           = D3D11_BIND_VERTEX_BUFFER; // ���ؽ�
					vbd.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
					vbd.MiscFlags           = 0;
					vbd.StructureByteStride = 0;
				}
				// �Ϲ� �� ����
				else
				{
					vbd.Usage               = D3D11_USAGE_DYNAMIC;
					vbd.ByteWidth           = sizeof(XMFLOAT4X4) * itor->second->mObjData.size();
					vbd.BindFlags           = D3D11_BIND_VERTEX_BUFFER; // ���ؽ�
					vbd.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
					vbd.MiscFlags           = 0;
					vbd.StructureByteStride = 0;
				}

				// �����Ҵ�
				HR(mCoreStorage->md3dDevice->CreateBuffer(&vbd, 0, &mInstancedBuffer[itor->second->mCreateName]));
			}
		}
	}

	// �ν��Ͻ� ���� ����
	void MakeInsBuf(string& _name)
	{
		// �� ����
		InitMetaData* _selectModel = mModelList[_name];

		// ���� ��Ʈ������ �־�߸� ����
		if (_selectModel->mObjData.size())
		{
			D3D11_BUFFER_DESC vbd;

			// �ִ� �� ����
			if (_selectModel->mShaderMode == e_ShaderPongTexAni)
			{
				vbd.Usage               = D3D11_USAGE_DYNAMIC;
				vbd.ByteWidth           = sizeof(InsAni) * _selectModel->mObjData.size();
				vbd.BindFlags           = D3D11_BIND_VERTEX_BUFFER; // ���ؽ�
				vbd.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
				vbd.MiscFlags           = 0;
				vbd.StructureByteStride = 0;
			}
			// �� �� ����
			else if (_selectModel->mShaderMode == e_ShaderPongTexMap)
			{
				vbd.Usage               = D3D11_USAGE_DYNAMIC;
				vbd.ByteWidth           = sizeof(InsMap) * _selectModel->mObjData.size();
				vbd.BindFlags           = D3D11_BIND_VERTEX_BUFFER; // ���ؽ�
				vbd.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
				vbd.MiscFlags           = 0;
				vbd.StructureByteStride = 0;
			}
			// �Ϲ� �� ����
			else
			{
				vbd.Usage               = D3D11_USAGE_DYNAMIC;
				vbd.ByteWidth           = sizeof(XMFLOAT4X4) * _selectModel->mObjData.size();
				vbd.BindFlags           = D3D11_BIND_VERTEX_BUFFER; // ���ؽ�
				vbd.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
				vbd.MiscFlags           = 0;
				vbd.StructureByteStride = 0;
			}

			// �����Ҵ�
			HR(mCoreStorage->md3dDevice->CreateBuffer(&vbd, 0, &mInstancedBuffer[_selectModel->mCreateName]));
		}
	}

	// �ν��Ͻ� �� ������Ʈ
	void UpdateIns()
	{
		// �ش� ���̴��� �ִ� ��� �𵨵��� �ν��Ͻ� ���۸� �����ؾ� �Ѵ�.
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// �ν��Ͻ� ���� ���� ( ���̳��� ���� )
			D3D11_MAPPED_SUBRESOURCE mappedData;

			// �ش� ���� ���� ��Ʈ������ ������
			if (itor->second->mObjData.size())
			{
				// �ν��Ͻ� ���� ����
				mCoreStorage->md3dImmediateContext->Map(mInstancedBuffer[itor->second->mCreateName], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);

				// �� ������ ��������
				map<int, ObjData>& tModelData = itor->second->mObjData;
				UINT i = -1;

				// �ִ� �� ����
				if (itor->second->mShaderMode == e_ShaderPongTexAni)
				{
					// << �ν��Ͻ� ���� >> �� "�������̽�"�� ���´�.
					InsAni* dataView = reinterpret_cast<InsAni*>(mappedData.pData);

					// ��ϵ� �� ����ŭ ��� (�ø� ����)
					for (auto itor2 = tModelData.begin(); itor2 != tModelData.end(); ++itor2)
					{
						// �� ��Ʈ����
						dataView[++i].mMtx = itor2->second.mWdMtx;

						// �� �ִ�����
						dataView[i].mInsAni.x = itor2->second.mAniType;    // ����� �ִ��� �ؽ�ó��ȣ
						dataView[i].mInsAni.y = itor2->second.mFrame;      // ������ ����
						dataView[i].mInsAni.z = itor2->second.mTexWidth;   // �ؽ�ó �ʺ�
						dataView[i].mInsAni.w = itor2->second.mTexHeight;  // �ؽ�ó ����
					}
				}
				// �� ���� ����
				else if (itor->second->mShaderMode == e_ShaderPongTexMap)
				{
					// << �ν��Ͻ� ���� >> �� "�������̽�"�� ���´�.
					InsMap* dataView = reinterpret_cast<InsMap*>(mappedData.pData);

					// ��ϵ� �� ����ŭ ��� (�ø� ����)
					for (auto itor2 = tModelData.begin(); itor2 != tModelData.end(); ++itor2)
					{
						// �� ��Ʈ����
						dataView[++i].mMtx = itor2->second.mWdMtx;

						// �� �ִ�����
						dataView[i].mInsTex.x = 0;						   // ����� ���� �ؽ�ó��ȣ
						dataView[i].mInsTex.y = itor2->second.mTexWidth;   // ������ �ʺ�
						dataView[i].mInsTex.z = itor2->second.mTexHeight;  // �ؽ�ó ����
					}
				}
				// �Ϲ� �� ����
				else
				{
					// << �ν��Ͻ� ���� >> �� "�������̽�"�� ���´�.
					XMFLOAT4X4* dataView = reinterpret_cast<XMFLOAT4X4*>(mappedData.pData);

					// ��ϵ� �� ����ŭ ��� (�ø� ����)
					for (auto itor2 = tModelData.begin(); itor2 != tModelData.end(); ++itor2)
					{
						dataView[++i] = itor2->second.mWdMtx;
					}
				}

				// �ν��Ͻ� ���� �ݱ�
				mCoreStorage->md3dImmediateContext->Unmap(mInstancedBuffer[itor->second->mCreateName], 0);
			}
		}
	}

	//-----------------------------------------------------------------------------------------------------------//
	// ����
	//-----------------------------------------------------------------------------------------------------------//
	// ��ũ�� ���� ����
	void Build_GScreen(InitMetaData* _Screen)
	{
		_Screen->mVertexOffset = 0;
		_Screen->mIndexOffset  = 0;


		// �� ���ؽ� ������ ���
		vector<VertexG> vertices; // �Ѱ����� �ű�� �۾� ( ���� ū ���� )
		UINT totalVertexCount = _Screen->Vertices.size();

		// ���� �ø���
		CalVtxINCAndSet(_Screen);

		// ��ü���� ������ �ø���
		vertices.resize(totalVertexCount);

		// ���� ū ���۷� ����
		// ���ͷ����Ͱ� ���鼭, ���ؽ� ũ�⸸ŭ ���Ѵ�.
		for (unsigned int i = 0; i < _Screen->Vertices.size(); ++i)
		{
			vertices[i].Pos     = _Screen->Vertices[i].Position;
			vertices[i].Tex     = _Screen->Vertices[i].TexUV;
			vertices[i].Pedding = XMFLOAT3(0.0f, 0.0f, 0.0f);
		}

		// ���ؽ� ���� �����
		D3D11_BUFFER_DESC vbd;
		vbd.Usage          = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth      = sizeof(VertexG)* totalVertexCount;
		vbd.BindFlags      = D3D11_BIND_VERTEX_BUFFER; // ���ؽ�
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags      = 0;

		D3D11_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = &vertices[0];
		HR(cCoreStorage::GetInstance()->md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));

		// �� �ε��� ������ ���
		UINT totalIndexCount = _Screen->Indices.size();

		// �ε��� ���� �����
		std::vector<UINT> indices;

		// ���� ū ���۷� ����
		// ���ͷ����Ͱ� ���鼭, �ε��� ũ�⸸ŭ ���Ѵ�.
		for (unsigned int i = 0; i < _Screen->Indices.size(); ++i)
		{
			indices.push_back(_Screen->Indices[i]);
		}

		// �ε��� ���� �����
		D3D11_BUFFER_DESC ibd;
		ibd.Usage          = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth      = sizeof(UINT)* totalIndexCount;
		ibd.BindFlags      = D3D11_BIND_INDEX_BUFFER; // �ε���
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags      = 0;

		D3D11_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = &indices[0];
		HR(mCoreStorage->md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
	}

private:
	// ���� ��Ÿ������( ������ ������ �ִ�) , ������ �� 
	void Build_PC()
	{
		// �Ѱ����� �ű�� �۾� ( ���� ū ���� )
		vector<VertexPC> vertices;

		// �� ���ؽ� ������ ���
		UINT totalVertexCount = 0;
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			totalVertexCount += itor->second->Vertices.size();
		}

		// �� �ε��� ������ ���
		UINT totalIndexCount = 0;
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			totalIndexCount += itor->second->Indices.size();
		}

		// ���� ���ؽ� ������ ���
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// ���� ���ؽ� ����
			int AfterVtxOffset = 0;

			// ó���̶��
			if (itor == mModelList.begin())
				itor->second->mVertexOffset = 0;
			else
				// ���Ͱ�, �� ���� �� ���Ѵ�.
				itor->second->mVertexOffset = (itor->second->mVertexOffset + AfterVtxOffset);

			// ���� ���ؽ� ����
			AfterVtxOffset = itor->second->mVertexOffset;
		}

		// ���� �ε��� ������ ���
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// ���� �ε��� ����
			int AfterIdxOffset = 0;

			// ó���̶��
			if (itor == mModelList.begin())
				itor->second->mIndexOffset = 0;
			else
				// ���Ͱ�, �� ���� �� ���Ѵ�.
				itor->second->mIndexOffset = (itor->second->mIndexOffset + AfterIdxOffset);

			// ���� �ε��� ����
			AfterIdxOffset = itor->second->mIndexOffset;
		}

		// ��ü���� ������ �ø���
		vertices.resize(totalVertexCount);

		// ���� ū ���۷� ����
		UINT k = 0;
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// ���ͷ����Ͱ� ���鼭, ���ؽ� ũ�⸸ŭ ���Ѵ�.
			for (unsigned int i = 0; i < itor->second->Vertices.size(); ++i, ++k)
			{
				vertices[k].Pos     = itor->second->Vertices[i].Position;
				vertices[k].Color   = itor->second->Vertices[i].Color;
				vertices[k].Pedding = 0.0f;			// �е� ��
			}
		}

		// ���ؽ� ���� �����
		D3D11_BUFFER_DESC vbd;
		vbd.Usage          = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth      = sizeof(VertexPC)* totalVertexCount;
		vbd.BindFlags      = D3D11_BIND_VERTEX_BUFFER; // ���ؽ�
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags      = 0;

		D3D11_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = &vertices[0];
		HR(cCoreStorage::GetInstance()->md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));

		// �ε��� ���� �����
		std::vector<UINT> indices;

		// ���� ū ���۷� ����
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// ���ͷ����Ͱ� ���鼭, �ε��� ũ�⸸ŭ ���Ѵ�.
			for (unsigned int i = 0; i < itor->second->Indices.size(); ++i)
			{
				indices.push_back(itor->second->Indices[i]);
			}
		}

		// �ε��� ���� �����
		D3D11_BUFFER_DESC ibd;
		ibd.Usage          = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth      = sizeof(UINT)* totalIndexCount;
		ibd.BindFlags      = D3D11_BIND_INDEX_BUFFER; // �ε���
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags      = 0;

		D3D11_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = &indices[0];
		HR(mCoreStorage->md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
	}
	void Build_PN()
	{
		// �Ѱ����� �ű�� �۾� ( ���� ū ���� )
		vector<VertexPN> vertices;

		// �� ���ؽ� ������ ���
		UINT totalVertexCount = 0;
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			totalVertexCount += itor->second->Vertices.size();
		}

		// �� �ε��� ������ ���
		UINT totalIndexCount = 0;
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			totalIndexCount += itor->second->Indices.size();
		}

		// ���� ���ؽ� ������ ���
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// ���� ���ؽ� ����
			int AfterVtxOffset = 0;

			// ó���̶��
			if (itor == mModelList.begin())
				itor->second->mVertexOffset = 0;
			else
				// ���Ͱ�, �� ���� �� ���Ѵ�.
				itor->second->mVertexOffset = (itor->second->mVertexOffset + AfterVtxOffset);

			// ���� ���ؽ� ����
			AfterVtxOffset = itor->second->mVertexOffset;
		}

		// ���� �ε��� ������ ���
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// ���� �ε��� ����
			int AfterIdxOffset = 0;

			// ó���̶��
			if (itor == mModelList.begin())
				itor->second->mIndexOffset = 0;
			else
				// ���Ͱ�, �� ���� �� ���Ѵ�.
				itor->second->mIndexOffset = (itor->second->mIndexOffset + AfterIdxOffset);

			// ���� �ε��� ����
			AfterIdxOffset = itor->second->mIndexOffset;
		}

		// �������� ���
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			CalNormalVtx(itor->second);
		}

		// ��ü���� ������ �ø���
		vertices.resize(totalVertexCount);

		// ���� ū ���۷� ����
		UINT k = 0;
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// ���ͷ����Ͱ� ���鼭, ���ؽ� ũ�⸸ŭ ���Ѵ�.
			for (unsigned int i = 0; i < itor->second->Vertices.size(); ++i, ++k)
			{
				vertices[k].Pos     = itor->second->Vertices[i].Position;
				vertices[k].Normal  = itor->second->Vertices[i].Normal;
				vertices[k].Pedding = XMFLOAT2(0.0f, 0.0f);			// �е� ��
			}
		}

		// ���ؽ� ���� �����
		D3D11_BUFFER_DESC vbd;
		vbd.Usage          = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth      = sizeof(VertexPN)* totalVertexCount;
		vbd.BindFlags      = D3D11_BIND_VERTEX_BUFFER; // ���ؽ�
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags      = 0;

		D3D11_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = &vertices[0];
		HR(cCoreStorage::GetInstance()->md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));

		// �ε��� ���� �����
		std::vector<UINT> indices;

		// ���� ū ���۷� ����
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// ���ͷ����Ͱ� ���鼭, �ε��� ũ�⸸ŭ ���Ѵ�.
			for (unsigned int i = 0; i < itor->second->Indices.size(); ++i)
			{
				indices.push_back(itor->second->Indices[i]);
			}
		}

		// �ε��� ���� �����
		D3D11_BUFFER_DESC ibd;
		ibd.Usage          = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth      = sizeof(UINT)* totalIndexCount;
		ibd.BindFlags      = D3D11_BIND_INDEX_BUFFER; // �ε���
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags      = 0;

		D3D11_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = &indices[0];
		HR(mCoreStorage->md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
	}
	void Build_PNT()
	{
		//-----------------------------------------------------------------------------------------------------------//
		// ���� ���ؽ� �����
		int AfterVtxOffset = 0;

		// ���� ���ؽ� ������ ���
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// ó���̶��
			if (AfterVtxOffset == 0)
			{
				// ���� ���ؽ� ����
				AfterVtxOffset = itor->second->mVertexOffset;
				itor->second->mVertexOffset = 0;
			}
			else
			{
				// ���Ͱ�, �� ���� �� ���Ѵ�.
				itor->second->mVertexOffset = AfterVtxOffset;
				AfterVtxOffset = itor->second->mVertexOffset + itor->second->Vertices.size();
			}
		}
		// ���� �ε��� �����
		int AfterIdxOffset = 0;

		// ���� �ε��� ������ ���
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// ó���̶��
			if (AfterIdxOffset == 0)
			{
				// ���� �ε��� ����
				AfterIdxOffset = itor->second->mIndexCount;
				itor->second->mIndexOffset = 0;
			}
			else
			{
				// ���Ͱ�, �� ���� �� ���Ѵ�.
				itor->second->mIndexOffset = AfterIdxOffset;
				AfterIdxOffset = itor->second->mIndexOffset + itor->second->mIndexCount;
			}
		}

		// �������� ���
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// itor�� �Ľ̵� ���̶�� �Ѿ��.
			if (itor->second->mModelType == e_ParsingModel)
				continue;

			// �������� ���
			CalNormalVtx(itor->second);
		}

		// ���� �ø��� & ���ؽ� ���۷� �۽�
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// itor�� �Ľ̵� ���̶�� �Ѿ��.
			if (itor->second->mModelType == e_ParsingModel)
				continue;

			// ���� �ø���
			CalVtxINCAndSet(itor->second);
		}

		// ź��Ʈ ���� ���
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// itor�� �Ľ̵� ���̶�� �Ѿ��.
			if (itor->second->mModelType == e_ParsingModel)
				continue;

			// ź��Ʈ ���� ���
			CalTangentSpace(itor->second);
		}

		// �� ���ؽ� ������ ���
		vector<VertexPNT> vertices; // �Ѱ����� �ű�� �۾� ( ���� ū ���� )
		UINT totalVertexCount = 0;
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			totalVertexCount += itor->second->Vertices.size();
		}

		// ��ü���� ������ �ø���
		vertices.resize(totalVertexCount);

		// ���� ū ���۷� ����
		UINT k = 0;
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// ���ͷ����Ͱ� ���鼭, ���ؽ� ũ�⸸ŭ ���Ѵ�.
			for (unsigned int i = 0; i < itor->second->Vertices.size(); ++i, ++k)
			{
				vertices[k].Pos      = itor->second->Vertices[i].Position;
				vertices[k].Tex      = itor->second->Vertices[i].TexUV;
				vertices[k].Normal   = itor->second->Vertices[i].Normal;
				vertices[k].Tangent  = itor->second->Vertices[i].TangentU;
				vertices[k].BiNormal = itor->second->Vertices[i].BiNormal;
				vertices[k].Pedding  = XMFLOAT2(0.0f, 0.0f);			// �е� ��
			}
		}

		// ���ؽ� ���� �����
		D3D11_BUFFER_DESC vbd;
		vbd.Usage          = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth      = sizeof(VertexPNT)* totalVertexCount;
		vbd.BindFlags      = D3D11_BIND_VERTEX_BUFFER; // ���ؽ�
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags      = 0;

		D3D11_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = &vertices[0];
		HR(cCoreStorage::GetInstance()->md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));


		// �� �ε��� ������ ���
		UINT totalIndexCount = 0;
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			totalIndexCount += itor->second->Indices.size();
		}

		// �ε��� ���� �����
		std::vector<UINT> indices;

		// ���� ū ���۷� ����
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// ���ͷ����Ͱ� ���鼭, �ε��� ũ�⸸ŭ ���Ѵ�.
			for (unsigned int i = 0; i < itor->second->Indices.size(); ++i)
			{
				indices.push_back(itor->second->Indices[i]);
			}
		}

		// �ε��� ���� �����
		D3D11_BUFFER_DESC ibd;
		ibd.Usage          = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth      = sizeof(UINT) * totalIndexCount;
		ibd.BindFlags      = D3D11_BIND_INDEX_BUFFER; // �ε���
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags      = 0;

		D3D11_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = &indices[0];
		HR(mCoreStorage->md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
	}
	void Build_PNT_Ani()
	{
		//-----------------------------------------------------------------------------------------------------------//
		// ���� ���ؽ� �����
		int AfterVtxOffset = 0;

		// ���� ���ؽ� ������ ���
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// ó���̶��
			if (AfterVtxOffset == 0)
			{
				// ���� ���ؽ� ����
				AfterVtxOffset = itor->second->mVertexOffset;
				itor->second->mVertexOffset = 0;
			}
			else
			{
				// ���Ͱ�, �� ���� �� ���Ѵ�.
				itor->second->mVertexOffset = AfterVtxOffset;
				AfterVtxOffset = itor->second->mVertexOffset + itor->second->Vertices.size();
			}
		}
		// ���� �ε��� �����
		int AfterIdxOffset = 0;

		// ���� �ε��� ������ ���
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// ó���̶��
			if (AfterIdxOffset == 0)
			{
				// ���� �ε��� ����
				AfterIdxOffset = itor->second->mIndexCount;
				itor->second->mIndexOffset = 0;
			}
			else
			{
				// ���Ͱ�, �� ���� �� ���Ѵ�.
				itor->second->mIndexOffset = AfterIdxOffset;
				AfterIdxOffset = itor->second->mIndexOffset + itor->second->mIndexCount;
			}
		}

		// �������� ���
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// itor�� �Ľ̵� ���̶�� �Ѿ��.
			if (itor->second->mModelType == e_ParsingModel)
				continue;

			// �������� ���
			CalNormalVtx(itor->second);
		}

		// ���� �ø��� & ���ؽ� ���۷� �۽�
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// itor�� �Ľ̵� ���̶�� �Ѿ��.
			if (itor->second->mModelType == e_ParsingModel)
				continue;

			// ���� �ø���
			CalVtxINCAndSet(itor->second);
		}

		// ź��Ʈ ���� ���
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// itor�� �Ľ̵� ���̶�� �Ѿ��.
			if (itor->second->mModelType == e_ParsingModel)
				continue;

			// ź��Ʈ ���� ���
			CalTangentSpace(itor->second);
		}


		// �� ���ؽ� ������ ���
		vector<VertexPNTAni> vertices; // �Ѱ����� �ű�� �۾� ( ���� ū ���� )
		UINT totalVertexCount = 0;
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			totalVertexCount += itor->second->Vertices.size();
		}

		// ��ü���� ������ �ø���
		vertices.resize(totalVertexCount);

		// ���� ū ���۷� ����
		UINT k = 0;
		int  count = 0;
		int  vtxCount = 0;
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// ���ͷ����Ͱ� ���鼭, ���ؽ� ũ�⸸ŭ ���Ѵ�. (���ؽ� ���� ��� �Ǵ� ��)
			for (unsigned int i = 0; i < itor->second->Vertices.size(); ++i, ++k)
			{
				vertices[k].Tex     = itor->second->Vertices[i].TexUV;
				vertices[k].VtxInfo = XMFLOAT3((float)i, (float)itor->second->Vertices.size(), 0.0f); // �е���

				// ���õ� ���� ����ġ ������
				vector<WeightVtx>& _sModelWeight = itor->second->weightVtx;

				// ������ ����ġ ����
				if (_sModelWeight.size() > i) // Ȥ�ø��� ������ ������ġ
				{
					for (int x = 0; x < 4; ++x)
					{
						// ����ġ 3����, �� �ε��� 4��
						switch (x)
						{
						case 0:
							if (_sModelWeight[i].Bone.size() >(unsigned int)x)
							{
								vertices[k].Weights.x      =       _sModelWeight[i].Bone[x].Weight;
								vertices[k].BoneIndices[x] = (UINT)_sModelWeight[i].Bone[x].ID;
							}
							else
							{
								vertices[k].Weights.x      = 0.0f;
								vertices[k].BoneIndices[x] = (UINT)0;
							}
							break;
						case 1:
							if (_sModelWeight[i].Bone.size() > (unsigned int)x)
							{
								vertices[k].Weights.y      =       _sModelWeight[i].Bone[x].Weight;
								vertices[k].BoneIndices[x] = (UINT)_sModelWeight[i].Bone[x].ID;
							}
							else
							{
								vertices[k].Weights.y      = 0.0f;
								vertices[k].BoneIndices[x] = (UINT)0;
							}
							break;
						case 2:
							if (_sModelWeight[i].Bone.size() > (unsigned int)x)
							{
								vertices[k].Weights.z      =       _sModelWeight[i].Bone[x].Weight;
								vertices[k].BoneIndices[x] = (UINT)_sModelWeight[i].Bone[x].ID;
							}
							else
							{
								vertices[k].Weights.z      = 0.0f;
								vertices[k].BoneIndices[x] = 0;
							}
							break;
						case 3:
							if (_sModelWeight[i].Bone.size() > (unsigned int)x)
								vertices[k].BoneIndices[x] = (UINT)_sModelWeight[i].Bone[x].ID;
							else
								vertices[k].BoneIndices[x] = (UINT)0;
							break;
						}
					}
				}
			}
		}

		// ���ؽ� ���� �����
		D3D11_BUFFER_DESC vbd;
		vbd.Usage          = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth      = sizeof(VertexPNTAni)* totalVertexCount;
		vbd.BindFlags      = D3D11_BIND_VERTEX_BUFFER; // ���ؽ�
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags      = 0;

		D3D11_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = &vertices[0];
		HR(cCoreStorage::GetInstance()->md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));


		// �� �ε��� ������ ���
		UINT totalIndexCount = 0;
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			totalIndexCount += itor->second->Indices.size();
		}

		// �ε��� ���� �����
		std::vector<UINT> indices;

		// ���� ū ���۷� ����
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// ���ͷ����Ͱ� ���鼭, �ε��� ũ�⸸ŭ ���Ѵ�.
			for (unsigned int i = 0; i < itor->second->Indices.size(); ++i)
			{
				indices.push_back(itor->second->Indices[i]);
			}
		}

		// �ε��� ���� �����
		D3D11_BUFFER_DESC ibd;
		ibd.Usage          = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth      = sizeof(UINT) * totalIndexCount;
		ibd.BindFlags      = D3D11_BIND_INDEX_BUFFER; // �ε���
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags      = 0;

		D3D11_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = &indices[0];
		HR(mCoreStorage->md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
	}
	void Build_PNT_Map()
	{
		//-----------------------------------------------------------------------------------------------------------//
		// ���� ���ؽ� �����

		int AfterVtxOffset = 0;

		// ���� ���ؽ� ������ ���
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// ó���̶��
			if (AfterVtxOffset == 0)
			{
				// ���� ���ؽ� ����
				AfterVtxOffset = itor->second->mVertexOffset;
				itor->second->mVertexOffset = 0;
			}
			else
			{
				// ���Ͱ�, �� ���� �� ���Ѵ�.
				itor->second->mVertexOffset = AfterVtxOffset;
				AfterVtxOffset = itor->second->mVertexOffset + itor->second->Vertices.size();
			}
		}
		// ���� �ε��� �����
		int AfterIdxOffset = 0;

		// ���� �ε��� ������ ���
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// ó���̶��
			if (AfterIdxOffset == 0)
			{
				// ���� �ε��� ����
				AfterIdxOffset = itor->second->mIndexCount;
				itor->second->mIndexOffset = 0;
			}
			else
			{
				// ���Ͱ�, �� ���� �� ���Ѵ�.
				itor->second->mIndexOffset = AfterIdxOffset;
				AfterIdxOffset = itor->second->mIndexOffset + itor->second->mIndexCount;
			}
		}

		// �������� ���
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// itor�� �Ľ̵� ���̶�� �Ѿ��.
			if (itor->second->mModelType == e_ParsingModel)
				continue;

			// �������� ���
			CalNormalVtx(itor->second);
		}

		// ���� �ø��� & ���ؽ� ���۷� �۽�
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// itor�� �Ľ̵� ���̶�� �Ѿ��.
			if (itor->second->mModelType == e_ParsingModel)
				continue;

			// ���� �ø���
			CalVtxINCAndSet(itor->second);
		}

		// ź��Ʈ ���� ���
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// itor�� �Ľ̵� ���̶�� �Ѿ��.
			if (itor->second->mModelType == e_ParsingModel)
				continue;

			// ź��Ʈ ���� ���
			CalTangentSpace(itor->second);
		}


		// �� ���ؽ� ������ ���
		vector<VertexPNTMap> vertices; // �Ѱ����� �ű�� �۾� ( ���� ū ���� )
		UINT totalVertexCount = 0;
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			totalVertexCount += itor->second->Vertices.size();
		}

		// ��ü���� ������ �ø���
		vertices.resize(totalVertexCount);

		// ���� ū ���۷� ����
		UINT k = 0;
		int  count = 0;
		int  vtxCount = 0;
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// ���ͷ����Ͱ� ���鼭, ���ؽ� ũ�⸸ŭ ���Ѵ�. (���ؽ� ���� ��� �Ǵ� ��)
			for (unsigned int i = 0; i < itor->second->Vertices.size(); ++i, ++k)
			{
				vertices[k].Tex     = itor->second->Vertices[i].TexUV;
				vertices[k].VtxInfo = XMFLOAT2((float)i, (float)itor->second->Vertices.size()); // ���ؽ� ����
			}
		}

		// ���ؽ� ���� �����
		D3D11_BUFFER_DESC vbd;
		vbd.Usage          = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth      = sizeof(VertexPNTMap)* totalVertexCount;
		vbd.BindFlags      = D3D11_BIND_VERTEX_BUFFER; // ���ؽ�
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags      = 0;

		D3D11_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = &vertices[0];
		HR(cCoreStorage::GetInstance()->md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));


		// �� �ε��� ������ ���
		UINT totalIndexCount = 0;
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			totalIndexCount += itor->second->Indices.size();
		}

		// �ε��� ���� �����
		std::vector<UINT> indices;

		// ���� ū ���۷� ����
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// ���ͷ����Ͱ� ���鼭, �ε��� ũ�⸸ŭ ���Ѵ�.
			for (unsigned int i = 0; i < itor->second->Indices.size(); ++i)
			{
				indices.push_back(itor->second->Indices[i]);
			}
		}

		// �ε��� ���� �����
		D3D11_BUFFER_DESC ibd;
		ibd.Usage          = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth      = sizeof(UINT) * totalIndexCount;
		ibd.BindFlags      = D3D11_BIND_INDEX_BUFFER; // �ε���
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags      = 0;

		D3D11_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = &indices[0];
		HR(mCoreStorage->md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
	}
	// ���� ��Ÿ������( ������ ������ �ִ�) , ������ �� 
	void Build_SkyBox()
	{
		// �Ѱ����� �ű�� �۾� ( ���� ū ���� )
		vector<VertexSkyBox> vertices;

		// �� ���ؽ� ������ ���
		UINT totalVertexCount = 0;
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			totalVertexCount += itor->second->Vertices.size();
		}

		// �� �ε��� ������ ���
		UINT totalIndexCount = 0;
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			totalIndexCount += itor->second->Indices.size();
		}

		// ���� ���ؽ� ������ ���
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// ���� ���ؽ� ����
			int AfterVtxOffset = 0;

			// ó���̶��
			if (itor == mModelList.begin())
				itor->second->mVertexOffset = 0;
			else
				// ���Ͱ�, �� ���� �� ���Ѵ�.
				itor->second->mVertexOffset = (itor->second->mVertexOffset + AfterVtxOffset);

			// ���� ���ؽ� ����
			AfterVtxOffset = itor->second->mVertexOffset;
		}

		// ���� �ε��� ������ ���
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// ���� �ε��� ����
			int AfterIdxOffset = 0;

			// ó���̶��
			if (itor == mModelList.begin())
				itor->second->mIndexOffset = 0;
			else
				// ���Ͱ�, �� ���� �� ���Ѵ�.
				itor->second->mIndexOffset = (itor->second->mIndexOffset + AfterIdxOffset);

			// ���� �ε��� ����
			AfterIdxOffset = itor->second->mIndexOffset;
		}

		// ��ü���� ������ �ø���
		vertices.resize(totalVertexCount);

		// ���� ū ���۷� ����
		UINT k = 0;
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// ���ͷ����Ͱ� ���鼭, ���ؽ� ũ�⸸ŭ ���Ѵ�.
			for (unsigned int i = 0; i < itor->second->Vertices.size(); ++i, ++k)
			{
				vertices[k].Pos = itor->second->Vertices[i].Position;
				vertices[k].Pedding = 0.0f;			// �е� ��
			}
		}

		// ���ؽ� ���� �����
		D3D11_BUFFER_DESC vbd;
		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = sizeof(VertexSkyBox) * totalVertexCount;
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // ���ؽ�
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = &vertices[0];
		HR(cCoreStorage::GetInstance()->md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));

		// �ε��� ���� �����
		std::vector<UINT> indices;

		// ���� ū ���۷� ����
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// ���ͷ����Ͱ� ���鼭, �ε��� ũ�⸸ŭ ���Ѵ�.
			for (unsigned int i = 0; i < itor->second->Indices.size(); ++i)
			{
				indices.push_back(itor->second->Indices[i]);
			}
		}

		// �ε��� ���� �����
		D3D11_BUFFER_DESC ibd;
		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = sizeof(UINT) * totalIndexCount;
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER; // �ε���
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = &indices[0];
		HR(mCoreStorage->md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
	}
	// ź��Ʈ ���� ���
	void CalTangentSpace(InitMetaData* _MetaData)
	{
		// ���� ����
		vector<XMVECTOR> tan1(_MetaData->Vertices.size());
		vector<XMVECTOR> tan2(_MetaData->Vertices.size());

		// �� ���� ��ŭ
		for (int fi = 0; fi < (int)_MetaData->Indices.size() / 3; ++fi)
		{
			//	�ﰢ�� �����͸� ��´�.
			WORD index1 = _MetaData->Indices[fi * 3 + 0];
			WORD index2 = _MetaData->Indices[fi * 3 + 1];
			WORD index3 = _MetaData->Indices[fi * 3 + 2];

			//	�ﰢ���� �ش��ϴ� ����3���� ���Ѵ�.
			XMVECTOR Vtx1 = XMLoadFloat3(&_MetaData->Vertices[index1].Position);
			XMVECTOR Vtx2 = XMLoadFloat3(&_MetaData->Vertices[index2].Position);
			XMVECTOR Vtx3 = XMLoadFloat3(&_MetaData->Vertices[index3].Position);

			// �ﰢ���� �ش��ϴ� �ؽ��� ����3���� ���Ѵ�.
			XMVECTOR TexVtx1 = XMLoadFloat2(&_MetaData->Vertices[index1].TexUV);
			XMVECTOR TexVtx2 = XMLoadFloat2(&_MetaData->Vertices[index2].TexUV);
			XMVECTOR TexVtx3 = XMLoadFloat2(&_MetaData->Vertices[index3].TexUV);

			// ź��Ʈ xyz ���ϱ�
			float x1 = XMVectorGetX(Vtx2) - XMVectorGetX(Vtx1);
			float x2 = XMVectorGetX(Vtx3) - XMVectorGetX(Vtx1);
			float y1 = XMVectorGetY(Vtx2) - XMVectorGetY(Vtx1);
			float y2 = XMVectorGetY(Vtx3) - XMVectorGetY(Vtx1);
			float z1 = XMVectorGetZ(Vtx2) - XMVectorGetZ(Vtx1);
			float z2 = XMVectorGetZ(Vtx3) - XMVectorGetZ(Vtx1);

			// ź��Ʈ w ���ϱ�
			float s1 = XMVectorGetX(TexVtx2) - XMVectorGetX(TexVtx1);
			float s2 = XMVectorGetX(TexVtx3) - XMVectorGetX(TexVtx1);
			float t1 = XMVectorGetY(TexVtx2) - XMVectorGetY(TexVtx1);
			float t2 = XMVectorGetY(TexVtx3) - XMVectorGetY(TexVtx1);

			// ���� ����
			XMVECTOR sdir;
			XMVECTOR tdir;
			if ((s1 * t2 - s2 * t1) == 0.0f)
			{
				sdir = XMLoadFloat3(&XMFLOAT3(1.0f, 0.0f, 0.0f));
				tdir = XMLoadFloat3(&XMFLOAT3(0.0f, 1.0f, 0.0f));
			}
			else
			{
				// ���� ����
				float r = 1.0F / (s1 * t2 - s2 * t1);
				sdir = XMLoadFloat3(&XMFLOAT3((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r));
				tdir = XMLoadFloat3(&XMFLOAT3((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r));
			}

			// �� ����
			tan1[index1] += sdir;
			tan1[index2] += sdir;
			tan1[index3] += sdir;

			tan2[index1] += tdir;
			tan2[index2] += tdir;
			tan2[index3] += tdir;
		}

		// �� ����
		for (int v = 0; v < (int)_MetaData->Vertices.size(); ++v)
		{
			const XMVECTOR& n = XMLoadFloat3(&_MetaData->Vertices[v].Normal);
			const XMVECTOR& t = tan1[v];

			// Gram-Schmidt orthogonalize
			// ź��Ʈ Vtx �����ϱ� (xyz)
			XMStoreFloat3(&_MetaData->Vertices[v].TangentU, XMVector3Normalize((t - n * XMVector3Dot(n, t))));

			// Calculate handedness
			//// �ؽ�ó ��ǥ ���⺤�� ���Ѵ�. (w) <-- �ϴ� �ȱ��� (���߿� ���ؼ� ����)
			float TangentU = (XMVectorGetX(XMVector3Dot(XMVector3Cross(n, t), tan2[v])) < 0.0F) ? -1.0f : 1.0f;

			// �����Ͽ� ����Ѵ�
			XMVECTOR BiNormal = XMVector3Cross(XMLoadFloat3(&_MetaData->Vertices[v].TangentU), n) * TangentU;

			// ��� �� ����
			XMStoreFloat3(&_MetaData->Vertices[v].BiNormal, BiNormal);
		}

		tan1.clear();
		tan2.clear();
	}


	// ���� �����ڵ� 1

	//// ���� �ø��� & ���ؽ� ���۷� �۽�
	////template <class T>
	////void CalNormalVtx(T& _vertices, cTestModel::InitMetaData& _MetaData)
	//void CalVtxINCAndSet(vector<VertexPNT>& vertices, cTestModel::InitMetaData& _MetaData)
	//{
	//
	//	// ��Ÿ�����͸� �����ؼ�, ������ �÷�����Ѵ�.
	//	// �ǵ���: ���ؽ� ��ǥ��, uv��ǥ�� ����ġ �Ҽ� �ִٴ� �Ϳ��� ���
	//	//         �׷��Ƿ�, ���ؽ� ��ǥ�� uv ��ǥ�� ���� �̾Ƴ�, �ΰ��� ��ġ�ϸ� ����ΰ�,
	//	//         ����ġ��, ���ؽ� ��ǥ�� �״�� ����ϰ�, uv ��ǥ�� �����Ͽ� ������ �߰��� �Ĺ̿� ��ġ�Ѵ�.
	//	//         ����, �߰��� ���������� ��������, �� �ε�������Ʈ�� �ٽ� �ۼ��Ѵ�.
	//	
	//	// *�ٽ�: uv ��ǥ �ֱ���, ����ִ� �������� Ȯ��
	//	//        1) ����ִ� �����̶�� ����,
	//	//        2) ��� ���� �ʴ� �����̶��, ��
	//	//           -> �������� uv ��ǥ�� ��ġ�Ѵٸ�, �ƹ��۾� ���ϰ�, ���� ��ǥ��
	//	//           -> �������� uv ��ǥ�� ��ġ���� �ʴ´ٸ�, ���� ���� ( ���ؽ� ��ǥ ����, uv ��ǥ ���� )
	//	//        3) ������ �߰��ϸ�, ���� �˻��ϰ� �ִ� �� �ε�����, �ش� ���� ��ȣ�� �ִ´�.
	//
	//	// ���� �ø��⿡ ���Ǵ� ����
	//	vector<TexUV> _TexUV;
	//
	//	// ���� �����Ǿ��ִ� ���� ��
	//	unsigned int CntVtx = _MetaData->Vertices.size();
	//	for (unsigned int v = 0; v < CntVtx; ++v)
	//	{
	//		// ���� �ε��� ũ�⸸ŭ �ݺ� ( �ؽ�ó �ε����� �Ϲ������δ� �����ϹǷ�, ������ �� ó���� ���� ó�� �ʿ� )
	//		for (unsigned int fi = 0; fi < _MetaData->Indices.size(); ++fi)
	//		{
	//			// ������� ������ ������ȣ �ȿ� �ش� ������ ���� ������ �ֳ�
	//			if (_MetaData->Indices[fi] == v)
	//				CalTexVtx(_MetaData, v, _MetaData->Indices[fi], _MetaData->TexIndices[fi], _TexUV);
	//		}// �ݺ�
	//		//�ش� ���ؽ��� ���� UV��ȣ, ������ ������ȣ Ŭ����
	//		_TexUV.clear();
	//	}// �ݺ�
	//}
	//
	//void CalTexVtx(cTestModel::InitMetaData& _MetaData, int v, UINT& Modelfi, UINT& Texfi, vector<TexUV>& _TexUV)
	//{
	//	// �� ����
	//	Vertex  & ThisModel = _MetaData->Vertices[v];
	//	XMFLOAT3& ThisTex   = _MetaData->TexVertices[Texfi];
	//
	//	// ���� �˻��ϰ� �ִ� ���ؽ� ������ Uv������ ����°�?(����� ������ ����, �ʱ� ���� ���ΰ�?)
	//	if (ThisModel.TexUV.x == FLT_MAX || ThisModel.TexUV.y == FLT_MAX)
	//	{
	//		// ��
	//		// �� uv <= �ؽ�ó uv;
	//		ThisModel.TexUV.x = ThisTex.x;
	//		ThisModel.TexUV.y = ThisTex.y;
	//
	//		// ����� uv�� ���ؽ� ����
	//		TexUV tUv;	
	//		tUv.Uv  = Texfi;
	//		tUv.Vtx = v;
	//		_TexUV.push_back(tUv);
	//	}
	//	else
	//	{
	//		// �ƴϿ�
	//		// ����� ����� �ִٸ�,
	//		// �߰��� UV�� �̹� �߰� �� UV�� �ƴѰ�?
	//		// - �� �ø��ʿ䰡 ���⿡ �ε����� ����
	//
	//		//�˻�
	//		UINT ItorUV = 0;
	//		bool UseUVCheck = false;
	//		for (unsigned int i = 0; i < _TexUV.size(); ++i)
	//		{
	//			if (_TexUV[i].Uv == Texfi) //����� ���ؽ���, tex�ε����� ����Ű�� ���ؽ��� ������?
	//			{
	//				ItorUV     = i;
	//				UseUVCheck = true;
	//				break;
	//			}
	//		}
	//
	//		//�˻� ���
	//		if (!UseUVCheck)
	//		{
	//			// �ε���[���� �ε��� ��ȣ].���° ���� = ���� �߰� �� ���ؽ� ������ ��ȣ
	//			Modelfi = _MetaData->Vertices.size();
	//
	//			// �� - ���ο� �����̴�
	//			// ���� �ø���
	//			Vertex vtx =  ThisModel;
	//			vtx.TexUV.x = ThisTex.x;
	//			vtx.TexUV.y = ThisTex.y;
	//			_MetaData->Vertices.push_back(vtx);
	//
	//			// ����� �������� ǥ��
	//			// ���� �߰��� ���� ���� = ���� �߰��� ������ �׻� �� ���� ����Ʈ�� �������� ���� ���̹Ƿ�.
	//			TexUV tUv;
	//			tUv.Uv  = Texfi;
	//			tUv.Vtx = _MetaData->Vertices.size(); // -1 �Ϻη� ����
	//			_TexUV.push_back(tUv);
	//		}
	//		else
	//		{// �ƴϿ� - �߰��� �����̴�
	//			Modelfi = _TexUV[ItorUV].Vtx;
	//		}
	//	}
	//	return;
	//}

	// ���� �����ڵ� 2
	void CalVtxINCAndSet(InitMetaData* _MetaData)
	{
		// ��Ÿ�����͸� �����ؼ�, ������ �÷�����Ѵ�.
		// �ǵ���: ���ؽ� ��ǥ��, uv��ǥ�� ����ġ �Ҽ� �ִٴ� �Ϳ��� ���
		//         �׷��Ƿ�, ���ؽ� ��ǥ�� uv ��ǥ�� ���� �̾Ƴ�, �ΰ��� ��ġ�ϸ� ����ΰ�,
		//         ����ġ��, ���ؽ� ��ǥ�� �״�� ����ϰ�, uv ��ǥ�� �����Ͽ� ������ �߰��� �Ĺ̿� ��ġ�Ѵ�.
		//         ����, �߰��� ���������� ��������, �� �ε�������Ʈ�� �ٽ� �ۼ��Ѵ�.
		//
		// *�ٽ�: uv ��ǥ �ֱ���, ����ִ� �������� Ȯ��
		//        1) ����ִ� �����̶�� ����,
		//        2) ��� ���� �ʴ� �����̶��, ��
		//           -> �������� uv ��ǥ�� ��ġ�Ѵٸ�, �ƹ��۾� ���ϰ�, ���� ��ǥ��
		//           -> �������� uv ��ǥ�� ��ġ���� �ʴ´ٸ�, ���� ���� ( ���ؽ� ��ǥ ����, uv ��ǥ ���� )
		//        3) ������ �߰��ϸ�, ���� �˻��ϰ� �ִ� �� �ε�����, �ش� ���� ��ȣ�� �ִ´�.


		// ���� �ε��� ũ�⸸ŭ �ݺ� ( �ؽ�ó �ε����� �Ϲ������δ� �����ϹǷ�, ������ �� ó���� ���� ó�� �ʿ� )
		for (unsigned int i = 0; i < _MetaData->Indices.size(); ++i)
		{
			// ���� �ε��� ��ȣ�� �˻��Ѵ�.
			UINT _ModelIdx = _MetaData->Indices[i];    //  ���� ���� �ε���
			UINT _TexIdx   = _MetaData->TexIndices[i]; // �ؽ�ó�� ���� �ε���

			// ������ �ε��� ��ȣ�� �ش��ϴ� uv���� ������.
			XMFLOAT2& ModelTexUV = _MetaData->Vertices[_ModelIdx].TexUV; //   �� �ε����� UV ��
			XMFLOAT3&      TexUV = _MetaData->TexVertices[_TexIdx];      // �ؽ�ó �ε����� UV ��

			// ���� ���� �ε����� ����Ű�� ���ؽ��� uv�� ����ִ°�?
			if (ModelTexUV.x == FLT_MAX || ModelTexUV.y == FLT_MAX)
			{
				// ����ִٸ�,
				// �ؽ�ó uv �� �ٷ� ����
				ModelTexUV.x = TexUV.x;
				ModelTexUV.y = TexUV.y;
			}
			else
			{
				// ��� ���� �ʴٸ�
				// ���� ���� UV�� ���� �ؽ�ó�� UV�� ��ġ�ϴ°�?
				if (ModelTexUV.x == TexUV.x && ModelTexUV.y == TexUV.y)
				{
					// ��ġ�Ѵٸ�, 

				}
				else
				{
					// ��ġ���� �ʴ´ٸ�, ���� �ø���
					// �ش� ���ؽ��� ������ ������
					Vertex ModelVtx = _MetaData->Vertices[_ModelIdx];

					// �ؽ�ó ���� ����
					ModelTexUV.x = TexUV.x;
					ModelTexUV.y = TexUV.y;

					// ����
					_MetaData->Vertices.push_back(ModelVtx);
				}
			}
		}
	}

	// ���� ���
	void CalNormalVtx(InitMetaData* _MetaData)
	{
		vector< vector<XMVECTOR> > face_normal;

		//�� ���
		face_normal.resize(_MetaData->Indices.size() / 3);
		for (unsigned int i = 0; i < face_normal.size(); ++i)
			face_normal[i].resize(2);

		//-----------------------------------//
		//�� ��ֹ���
		//-----------------------------------//
		//�ﰢ�� ������ŭ �ݺ�
		for (int fi = 0; fi < (int)_MetaData->Indices.size() / 3; ++fi)
		{
			//	�ﰢ�� �����͸� ��´�.
			WORD index1 = _MetaData->Indices[fi * 3 + 0];
			WORD index2 = _MetaData->Indices[fi * 3 + 1];
			WORD index3 = _MetaData->Indices[fi * 3 + 2];

			//	�ﰢ���� �ش��ϴ� ����3���� ���Ѵ�.
			XMVECTOR vtx1 = XMLoadFloat3(&_MetaData->Vertices[index1].Position);
			XMVECTOR vtx2 = XMLoadFloat3(&_MetaData->Vertices[index2].Position);
			XMVECTOR vtx3 = XMLoadFloat3(&_MetaData->Vertices[index3].Position);

			// �麤���� �߰� ������ �����Ѵ�.
			face_normal[fi][0] = vtx1 + vtx2 + vtx3;

			//	�Ѱ� �������� �ٸ� �ΰ� ������ ���ϴ� ���⺤�� �ΰ��� ���Ѵ�.
			XMVECTOR vtx_dir1 = vtx2 - vtx1;
			XMVECTOR vtx_dir2 = vtx3 - vtx1;

			//	���� ���� 2���� ����ȭ �Ѵ�.
			vtx_dir1 = XMVector3Normalize(vtx_dir1);
			vtx_dir2 = XMVector3Normalize(vtx_dir2);

			//	�����Ͽ� �������͸� ���Ѵ�.
			XMVECTOR normal_dir = XMVector3Cross(vtx_dir1, vtx_dir2);

			// �麤���� ��ģ ������ �����Ѵ�.
			face_normal[fi][1] = face_normal[fi][0] + normal_dir * 1.0f;

		} //�ݺ�

		//-----------------------------------//
		//���� ��ֹ���
		//-----------------------------------//
		//����������ŭ �ݺ� 
		for (unsigned int vi = 0; vi < _MetaData->Vertices.size(); ++vi) //<- ������ȣ 0���� �����˻�
		{
			//	�������� 0,0,0 �ʱ�ȭ 
			XMVECTOR result_vec = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

			//	�������� 0 �ʱ�ȭ 
			int result_count = 0;

			//	�ﰢ�� ������ŭ �ݺ�
			for (unsigned int fi = 0; fi < _MetaData->Indices.size() / 3; ++fi)
			{
				//		�ﰢ�� �����͸� ��´�.
				WORD index1 = _MetaData->Indices[fi * 3 + 0];
				WORD index2 = _MetaData->Indices[fi * 3 + 1];
				WORD index3 = _MetaData->Indices[fi * 3 + 2];

				//		�ﰢ�� ������ �߿� ���� �ε����� �ճ�?
				if ((index1 == (WORD)vi) || (index2 == (WORD)vi) || (index3 == (WORD)vi)) //<- vi�� ������ȣ, index���� ������ ������ȣ�� ���ִ�.
				{   //		��
					//			�������� �� �� �������͸� ���Ѵ�.
					XMVECTOR tmp_dir = face_normal[fi][1] - face_normal[fi][0]; //���⺤�͸� �����ؾ���.
					tmp_dir = XMVector3Normalize(tmp_dir);

					// ��� ���Ϳ� ����
					result_vec += tmp_dir;

					// �������� 1 ����.
					++result_count;
				}
			}//	�ݺ�
			//	�������� /= ��������
			result_vec /= (float)result_count;

			//��ֶ�����
			result_vec = XMVector3Normalize(result_vec);

			//����(����)�� �������� ����
			XMStoreFloat3(&_MetaData->Vertices[vi].Normal, result_vec);

		}//�ݺ�

		// �ڿ� ����
		// �ε���
		for (unsigned int i = 0; i < face_normal.size(); ++i)
		{
			face_normal[i].clear();
		}
		face_normal.clear();
	}
};

// ������ �Ž� ������
class MyMeshData
{
public:
	// ���� ����
	vector<Vertex> vertices;

	// �ε��� ����Ʈ
	vector<XMFLOAT3> indices;

	// �ִ� ������ ( �� ��ü )
	AniData mAniData;

	// ����ġ ������
	vector<WeightVtx> weightVtx;

	// ������
	UINT vertexOffset;
	UINT indexOffset;

	// ī��Ʈ
	UINT indexCount;

	// ���� ����
	MyMat mMyMat;

	// TM ���
	XMFLOAT4X4 mTMLocalMtx;
	XMFLOAT4X4 mTMWorldMtx;

	// �ٿ�� �ڽ�
	BoundBox mBoundingBox;

	//--------------------------------------------------//
	// ������Ʈ �ĺ� ����
	//--------------------------------------------------//

	// �̸�
	char mMainName[BUF_SIZE];

	// ������Ʈ�̸�
	int  mObjID;
	char mObjName[BUF_SIZE];
	char mObjClass[BUF_SIZE];

	// ���� Ŭ���� �̸�
	int  mParentID;
	char mParentName[BUF_SIZE];

public:
	MyMeshData()
	{
		InitClass();
	}

	~MyMeshData()
	{
		ClearClass();
	}

	void InitClass()
	{
		mMyMat.InitClass();
	}

	void ClearClass()
	{
		mMyMat   .ClearClass();
		mAniData .ClearClass();
		vertices .clear();
		indices  .clear();
		weightVtx.clear();
		
		mObjID = mParentID = vertexOffset = indexOffset = indexCount = 0;

		memset(&mMainName  , '\0', sizeof(mMainName));
		memset(&mObjName   , '\0', sizeof(mObjName));
		memset(&mObjClass  , '\0', sizeof(mObjClass));
		memset(&mParentName, '\0', sizeof(mParentName));
	}
};

// �θ� ������ ������ �ִ� ��
class BoneParentData
{
public:
	string mName;    // �� �θ��� �̸�
	XMFLOAT4X4 mLAP; // ���� �Ʒ� �ڽĿ��� ���� ��Ʈ����
public:
	BoneParentData()
	{
		ClearClass();
	}

	~BoneParentData()
	{
		mName.clear();
	}

private:
	void ClearClass()
	{
		XMMATRIX I = XMMatrixIdentity();
		XMStoreFloat4x4(&mLAP, I);

		mName.clear();
	}
};


// �� ������
class NodeBone
{
public:
	// TM ���
	XMFLOAT4X4 mTMLocalMtx;
	XMFLOAT4X4 mTMWorldMtx;

	// �� ����(B��)
	XMFLOAT4X4 mInvWorldTMMtx;

	// �ִ� ������
	AniData mAniData;

	// �ٿ�� �ڽ�
	BoundBox mBoundingBox;

	//--------------------------------------------------//
	// ������Ʈ �ĺ� ����
	//--------------------------------------------------//
	// ������Ʈ�̸�
	int  mObjID;
	char mObjName[BUF_SIZE];
	char mObjClass[BUF_SIZE];

	// ���� Ŭ���� �̸�
	int  mParentID;
	char mParentName[BUF_SIZE];
public:
	NodeBone()
	{
		mParentID = mObjID = -1;

		memset(&mObjName   , '\0', sizeof(mObjName));
		memset(&mObjClass  , '\0', sizeof(mObjClass));
		memset(&mParentName, '\0', sizeof(mParentName));

		XMMATRIX I = XMMatrixIdentity();
		XMStoreFloat4x4(&mTMLocalMtx, I);
		XMStoreFloat4x4(&mTMWorldMtx, I);
		XMStoreFloat4x4(&mInvWorldTMMtx, I);
	}

	// �ִ� ������
	XMFLOAT4X4 getAniMtx(int& _key)
	{
		XMFLOAT4X4 mAniMtx;
		XMMATRIX I = XMMatrixIdentity();
		XMStoreFloat4x4(&mAniMtx, I);

		//XMFLOAT4 _CRot = mAniData.Quaternion[_key].Vtx;
		//XMFLOAT3 _CPos = mAniData.Position[_key].Vtx;
		//
		//mAniData.Quaternion[_key].Vtx.x = _CRot.y;
		//mAniData.Quaternion[_key].Vtx.y = _CRot.z;
		//mAniData.Quaternion[_key].Vtx.z = _CRot.y;
		//mAniData.Position[_key].Vtx.x   = _CPos.y;
		//mAniData.Position[_key].Vtx.y   = _CPos.x;
		//mAniData.Position[_key].Vtx.z   = _CPos.z;
		
		// �ִϸ��̼� ������ ���
		//XMVECTOR _scaleKey = XMLoadFloat3(&mAniData.Scale[_key].Vtx);        // ������ ��
		XMVECTOR _rotKey   = XMLoadFloat4(&mAniData.Quaternion[_key].Vtx);   // ȸ��
		//XMVECTOR _posKey   = XMLoadFloat3(&mAniData.Position[_key].Vtx);     // ��ġ
		//XMVECTOR _zero     = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);			 // ȸ�� ����

		// ���ɺ�ȯ ���		
		//XMStoreFloat4x4(&mAniMtx, XMMatrixAffineTransformation(_scaleKey, _zero, _rotKey, _posKey));

		// �־��� ������� ȸ������� ���Ѵ�.
		XMStoreFloat4x4(&mAniMtx, XMMatrixRotationQuaternion(_rotKey));

		// �̵� ����
		mAniMtx._41 = mAniData.Position[_key].Vtx.x;
		mAniMtx._42 = mAniData.Position[_key].Vtx.y;
		mAniMtx._43 = mAniData.Position[_key].Vtx.z;
		
		// ������ ����
		mAniMtx._11 *= mAniData.Scale[_key].Vtx.x;
		mAniMtx._22 *= mAniData.Scale[_key].Vtx.y;
		mAniMtx._33 *= mAniData.Scale[_key].Vtx.z;

		return mAniMtx;
	}
};

// ��Ű�� Ʈ��
class SkinTree
{
public:
	// �θ� ������ (���� ������ / ��ĸ�)
	BoneParentData mParentData;

	// �� ������ (������ ����)
	map<string, NodeBone> mData;

	// �ڽ� ������ (������ ����)
	map<string, SkinTree> mChildData;

public:
	SkinTree()
	{
	}
	~SkinTree()
	{
		mData.clear();
		mChildData.clear();
	}

	// ��Ų��� �����, ���� ����ϱ� (�ڽ� ��)
	void MakeSkin(int _AniPoint, SkinTree& _SkinTree, vector< map<string, XMFLOAT4X4>>& _SkinMtx, vector< map<string, XMFLOAT4X4>>& _LAP)
	{
		// ���õ� �ڽ� ��
		auto itor = _SkinTree.mData.begin(); // ��ǻ� �ϳ� �ۿ� ����

		// �ִ� ��Ʈ����
		auto _aniMtx = itor->second.getAniMtx(_AniPoint);

		XMMATRIX tLoclMtx = XMLoadFloat4x4(&itor->second.mTMLocalMtx);
		XMMATRIX tAniMtx  = XMLoadFloat4x4(&_aniMtx);
		XMMATRIX tParMtx, tResultMtx, tResult;

		// �� ���� LA ó��
		tResultMtx = XMMatrixMultiply(tLoclMtx, tAniMtx);

		XMFLOAT4X4 _tResultMtx;
		XMStoreFloat4x4(&_tResultMtx, tResultMtx);

		//-------------------------------------------------------------------------------------//
		// ���� posŰ���� ������ local TM�� ��ǥ�� ����Ѵ�
		//-------------------------------------------------------------------------------------//
		if (_aniMtx._41 == 0.0f && _aniMtx._42 == 0.0f && _aniMtx._43 == 0.0f)
		{
			_tResultMtx._41 = itor->second.mTMLocalMtx._41;
			_tResultMtx._42 = itor->second.mTMLocalMtx._42;
			_tResultMtx._43 = itor->second.mTMLocalMtx._43;
		}
		else	// posŰ���� ��ǥ������ �����Ѵ�(�̷��� ���� ������ TM�� pos������ �ι�����ȴ�)
		{
			_tResultMtx._41 = _aniMtx._41;
			_tResultMtx._42 = _aniMtx._42;
			_tResultMtx._43 = _aniMtx._43;
		}
		//-------------------------------------------------------------------------------------//
		tResultMtx = XMLoadFloat4x4(&_tResultMtx);

	
		// Pó�� (�θ� ������ ����)
		tParMtx = XMLoadFloat4x4(&_SkinTree.mParentData.mLAP);
		tResult = XMMatrixMultiply(tResultMtx, tParMtx); // LAP
	
		// �ڽĿ��� �ѱ� �����ͷ� ����
		XMStoreFloat4x4(&_SkinTree.mParentData.mLAP, tResult); // _SkinTree.mParentData.mLAP �� ���� �Ʒ� �ڽĿ��� ���� ��Ʈ���� �������

		// LAP �� ����
		_LAP[_AniPoint][itor->first] = _SkinTree.mParentData.mLAP;

		// SKIN �� ����
		XMFLOAT4X4 tSkinMtx;
		XMMATRIX tInvWDMtx = XMLoadFloat4x4(&itor->second.mInvWorldTMMtx);
		XMStoreFloat4x4(&tSkinMtx, XMMatrixMultiply(tInvWDMtx, tResult));
		_SkinMtx[_AniPoint][itor->first] = tSkinMtx;

		// �ڽ� map ������ŭ �ݺ�
		for (auto itorChild = _SkinTree.mChildData.begin(); itorChild != _SkinTree.mChildData.end(); ++itorChild)
		{
			// ���õ� �ڽĿ�, �θ� ������ �ѱ��
			itorChild->second.mParentData.mLAP = _SkinTree.mParentData.mLAP;

			// �Լ�(�ڽĺ�����) �ִ� Ű��, (�ִϸ��̼���) AP��..!!? �ѱ��
			MakeSkin(_AniPoint, itorChild->second, _SkinMtx, _LAP);
		}
	}

	// ��Ų��� �����, ���� ����ϱ� (��Ʈ ��)
	void MakeSkin(int _AniPoint, vector< map<string, XMFLOAT4X4>>& _SkinMtx, vector< map<string, XMFLOAT4X4>>& _LAP)
	{
		// �����Ͱ� �ټ��� ���.. (��Ʈ ���� ���� ��..)
		//for (auto itor = mData.begin(); itor != mData.end(); ++itor)
		{
			auto itor = mData.begin();   // Dummy_root
			auto itor2 = ++itor; --itor; // NULL

			// �ִ� ��Ʈ����
			auto _aniMtx  = itor ->second.getAniMtx(_AniPoint);	  // Dummy_root
			auto _aniMtx2 = itor2->second.getAniMtx(_AniPoint);	  // NULL

			// Dummy_root
			XMMATRIX tLoclMtx = XMLoadFloat4x4(&itor->second.mTMLocalMtx);
			XMMATRIX tAniMtx  = XMLoadFloat4x4(&_aniMtx);

			// NULL
			XMMATRIX tLoclMtx2 = XMLoadFloat4x4(&itor2->second.mTMLocalMtx);
			XMMATRIX tAniMtx2  = XMLoadFloat4x4(&_aniMtx2);

			XMMATRIX tResultMtx, tResultMtx2;

			// �� ���� LA ó��
			tResultMtx  = XMMatrixMultiply(tLoclMtx , tAniMtx);
			tResultMtx2 = XMMatrixMultiply(tLoclMtx2, tAniMtx2);

			XMFLOAT4X4 _tResultMtx, _tResultMtx2;
			XMStoreFloat4x4(&_tResultMtx , tResultMtx);	 // Dummy_root
			XMStoreFloat4x4(&_tResultMtx2, tResultMtx2); // NULL

			//-------------------------------------------------------------------------------------//
			// ���� posŰ���� ������ local TM�� ��ǥ�� ����Ѵ�
			//-------------------------------------------------------------------------------------//
			// Dummy_root
			if (_aniMtx._41 == 0.0f && _aniMtx._42 == 0.0f && _aniMtx._43 == 0.0f)
			{
				_tResultMtx._41 = itor->second.mTMLocalMtx._41;
				_tResultMtx._42 = itor->second.mTMLocalMtx._42;
				_tResultMtx._43 = itor->second.mTMLocalMtx._43;
			}
			else	// posŰ���� ��ǥ������ �����Ѵ�(�̷��� ���� ������ TM�� pos������ �ι�����ȴ�)
			{
				_tResultMtx._41 = _aniMtx._41;
				_tResultMtx._42 = _aniMtx._42;
				_tResultMtx._43 = _aniMtx._43;
			}

			// NULL
			if (_aniMtx2._41 == 0.0f && _aniMtx2._42 == 0.0f && _aniMtx2._43 == 0.0f)
			{
				_tResultMtx2._41 = itor2->second.mTMLocalMtx._41;
				_tResultMtx2._42 = itor2->second.mTMLocalMtx._42;
				_tResultMtx2._43 = itor2->second.mTMLocalMtx._43;
			}
			else	// posŰ���� ��ǥ������ �����Ѵ�(�̷��� ���� ������ TM�� pos������ �ι�����ȴ�)
			{
				_tResultMtx2._41 = _aniMtx2._41;
				_tResultMtx2._42 = _aniMtx2._42;
				_tResultMtx2._43 = _aniMtx2._43;
			}
			//-------------------------------------------------------------------------------------//

			tResultMtx  = XMLoadFloat4x4(&_tResultMtx);	  // Dummy_root
			tResultMtx2 = XMLoadFloat4x4(&_tResultMtx2);  // NULL

			// LAP �� ���� (�ڽĿ��� �ѱ� ������ ����)
			XMStoreFloat4x4(&mParentData.mLAP, tResultMtx);	    // Dummy_root
			_LAP[_AniPoint][itor ->first] = mParentData.mLAP;

			XMStoreFloat4x4(&mParentData.mLAP, tResultMtx2);	// NULL
			_LAP[_AniPoint][itor2->first] = mParentData.mLAP;

			tResultMtx = XMMatrixMultiply(tResultMtx2, tResultMtx);

			// SKIN �� ����
			XMFLOAT4X4 tSkinMtx;
			XMMATRIX tInvWDMtx = XMLoadFloat4x4(&itor->second.mInvWorldTMMtx);
			XMStoreFloat4x4(&tSkinMtx, XMMatrixMultiply(tInvWDMtx, tResultMtx));
			_SkinMtx[_AniPoint][itor ->first] = tSkinMtx; // Dummy_root

			tInvWDMtx = XMLoadFloat4x4(&itor2->second.mInvWorldTMMtx);
			XMStoreFloat4x4(&tSkinMtx, XMMatrixMultiply(tInvWDMtx, tResultMtx2));
			_SkinMtx[_AniPoint][itor2->first] = tSkinMtx; // NULL

			// �ڽ� map ������ŭ �ݺ�
			for (auto itorChild = mChildData.begin(); itorChild != mChildData.end(); ++itorChild)
			{
				// ���õ� �ڽĿ�, �θ� ������ �ѱ��
				XMStoreFloat4x4(&mParentData.mLAP, tResultMtx); // Dummy_root
				itorChild->second.mParentData.mLAP = mParentData.mLAP;

				// �Լ�(�ڽĺ�����) �ִ� Ű��, AP �ѱ��
				MakeSkin(_AniPoint, itorChild->second, _SkinMtx, _LAP);
			}
		}
	}

	// Ű �߰��ϱ� (Ʈ�� ���鶧 ���)
	void AddKey(map<string, NodeBone>& _BoneData, vector<KeyString>& _KeyRoute, int _MaxSize, int _Count)
	{
		// ������ ��ȸ�ߴ°�
		int tMaxSize = _MaxSize - 1;
		if (tMaxSize == _Count)
		{
			// �θ� �̸� �ֱ�
			int tParCount = _Count - 1;
			if (tParCount > 0)
				mParentData.mName = _KeyRoute[tParCount].Name;

			// ���õ� �� �̸�
			string tSelctBone = _KeyRoute[_Count].Name;

			// �ش� �����͸� ã�� �ִ´�
			mData[tSelctBone] = _BoneData[tSelctBone];

			// �̸� ����
			tSelctBone.clear();
		}
		// ����Ű�� �ִٸ�
		else
		{
			int tCount = _Count + 1;
			string _selectName = _KeyRoute[tCount].Name;

			if (mChildData.size() == 0)
				mChildData[_selectName] = SkinTree();

			// �θ� �̸� �ֱ�
			mChildData[_selectName].mParentData.mName = _KeyRoute[_Count].Name;

			mChildData[_selectName].AddKey(_BoneData, _KeyRoute, _MaxSize, tCount);
			_selectName.clear();
		}
	}

	// Ű ã�ư��� (�׳� ������)
	NodeBone* FindKey(string _key)
	{
		// Ű�� ��ġ�ϴ� ��
		if (mData[_key].mObjName == _key)
		{
			// ��ġ�� Ű
			return &mData[_key];
		}
		else
		{
			// �ڽ��� ������� ã�ư���.
			for (auto itor = mChildData.begin(); itor != mChildData.end(); ++itor)
			{
				itor->second.FindKey(_key);
			}
		}
		_key.clear();
		return nullptr;
	}
};

// �� �𵨵��� ��, �ִϸ��̼� ������ ��Ƶ�
class MyBoneData
{
public:
	// ���� ������ ���� �� ����(�Ľ̿�)
	vector<NodeBone> mSaveBoneData;

	// ���� ������ ���� �� ����(���� ���)
	map<string, NodeBone> mBoneData;

	// ���� ������ ���������� ã�ư��� ����
	// ������ �ؽ���
	vector<vector<KeyString> > mBoneHierarchy;
	
	// ��Ų Ʈ��
	SkinTree mSkinTree;

	// �� SKin �ؽ�ó (���)
	SkinTexture mSkinTex;

	// �� SKin ��Ʈ���� (����)
	vector<map<string, XMFLOAT4X4>> mSkinMtx;

	// �� SKin ��Ʈ���� (����)
	vector<vector<XMFLOAT4X4>> mRelocSkinMtx;

	// LAP ��� (�׽�Ʈ��)
	vector<map<string, XMFLOAT4X4>> mLAP;

	// LAP ��� (�׽�Ʈ��)
	vector<vector<XMFLOAT4X4>> mRelocLAP;
	
	// �̸�
	char mMainName[BUF_SIZE];
	char mAniName[BUF_SIZE];

public:
	~MyBoneData()
	{
		// ����
		ClearClass();
	}

	// ������ ���
	void CalData(string _TexName, FSM_TYPE _fsmType)
	{
		// �ؽ�Ʈ�� �ֳ� ����
		string tTexName;

		tTexName += "Export/SkinTex/";
		tTexName += _TexName;
		tTexName += ".dds";

		//--------------------------------------------------------------------------------------------------//
		// �ִ� ������ ä���
		//--------------------------------------------------------------------------------------------------//
		// �̸� ����
		mSkinTex.mName = _TexName;

		// �ִ� Ÿ�� ����
		mSkinTex.mAniType = (float)_fsmType;

		// �ִ� ó��, ������ ��ġ ����
		mSkinTex.mStPoint = 0.0f;
		mSkinTex.mEdPoint = (float)mSaveBoneData[0].mAniData.Position.size();

		// �ؽ�ó �ػ�
		mSkinTex.mTexWidth  = (float)mSaveBoneData.size() * 4;
		mSkinTex.mTexHeight = (float)mSaveBoneData[0].mAniData.Position.size();

		//--------------------------------------------------------------------------------------------------//
		// ���� ��� ���ڿ�
		//--------------------------------------------------------------------------------------------------//
		wstring _WsTexName;
		StringToWchar_t(tTexName, _WsTexName);

		//--------------------------------------------------------------------------------------------------//
		// �ؽ�ó ����
		//--------------------------------------------------------------------------------------------------//
		RetryLoadTex:
		D3DX11_IMAGE_LOAD_INFO _info;
		_info.Width          = (UINT)mSkinTex.mTexWidth;
		_info.Height         = (UINT)mSkinTex.mTexHeight; 
		_info.Depth			 = 0;
		_info.FirstMipLevel  = 0;
		_info.MipLevels      = 1;
		_info.Usage          = D3D11_USAGE_DEFAULT;
		_info.BindFlags      = D3D11_BIND_SHADER_RESOURCE;
		_info.CpuAccessFlags = 0;
		_info.MiscFlags      = 0;
		_info.Format         = DXGI_FORMAT_R32G32B32A32_FLOAT;
		_info.Filter         = D3DX11_FILTER_POINT;
		_info.MipFilter      = D3DX11_FILTER_POINT;
		_info.pSrcInfo       = NULL;

		HRESULT hr = D3DX11CreateShaderResourceViewFromFile(cCoreStorage::GetInstance()->md3dDevice, _WsTexName.c_str(), &_info, 0, &mSkinTex.mTexSRV, 0);

		// �ؽ�ó�� ����
		if (hr == D3D11_ERROR_FILE_NOT_FOUND)
		{
			// ���ٸ� �ؽ�ó ����
			cout << "SkinTex ������ ���� �����մϴ�. �ټ� �ð��� �ɸ� �� �ֽ��ϴ�." << endl;
	
			// ������ �ű��
			RelocationData();

			//Ʈ�� �����
			MakeTree();

			// ��Ų �ؽ�ó �����
			MakeSkinTex(_WsTexName);

			// �ؽ�ó ��Ų �ٽ� �ε�
			goto RetryLoadTex;
		}
		else if (hr == S_OK)
			cout << "SkinTex �ε� ����" << endl;
		else
			cout << ">> ��õ��� ���� ���� �߻�. SkinTex ���� <<" << endl;

		_WsTexName.clear();
	}

	// LAP ����
	vector<XMFLOAT4X4>& GetLapStorage(int _aniKey)
	{
		return mRelocLAP[_aniKey];
	}

	// ��Ų ��� ����
	vector<XMFLOAT4X4>& GetSkinStorage(int _aniKey)
	{
		return mRelocSkinMtx[_aniKey];
	}

	void ClearClass()
	{
		// �� ������ ����
		mSaveBoneData.clear();
		mBoneData.clear();
		
		// ���� ���� ����
		ClearCalValue();

		// ���̶���Ű ����
		for (unsigned int i = 0; i < mBoneHierarchy.size(); ++i)
			mBoneHierarchy[i].clear();
		mBoneHierarchy.clear();
	}

private:
	// ������ ���ġ
	void RelocationData()
	{
		// map���� �ű�
		for (unsigned int i = 0; i < mSaveBoneData.size(); ++i)
		{
			// ȸ�� ���� ���
			vector<RotKeyVtx>& _QuaternionArray = mSaveBoneData[i].mAniData.Quaternion;

			_QuaternionArray[0].Vtx.x = 0.0f;
			_QuaternionArray[0].Vtx.y = 0.0f;
			_QuaternionArray[0].Vtx.z = 0.0f;
			_QuaternionArray[0].Vtx.w = 1.0f;

			for (unsigned int x = 1; x < _QuaternionArray.size(); ++x)
			{
				// ���� �����´�
				XMVECTOR beforeRot  = XMLoadFloat4(&_QuaternionArray[x - 1].Vtx); // ����
				XMVECTOR CurrentRot = XMLoadFloat4(&_QuaternionArray[x].Vtx);     // ����
				XMVECTOR ResultRot;

				// ����� ȸ���� ������Ų��.
				ResultRot = XMQuaternionMultiply(beforeRot, CurrentRot);

				// �����Ѵ�
				XMStoreFloat4(&_QuaternionArray[x].Vtx, ResultRot);
			}

			// ����� ����� �� ����
			XMMATRIX tWdMtx  = XMLoadFloat4x4(&mSaveBoneData[i].mTMWorldMtx);
			XMVECTOR tDet    = XMMatrixDeterminant(tWdMtx);
			XMMATRIX tInvMtx = XMMatrixInverse(&tDet, tWdMtx);

			XMStoreFloat4x4(&mSaveBoneData[i].mInvWorldTMMtx, tInvMtx);

			// Ű�� �°� ���ġ
			string tString = mSaveBoneData[i].mObjName;
			mBoneData[tString] = mSaveBoneData[i];
			tString.clear();
		}
	}

	
	// ��Ų �ؽ�ó �����
	void MakeSkinTex(wstring& _TexName)
	{
		// ��� Ű�� ������ ������ ������.
		int tAniSize = mSaveBoneData[0].mAniData.Position.size();

		// �ؽ�ó ����
		CreateTex(tAniSize);

		// �ؽ�ó ���
		CalTex(tAniSize);

		// �ؽ�ó ����
		SaveTex(_TexName);
	}

	// �ؽ�ó ����
	void CreateTex(int _AniSize)
	{
		D3D11_TEXTURE2D_DESC TextureDesc;
		ZeroMemory(&TextureDesc, sizeof(TextureDesc));

		TextureDesc.Width              = mSaveBoneData.size() * 4; // ������ ��� ���� (�� 4ĭ��, 1��)
		TextureDesc.Height             = _AniSize;				    // �ִ� ������ ��ŭ (��)
		TextureDesc.MipLevels          = 1;                             
		TextureDesc.ArraySize          = 1;                             
		TextureDesc.Format             = DXGI_FORMAT_R32G32B32A32_FLOAT;
		TextureDesc.SampleDesc.Count   = 1;  
		TextureDesc.SampleDesc.Quality = 0;
		TextureDesc.Usage              = D3D11_USAGE_DYNAMIC;
		TextureDesc.BindFlags          = D3D11_BIND_SHADER_RESOURCE;
		TextureDesc.CPUAccessFlags     = D3D11_CPU_ACCESS_WRITE;
		TextureDesc.MiscFlags		   = 0;

		HR(cCoreStorage::GetInstance()->md3dDevice->CreateTexture2D(&TextureDesc, 0, &mSkinTex.mTexture));
	}

	// �ؽ�ó ���
	void CalTex(int _AniSize)
	{
		// ���� ���� ����
		ClearCalValue();

		// ���� ���� ����Ȯ��
		ResizeCalValue();

		// ��Ų �����͸����
		MakeSkinMtx(_AniSize);

		// ��Ų ������ ������ �°� ���ġ
		RelocSkinData();

		// �ؽ�ó ����
		WriteTex(_AniSize);
	}

	// ��Ų ������ �����
	void MakeSkinMtx(int _AniSize)
	{
		for (int i = 0; i < _AniSize; ++i)
			mSkinTree.MakeSkin(i, mSkinMtx, mLAP);
	}

	// ��Ų ������ ������ �°� ���ġ
	void RelocSkinData()
	{
		// �ִϸ��̼� Ű ���� ��ŭ
		for (unsigned int i = 0; i < mSkinMtx.size(); ++i)
		{
			// �� ����
			for (unsigned int x = 0; x < mSaveBoneData.size(); ++x)
			{
				// �̸� ����
				string _getName = mSaveBoneData[x].mObjName;
				mRelocSkinMtx[i].push_back(mSkinMtx[i][_getName]); // i��Ű��, ���õ� ���� �־�� 
				mRelocLAP    [i].push_back(mLAP    [i][_getName]);
			}
		}
	}

	
	// �ؽ�ó ����
	void WriteTex(int _AniSize)
	{
		D3D11_MAPPED_SUBRESOURCE MappedResource;

		// ���� ����
		HRESULT hr = cCoreStorage::GetInstance()->md3dImmediateContext->Map(mSkinTex.mTexture, //������ �ؽ�ó
			D3D11CalcSubresource(0, 0, 1),													   //���� ���ҽ� ��ȣ
			D3D11_MAP_WRITE_DISCARD,														   //���ҽ��� ����
			0,
			&MappedResource);																   //�����͸� �� ������

		// ���� ������������
		if (hr == S_OK)
		{
			// ������ ���� ���۷��� ���
			float* pTexels = (float*)MappedResource.pData;
			
			// �� �ݺ� (�ִ� Ű)
			for (int y = 0; y < _AniSize; ++y) 
			{
				// �� ���� ( 4ĭ�� )
				for (UINT x = 0; x < mSaveBoneData.size(); ++x)
				{
					for (int i = 0; i < 4; ++i)
					{
						// �ȼ� ����
						int _sTex1 = (y * MappedResource.RowPitch / sizeof(float)) + (x * sizeof(float) * 4) + (i * sizeof(float)) + 0;	
						int _sTex2 = (y * MappedResource.RowPitch / sizeof(float)) + (x * sizeof(float) * 4) + (i * sizeof(float)) + 1;	
						int _sTex3 = (y * MappedResource.RowPitch / sizeof(float)) + (x * sizeof(float) * 4) + (i * sizeof(float)) + 2;	
						int _sTex4 = (y * MappedResource.RowPitch / sizeof(float)) + (x * sizeof(float) * 4) + (i * sizeof(float)) + 3;	
						 
						switch (i)
						{
						case 0:
							pTexels[_sTex1] = mRelocSkinMtx[y][x]._11;     //R (float 1)
							pTexels[_sTex2] = mRelocSkinMtx[y][x]._12;     //G (float 2)
							pTexels[_sTex3] = mRelocSkinMtx[y][x]._13;     //B (float 3)
							pTexels[_sTex4] = mRelocSkinMtx[y][x]._14;     //A (float 4)
							break;
						case 1:
							pTexels[_sTex1] = mRelocSkinMtx[y][x]._21;     //R (float 1)
							pTexels[_sTex2] = mRelocSkinMtx[y][x]._22;     //G (float 2)
							pTexels[_sTex3] = mRelocSkinMtx[y][x]._23;     //B (float 3)
							pTexels[_sTex4] = mRelocSkinMtx[y][x]._24;     //A (float 4)
							break;
						case 2:
							pTexels[_sTex1] = mRelocSkinMtx[y][x]._31;     //R (float 1)
							pTexels[_sTex2] = mRelocSkinMtx[y][x]._32;     //G (float 2)
							pTexels[_sTex3] = mRelocSkinMtx[y][x]._33;     //B (float 3)
							pTexels[_sTex4] = mRelocSkinMtx[y][x]._34;     //A (float 4)
							break;
						case 3:
							pTexels[_sTex1] = mRelocSkinMtx[y][x]._41;     //R (float 1)
							pTexels[_sTex2] = mRelocSkinMtx[y][x]._42;     //G (float 2)
							pTexels[_sTex3] = mRelocSkinMtx[y][x]._43;     //B (float 3)
							pTexels[_sTex4] = mRelocSkinMtx[y][x]._44;     //A (float 4)
							break;
						default:
							cout << "��� ���� �ʰ�" << endl;
							break;
						}
					}
				}
			}

			// ���� �ݱ�
			cCoreStorage::GetInstance()->md3dImmediateContext->Unmap(mSkinTex.mTexture, D3D11CalcSubresource(0, 0, 1));
		}
		else
			cout << "��Ų �ؽ�ó ���� ����" << endl;
	}


	// �ؽ�ó ����
	void SaveTex(wstring& _TexName)
	{
		D3DX11SaveTextureToFile(cCoreStorage::GetInstance()->md3dImmediateContext,
			mSkinTex.mTexture,      //������ �ؽ�ó
			D3DX11_IFF_DDS,         //dds�� ����
			_TexName.c_str());      //�̸�

		// �ؽ�ó�� ���������, ���� �����ʹ� ����
		mSkinTex.ClearTex();
	}

	// Ʈ�� �����
	void MakeTree()
	{
		// �� ������ ��ȸ
		for (unsigned int y = 0; y < mBoneHierarchy.size(); ++y)
		{
			// Ű�� �߰��Ѵ�.
			mSkinTree.AddKey(mBoneData, mBoneHierarchy[y], mBoneHierarchy[y].size(), 0);
		}
	}

	// ���� ���� ����
	void ClearCalValue()
	{
		// Skin ����
		for (unsigned int i = 0; i < mSkinMtx.size(); ++i)
			mSkinMtx[i].clear();
		mSkinMtx.clear();

		// Skin ����2
		for (unsigned int i = 0; i < mRelocSkinMtx.size(); ++i)
			mRelocSkinMtx[i].clear();
		mRelocSkinMtx.clear();

		// LAP ����
		for (unsigned int i = 0; i < mLAP.size(); ++i)
			mLAP[i].clear();
		mLAP.clear();

		// LAP ����
		for (unsigned int i = 0; i < mRelocLAP.size(); ++i)
			mRelocLAP[i].clear();
		mRelocLAP.clear();
	}

	// ���� ���� ����Ȯ��
	void ResizeCalValue()
	{
		// ��� Ű�� ������ ������ ������.
		int _size = mSaveBoneData[0].mAniData.Position.size();

		// ��Ų & LAP ��Ʈ����
		mRelocSkinMtx.resize(_size);
		mSkinMtx     .resize(_size);
		mLAP         .resize(_size);
		mRelocLAP    .resize(_size);
	}

	// ��ȯ �Լ�
	void StringToWchar_t(string& _string, wstring& _wstring)
	{
		// ��ȯ
		for (unsigned int i = 0; i < _string.length(); ++i)
			_wstring += wchar_t(_string[i]);

		// ������
		_wstring += wchar_t('\0');
	}
};
