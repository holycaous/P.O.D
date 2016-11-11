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
	e_ShaderColor      = 0,
	e_ShaderLight      = 1,
	e_ShaderPongTex    = 2,
	e_ShaderCartoonTex = 3,
	e_ShaderDeferred   = 4
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
};

struct VertexPN
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
};

struct VertexPNT
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT2 Tex;
	XMFLOAT3 Tangent;
	XMFLOAT3 BiNormal;
};

struct VertexG
{
	XMFLOAT3 Pos;
	XMFLOAT2 Tex;
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
	char mDiffuseSRV[BUF_SIZE];
	char mSpecularSRV[BUF_SIZE];
	char mNomalSRV[BUF_SIZE];

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

// �� ������ (����ġ)
class BoneData
{
public:
	int ID;
	float Weight;
	char Name[BUF_SIZE];
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
	int TagetIdx;
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
	vector<KeyVtx> Position;
	vector<KeyVtx> Quaternion;
	vector<KeyVtx> Scale;
public:
	~AniData()
	{
		ClearClass();
	}

	void ClearClass()
	{
		Position.clear();
		Quaternion.clear();
		Scale.clear();
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
	}
	~ObjData()
	{

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


// �Ľ��� �Ž� ������
class InitMetaData
{
public:
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

	// ���� ���õ� ���� ��Ų ���
	// �� SKin (���� ���)
	vector<XMFLOAT4X4> mSkinMtx;

	// ���� ���� �ִϻ���
	string mAniName;

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

	// �ִ� ������
	AniData aniData;

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
#ifdef DEBUG_MODE
		printf("Ŭ���� ��ü��: %17s ---> ������: %12s, �̸�: %12s\n", mObjName, mCreateName.c_str(), mMainName);
#endif
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
		mSkinMtx   .clear();
		mAniName   .clear();
	}

	// ��Ų ��� ������Ʈ
	void updateSkinMtx(vector<XMFLOAT4X4>& _SkinMtx)
	{
		mSkinMtx.clear();
		mSkinMtx = _SkinMtx;
	}

	// �ؽ�ó �ε�
	void LoadTex(string _TexName, TEXTURE_TYPE e_InitTex)
	{
		// string --> wString���� ��ȯ
		wstring _WsTexName;
		StringToWchar_t(_TexName.c_str(), _WsTexName);

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

		// Ŭ����
		_WsTexName.clear();
		_TexName.clear();
	}

	// ��ȯ �Լ�
	void StringToWchar_t(string _string, wstring& _wstring)
	{
		// ��ȯ
		for (unsigned int i = 0; i < _string.length(); ++i)
			_wstring += wchar_t(_string[i]);

		_wstring += wchar_t('\0');

		_string.clear();
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

	// �ش� ��ġ�� ���� ���͸� ��´�
	XMVECTOR GetPointDir(int _uniqueCode, float _x, float _y, float _z)
	{
		// ��ġ�� �־�����.
		XMVECTOR tPoint  = XMLoadFloat3(&XMFLOAT3(_x, 0.0f, _z));
		XMVECTOR tObjPos = XMLoadFloat3(&mObjData[_uniqueCode].getPosXZ());

		// ��ġ�� ���ϴ� ���͸� ��´�.
		return XMVECTOR(XMVector3Normalize(tPoint - tObjPos));
	}
	
	// ������Ʈ ȸ��
	void SetRotate(int _uniqueCode, float _x, float _y, float _z)
	{
		// ȸ�� ���� ��
		float InterpolationAmount = 12;

		// ȸ�� ��������
		float Rotatelimit = 0.05f;

		// ��ġ�� ���ϴ� ���͸� ��´�.
		XMVECTOR tfromPointLookDir = GetPointDir(_uniqueCode, _x, _y, _z);

		// ������Ʈ ������ ��´�.
		XMVECTOR tObjLook  = XMLoadFloat3(&mObjData[_uniqueCode].getLookDir());
		XMVECTOR tObjUp    = XMLoadFloat3(&mObjData[_uniqueCode].getUpDir());
		XMVECTOR tObjRight = XMLoadFloat3(&mObjData[_uniqueCode].getRightDir());

		// ȸ�� ������ ��´�(��, ��) - ��Į�� ������
		float tRadius = XMVectorGetX(XMVector3Dot(tObjUp, XMVector3Cross(tObjLook, tfromPointLookDir)));

		// ȸ���Ҹ��� ��ġ�� �ִ°�?
		if (tRadius > Rotatelimit || tRadius < -Rotatelimit)
		{
			// �������� �������� ȸ�� ����� �����.
			XMMATRIX tRotMtx;
			tRotMtx = XMMatrixRotationAxis(tObjUp, tRadius / InterpolationAmount);

			// ȸ�� ��� ��
			XMFLOAT3 tFinLookDir;
			XMStoreFloat3(&tFinLookDir, XMVector3TransformNormal(tObjLook, tRotMtx));

			XMFLOAT3 tFiRightDir;
			XMStoreFloat3(&tFiRightDir, XMVector3TransformNormal(tObjRight, tRotMtx));

			// �� ���⺤�Ϳ� �����Ѵ�.
			mObjData[_uniqueCode].setLookDir(tFinLookDir.x, tFinLookDir.y, tFinLookDir.z);
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
	

	/*
	void CreateGrid(float width, float depth, UINT m, UINT n, InitMetaData& meshData)
	{
	UINT vertexCount = m*n;
	UINT faceCount = (m - 1)*(n - 1) * 2;

	//
	// Create the vertices.
	//

	float halfWidth = 0.5f*width;
	float halfDepth = 0.5f*depth;

	float dx = width / (n - 1);
	float dz = depth / (m - 1);

	float du = 1.0f / (n - 1);
	float dv = 1.0f / (m - 1);

	meshData.Vertices.resize(vertexCount);
	for (UINT i = 0; i < m; ++i)
	{
	float z = halfDepth - i*dz;
	for (UINT j = 0; j < n; ++j)
	{
	float x = -halfWidth + j*dx;

	meshData.Vertices[i*n + j].Position = XMFLOAT3(x, 0.0f, z);
	meshData.Vertices[i*n + j].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	meshData.Vertices[i*n + j].TangentU = XMFLOAT3(1.0f, 0.0f, 0.0f);

	// Stretch texture over grid.
	meshData.Vertices[i*n + j].TexUV.x = j*du;
	meshData.Vertices[i*n + j].TexUV.y = i*dv;
	}
	}

	//
	// Create the indices.
	//

	meshData.Indices.resize(faceCount * 3); // 3 indices per face

	// Iterate over each quad and compute indices.
	UINT k = 0;
	for (UINT i = 0; i < m - 1; ++i)
	{
	for (UINT j = 0; j < n - 1; ++j)
	{
	meshData.Indices[k] = i*n + j;
	meshData.Indices[k + 1] = i*n + j + 1;
	meshData.Indices[k + 2] = (i + 1)*n + j;

	meshData.Indices[k + 3] = (i + 1)*n + j;
	meshData.Indices[k + 4] = i*n + j + 1;
	meshData.Indices[k + 5] = (i + 1)*n + j + 1;

	k += 6; // next quad
	}
	}
	}
	*/

};

// ����Ʈ ����
class EffectStorage
{
public:
	ID3DX11Effect* mFX;			   // ����Ʈ
	ID3DX11EffectTechnique* mTech; // ��ũ��
	map<string, ID3DX11EffectMatrixVariable*> mfxMtx; // ���
	map<string, ID3DX11EffectVectorVariable*> mfxVtx; // ����
	map<string, ID3DX11EffectVariable*>     mfxValue; // ����

	map<string, ID3DX11EffectShaderResourceVariable*>  mfxResource; // ���ҽ�
	
	ID3D11InputLayout* mInputLayout;
public:
	EffectStorage()
	{
		mFX = 0;
		mTech = 0;
		mInputLayout = 0;
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
		
		mfxMtx.clear();
		mfxVtx.clear();
		mfxValue.clear();
		mfxResource.clear();

		ReleaseCOM(mTech);
		ReleaseCOM(mFX);
		ReleaseCOM(mInputLayout);
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

	// �ش� ���ۿ� �� �� ����Ʈ ( ����, ���� �� �����ϰ� ����)
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
		MakeInsBuf(_name);
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

		default:
			break;
		}
	}

	// ��ũ�� ���� ����
	void Build_GScreen(InitMetaData* _Screen)
	{
		//-----------------------------------------------------------------------------------------------------------//
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
			vertices[i].Pos = _Screen->Vertices[i].Position;
			vertices[i].Tex = _Screen->Vertices[i].TexUV;
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

	// �� �߰�
	void AddModel(string _ModelName, InitMetaData* _ModelData)
	{
		mModelList[_ModelName] = _ModelData;
	}

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
				vbd.Usage               = D3D11_USAGE_DYNAMIC;
				vbd.ByteWidth           = sizeof(XMFLOAT4X4) * itor->second->mObjData.size();
				vbd.BindFlags           = D3D11_BIND_VERTEX_BUFFER; // ���ؽ�
				vbd.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
				vbd.MiscFlags           = 0;
				vbd.StructureByteStride = 0;

				// �����Ҵ�
				//mInstancedBuffer[itor->second->mCreateName] = NULL;
				HR(mCoreStorage->md3dDevice->CreateBuffer(&vbd, 0, &mInstancedBuffer[itor->second->mCreateName]));
			}
		}
	}

	// �ν��Ͻ� ���� ����
	void MakeInsBuf(string& _name)
	{
		// �� ����
		auto _selectModel = mModelList[_name];

		// ���� ��Ʈ������ �־�߸� ����
		if (_selectModel->mObjData.size())
		{
			D3D11_BUFFER_DESC vbd;
			vbd.Usage               = D3D11_USAGE_DYNAMIC;
			vbd.ByteWidth           = sizeof(XMFLOAT4X4) * _selectModel->mObjData.size();
			vbd.BindFlags           = D3D11_BIND_VERTEX_BUFFER; // ���ؽ�
			vbd.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
			vbd.MiscFlags           = 0;
			vbd.StructureByteStride = 0;

			// �����Ҵ�
			//mInstancedBuffer[itor->second->mCreateName] = NULL;
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
				mCoreStorage->md3dImmediateContext->Map(mInstancedBuffer[itor->second->mCreateName], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);

				// << �ν��Ͻ� ���� >> �� "�������̽�"�� ���´�.
				XMFLOAT4X4* dataView = reinterpret_cast<XMFLOAT4X4*>(mappedData.pData);

				// �ø� ����
				// ��Ʈ���� ����ŭ ���
				auto tModelMtx = itor->second->mObjData;
				UINT i = -1;
				for (auto itor2 = tModelMtx.begin(); itor2 != tModelMtx.end(); ++itor2)
				{
					dataView[++i] = itor2->second.mWdMtx;
				}

				// �ν��Ͻ� ���� �ݱ�
				mCoreStorage->md3dImmediateContext->Unmap(mInstancedBuffer[itor->second->mCreateName], 0);
			}
		}
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
				vertices[k].Pos = itor->second->Vertices[i].Position;
				vertices[k].Color = itor->second->Vertices[i].Color;
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
				vertices[k].Pos = itor->second->Vertices[i].Position;
				vertices[k].Normal = itor->second->Vertices[i].Normal;
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
				vertices[k].Pos = itor->second->Vertices[i].Position;
				vertices[k].Tex = itor->second->Vertices[i].TexUV;
				vertices[k].Normal = itor->second->Vertices[i].Normal;
				vertices[k].Tangent = itor->second->Vertices[i].TangentU;
				vertices[k].BiNormal = itor->second->Vertices[i].BiNormal;
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
		ibd.ByteWidth      = sizeof(UINT)* totalIndexCount;
		ibd.BindFlags      = D3D11_BIND_INDEX_BUFFER; // �ε���
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags      = 0;

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
			UINT _TexIdx = _MetaData->TexIndices[i]; // �ؽ�ó�� ���� �ε���

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
	AniData aniData;

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
		aniData  .ClearClass();
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
	string mName;
	XMFLOAT4X4 mParentMtx;
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
		XMStoreFloat4x4(&mParentMtx, I);

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

	// �ִ� ������
	AniData aniData;

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
	}

	// �ִ� ������
	XMFLOAT4X4 getAniMtx(/*int _key*/)
	{
		XMFLOAT4X4 mAniMtx;
		
		XMMATRIX I = XMMatrixIdentity();
		XMStoreFloat4x4(&mAniMtx, I);

		// �ִϸ��̼� ������ ���
		// aniData, _key


		return mAniMtx;
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

	// ����� P ����� (�����Ӵ� �ٸ��⿡, �� �����Ӵ� ������ �Ѵ�.)
	map<string, BoneParentData> mBoneParentData;

	// �� LAP
	vector<XMFLOAT4X4> mLAPMtx;

	// �� SKin
	vector<XMFLOAT4X4> mSkinMtx;

	// �̸�
	char mMainName[BUF_SIZE];
	char mAniName[BUF_SIZE];

public:
	~MyBoneData()
	{
		// ����
		ClearClass();
	}

	// LAP ���
	vector<XMFLOAT4X4> GetLapStorage()
	{
		return mLAPMtx;
	}

	// Skin ���
	vector<XMFLOAT4X4> GetSkinStorage()
	{
		//ReCalData();
		return mSkinMtx;
	}


	// ������ ���
	void CalData(/*XMFLOAT4X4& _CharMtx*/)
	{
		// �׽�Ʈ �뵵
		XMFLOAT4X4 mTestMtx;
		XMMATRIX I = XMMatrixIdentity();
		XMStoreFloat4x4(&mTestMtx, I);

		// ������ �ű��
		RelocationData();

		// ����� P ������ �����
		MakeParentData(mTestMtx);

		// Skin ���
		CalSkinMtx();
	}

	// ������ ����
	void ReCalData(/*XMFLOAT4X4& _CharMtx*/)
	{
		// �׽�Ʈ �뵵
		XMFLOAT4X4 mTestMtx;
		XMMATRIX I = XMMatrixIdentity();
		XMStoreFloat4x4(&mTestMtx, I);

		// ����� P ������ �����
		MakeParentData(mTestMtx);

		// Skin ���
		CalSkinMtx();
	}

	void ClearClass()
	{
		// �� ������ ����
		mSaveBoneData.clear();
		mBoneData.clear();
		mLAPMtx.clear();
		mSkinMtx.clear();

		// ���̶���Ű ����
		for (unsigned int i = 0; i < mBoneHierarchy.size(); ++i)
			mBoneHierarchy[i].clear();
		mBoneHierarchy.clear();

		// ����� P ����
		mBoneParentData.clear();
	}

private:
	// ��Ų ���
	void CalSkinMtx()
	{
		// ����
		mLAPMtx.clear();
		mSkinMtx.clear();

		XMFLOAT4X4 tSaveMtx;

		// LAP ��� ���
		for (auto itor = mBoneParentData.begin(); itor != mBoneParentData.end(); ++itor)
		{
			auto tBoneData = mBoneData[itor->second.mName];
			XMMATRIX tLoclMtx = XMLoadFloat4x4(&tBoneData.mTMLocalMtx);
			XMMATRIX tWdMtx   = XMLoadFloat4x4(&tBoneData.mTMWorldMtx);
			XMMATRIX tAniMtx  = XMLoadFloat4x4(&tBoneData.getAniMtx());
			XMMATRIX tParMtx  = XMLoadFloat4x4(&itor->second.mParentMtx);
			XMMATRIX tResultMtx;

			tResultMtx = XMMatrixMultiply(tLoclMtx, tAniMtx);
			XMStoreFloat4x4(&tSaveMtx, XMMatrixMultiply(tResultMtx, tParMtx));
			mLAPMtx.push_back(tSaveMtx);

			// ��Ų ��� ���
			// ��Ľ� & ����� ���ϱ�
			XMVECTOR tDet = XMMatrixDeterminant(tWdMtx);
			XMMATRIX tInvMtx = XMMatrixInverse(&tDet, tWdMtx);

			tResultMtx = XMMatrixMultiply(tInvMtx, tResultMtx);
			XMStoreFloat4x4(&tSaveMtx, XMMatrixMultiply(tResultMtx, tParMtx));
			mSkinMtx.push_back(tSaveMtx);
		}
	}

	// ������ ���ġ
	void RelocationData()
	{
		// map���� �ű�
		for (unsigned int i = 0; i < mSaveBoneData.size(); ++i)
		{
			string tString = mSaveBoneData[i].mObjName;

			// Ű�� �°� ���ġ
			mBoneData[tString] = mSaveBoneData[i];
		}
	}

	// �θ� ������ ���
	void MakeParentData(XMFLOAT4X4& _CharMtx/*, int _key*/)
	{
		// ���� �����͸� ���ش�.
		mBoneParentData.clear();

		// ��ü�� ĳ���� ��Ʈ������ �����´�.
		XMMATRIX tCharMtx = XMLoadFloat4x4(&_CharMtx);

		// ���̶���Ű 0������ ������� ��ȸ
		for (unsigned int y = 0; y < mBoneHierarchy.size(); ++y)
		{
			// �����͸� �ӽ������� ������ ��, ���̶���Ű ������ �ʱ�ȭ, ���� �ȴ�.
			BoneParentData tParentData;

			// �ش� Ű�� �����ϱ� ���� ��Ʈ
			for (unsigned int x = 0; x < mBoneHierarchy[y].size(); ++x)
			{
				// ���� Ű�� ������.
				string tKey;
				tKey = mBoneHierarchy[y][x].Name;

				// mBoneParentData�� �ش�Ű�� �ִ��� �˻��Ѵ�.
				if (mBoneParentData.find(tKey) != mBoneParentData.end())
				{
					// �ִٸ�, �� ���� ������.
					XMMATRIX tResultPMtx = XMLoadFloat4x4(&tParentData.mParentMtx);
					XMMATRIX tParentMtx  = XMLoadFloat4x4(&mBoneParentData[tKey].mParentMtx);

					// ���� �� �� �����ϱ�
					XMStoreFloat4x4(&tParentData.mParentMtx, XMMatrixMultiply(tResultPMtx, tParentMtx));
				}
				else
				{
					// ���ٸ�, ��� ����
					XMMATRIX tBParentMtx, tBAniMtx, tResultPMtx;

					// ���� Ű�� �ֻ��� ��Ʈ�� �ƴҶ�, �� ���� ��Ʈ���� ���� �����´�.
					string tBeforeKey;
					int tBeforeX = x - 1;
					if (tBeforeX >= 0)
					{
						// �� ������ Ű�� �����´�.
						tBeforeKey = mBoneHierarchy[y][tBeforeX].Name;

						// �ش� Ű�� ���� ��Ʈ�� ���̶���Ű�� �̹� ���ĵǾ������Ƿ�, 
						// �� ������ �ִ� ���� ��Ʈ������ ������, �ִϸ��̼� �����͸� �����´�.
						tBParentMtx = XMLoadFloat4x4(&mBoneData[tBeforeKey].mTMWorldMtx);
						tBAniMtx    = XMLoadFloat4x4(&mBoneData[tBeforeKey].getAniMtx(/*_key*/));

						// ���� �� �����Ѵ�.
						tResultPMtx = XMMatrixMultiply(tBAniMtx, tBParentMtx);
						XMStoreFloat4x4(&tParentData.mParentMtx, XMMatrixMultiply(tResultPMtx, tCharMtx));
					}
					else
					{
						XMMATRIX tResultPMtx = XMLoadFloat4x4(&tParentData.mParentMtx);

						// �ֻ��� ��Ʈ��� ĳ���� ��Ʈ������ ���Ѵ�. (���� ��Ʈ����)
						XMStoreFloat4x4(&tParentData.mParentMtx, XMMatrixMultiply(tResultPMtx, tCharMtx));
					}
				}
				// �� ����
				tParentData.mName = tKey;
				mBoneParentData[tKey] = tParentData;
				tKey.clear();
			}
		}
	}
};