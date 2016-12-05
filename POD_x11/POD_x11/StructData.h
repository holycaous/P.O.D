#pragma once

#define BUF_SIZE 1024

// Enum 값
// 스테이지
typedef enum
{
	e_MapTool   = 0,
	e_MainState = 1,
	e_PlayState = 2
}GAME_STATE_TYPE;

// 기본 재질
typedef enum
{
	e_MatWhite = 0,
	e_MatBlack = 1,
	e_MatRed   = 2,
	e_MatGreen = 3,
	e_MatBlue  = 4
}MAT_TYPE;

// 조합방법
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

// 쉐이더 변수 초기화
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

// 모델 FSM
typedef enum
{
	e_Idle      = 0,
	e_Damage    = 1,
	e_Run       = 2,
	e_Walk      = 3,
	e_Death     = 4,
	e_Attack    = 5
}FSM_TYPE;

// 어떤 텍스처를 로드할 것인가
typedef enum
{
	e_DiffuseMap  = 0,
	e_SpecularMap = 1,
	e_NomalMap    = 2
}TEXTURE_TYPE;

// 어떤식으로 레스터라이징을 할것인가
typedef enum
{
	e_Wire  = 0,
	e_Solid = 1
}RES_STATE;

// 기본제공 모델, 파싱한 모델인가
typedef enum
{
	e_BasicModel   = 0,
	e_ParsingModel = 1

}MODEL_TYPE;

// 현재 카메라의 모드가 무엇인가
typedef enum
{
	e_1Person = 0,
	e_3Person = 1
}CAM_MODE;

// 오브젝트가 정적인가, 동적인가
typedef enum
{
	e_StaticObj  = 0,
	e_DynamicObj = 1

}OBJ_MOVEABLE;


// 버텍스 구조
struct VertexPC
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
	float   Pedding;			// 패딩 값
};

struct VertexPN
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT2 Pedding;			// 패딩 값
};

struct VertexPNT
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT2 Tex;
	XMFLOAT3 Tangent;
	XMFLOAT3 BiNormal;
	XMFLOAT2 Pedding; // 패딩 값
};

struct VertexPNTAni
{
	XMFLOAT2 Tex;
	XMFLOAT3 Weights;
	XMFLOAT3 VtxInfo; // 버텍스 정보 넣었음
	UINT     BoneIndices[4];
};

struct VertexPNTMap
{
	XMFLOAT2 Tex;
	XMFLOAT2 VtxInfo; // 버텍스 정보 넣었음
};

struct VertexSkyBox
{
	XMFLOAT3 Pos;
	float    Pedding; // 패딩 값
};

struct VertexG
{
	XMFLOAT3 Pos;
	XMFLOAT2 Tex;
	XMFLOAT3 Pedding;
};

// 인스턴스 정보
struct InsAni
{
	XMFLOAT4X4 mMtx;
	XMFLOAT4   mInsAni; // 실행될 애니의 텍스처번호, 프레임 정보, 테스처 너비, 높이
};

// 인스턴스 정보
struct InsMap
{
	XMFLOAT4X4 mMtx;
	XMFLOAT3   mInsTex; // 실행될 텍스처번호,  테스처 너비, 높이
};

// 구조체 정의
struct Vertex
{
	// 생성자 0
	Vertex() : TexUV(FLT_MAX, FLT_MAX), BiNormal(0.0f, 0.0f, 0.0f), Color(1.0f, 0.0f, 0.0f, 1.0f)
	{}

	// 생성자 1
	Vertex(const XMFLOAT3& p, const XMFLOAT3& n, const XMFLOAT3& t, const XMFLOAT2& uv)
		: Position(p), Normal(n), TangentU(t), TexUV(FLT_MAX, FLT_MAX), BiNormal(0.0f, 0.0f, 0.0f), Color(1.0f, 0.0f, 0.0f, 1.0f)
	{}

	// 생성자 2
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

// 바운딩 박스
class BoundBox
{
public:
	XMFLOAT3 Max;
	XMFLOAT3 Min;
	XMFLOAT3 Center;
};

// 재질
class MyMat
{
public:
	// 리소스 뷰 ( 위치 )
	char mDiffuseSRV [BUF_SIZE];
	char mSpecularSRV[BUF_SIZE];
	char mNomalSRV   [BUF_SIZE];

	// 투명 (디퓨즈 맵)
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

// 식별코드를 가진 버텍스 리스트
class KeyVtx
{
public:
	int Key;      // 키값
	XMFLOAT3 Vtx; // 버텍스
};

class RotKeyVtx
{
public:
	int Key;      // 키값
	XMFLOAT4 Vtx; // 버텍스
};

// 본 데이터 (가중치)
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


// 버텍스 가중치 데이터
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

// 애니 데이터
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

// 스키닝 텍스처
class SkinTexture
{
public:
	string mName;
	ID3D11Texture2D* mTexture;
	ID3D11ShaderResourceView* mTexSRV;

	// 애니 정보
	float mAniType;  // 타입
	float mStPoint;  // 시작
	float mEdPoint;  // 끝

	float mTexWidth;  // 텍스처 너비
	float mTexHeight; // 텍스처 높이

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

// 버퍼
struct CharBuf
{
	char Data[BUF_SIZE];
};

// 키를 가진 문자열
class KeyString
{
public:
	int  Key;
	char Name[BUF_SIZE];
	char objClass[BUF_SIZE];
};

// 스텟 등 정보들 모두 넣을 거임
class ObjData
{
public:
	int          mUniqueCode;
	XMFLOAT4X4   mWdMtx;
	OBJ_MOVEABLE mObjMoveAble;

	// 오브젝트 상태값
	float mHP;
	float mMP;

	// 애니 정보
	float mAniType;  // 타입
	float mStPoint;  // 시작
	float mEdPoint;  // 끝
	float mFrame;    // 프레임

	float mTexWidth;  // 텍스처 너비
	float mTexHeight; // 텍스처 높이

	float mAniSpeed; // 애니 스피드

private:
	// 힘 (중력)
	float mForce;

	float mGround;
public:
	ObjData()
	{
		// 키 (따로 키 번호를 부여해주고 있음)
		mUniqueCode = -1; 

		// 월드 매트릭스 초기화
		XMMATRIX I = XMMatrixIdentity();
		XMStoreFloat4x4(&mWdMtx, I);

		// 오브젝트가 동적인지 정적인지
		mObjMoveAble = e_StaticObj;

		mAniType = (float)e_Idle;
		mStPoint = mEdPoint = mFrame = 0.0f;

		// 애니메이션 기본 스피드
		mAniSpeed = 25.0f;

		// 중력
		mForce = 0.0f;
		mGround = 0.0f;
	}
	~ObjData()
	{

	}

	// 점프
	void Jump()
	{
		if (mWdMtx._42 == mGround)
		{
			mForce += 350.0f;
		}
	}

	// 체력 설정
	void SetHp(float _hp)
	{
		mHP = _hp;
	}

	// 마나 설정
	void SetMp(float _mp)
	{
		mMP = _mp;
	}

	// 애니메이션 스피드 조절
	void SetAniSpeed(float _aniSpeed)
	{
		// 애니메이션 속도는 1보다 커야한다.
		if (_aniSpeed <= 1)
			mAniSpeed = 1;
		else
			mAniSpeed = _aniSpeed;
	}

	// 애니타입 변경
	void SetFSM(float _modelFsm, float _StPoint, float _mEdPoint, float _TexWidth, float _TexHeight)
	{
		// 애니타입 변경
		mAniType = _modelFsm;

		// 상태가 변했으면 프레임 초기화 및 애니메이션의 시작과 끝을 가져와야함.
		mFrame   = 0.0f;
		mStPoint = _StPoint;
		mEdPoint = _mEdPoint;

		// 텍스처 크기 갱신
		mTexWidth  = _TexWidth;
		mTexHeight = _TexHeight;
	}

	// 애니타입 변경
	void SetFSM(float _modelFsm, float _StPoint, float _mEdPoint, float _TexWidth, float _TexHeight, float _Frame)
	{
		// 애니타입 변경
		mAniType = _modelFsm;

		// 상태가 변했으면 프레임 초기화 및 애니메이션의 시작과 끝을 가져와야함.
		mFrame   = _Frame;
		mStPoint = _StPoint;
		mEdPoint = _mEdPoint;

		// 텍스처 크기 갱신
		mTexWidth  = _TexWidth;
		mTexHeight = _TexHeight;
	}

	// 프레임 셋하기
	void SetFrame(float _frame)
	{
		mFrame = _frame;
	}

	// 애니 시간 업데이트
	void Update(float& dt, float _y)
	{
		// 시간 흘르기
		mFrame += (dt * mAniSpeed);

		// 마지막보다 크다면, 초기화
		if (mEdPoint <= mFrame)
			mFrame = mStPoint;

		// 바닥 위치 설정
		mGround = _y;

		// 중력 적용
		mForce -= GRAVITY * dt;
		mWdMtx._42 += mForce * dt;

		// 땅바닥 충돌체크
		mWdMtx._42 = max(mGround, mWdMtx._42);

		// 바닥에 닿으면 힘 제로
		if (mWdMtx._42 <= mGround)
			mForce = 0.0f;
	}
	
	// 위치 반환
	XMFLOAT3 getPos() const
	{
		return XMFLOAT3(mWdMtx._41, mWdMtx._42, mWdMtx._43);
	}

	// 위치 반환
	XMFLOAT3 getPosXZ() const
	{
		return XMFLOAT3(mWdMtx._41, 0.0f, mWdMtx._43);
	}

	// 위치 설정
	void setPos(float _x, float _y, float _z)
	{
		mWdMtx._41 = _x;
		mWdMtx._42 = _y;
		mWdMtx._43 = _z;
	}

	// 위치 설정
	void setPosXZ(float _x, float _z)
	{
		mWdMtx._41 = _x;
		mWdMtx._43 = _z;
	}

	// 위치 설정
	void setPosY(float _y)
	{
		mWdMtx._42 = _y;
	}

	// 스케일 설정
	void setScale(float _x, float _y, float _z)
	{
		mWdMtx._11 *= _x;
		mWdMtx._22 *= _y;
		mWdMtx._33 *= _z;
	}

	// 월드 매트릭스 얻기
	XMFLOAT4X4 getWdMtx() const
	{
		return mWdMtx;
	}

	// X 방향 얻기
	XMFLOAT3 getRightDir() const
	{
		return XMFLOAT3(mWdMtx._31, mWdMtx._32, mWdMtx._33);
	}

	// Y 방향 얻기
	XMFLOAT3 getUpDir() const
	{
		return XMFLOAT3(mWdMtx._21, mWdMtx._22, mWdMtx._23);
	}

	// Z 방향 얻기
	XMFLOAT3 getLookDir() const
	{
		return XMFLOAT3(mWdMtx._11, mWdMtx._12, mWdMtx._13);
	}


	// 월드 매트릭스 저장
	void SetWdMtx(XMFLOAT4X4 _WdMtx)
	{
		mWdMtx = _WdMtx;
	}

	// X 방향 저장
	void setRightDir(float _x, float _y, float _z) 
	{
		mWdMtx._31 = _x;
		mWdMtx._32 = _y;
		mWdMtx._33 = _z;
	}

	// Y 방향 저장
	void setUpDir(float _x, float _y, float _z) 
	{
		mWdMtx._21 = _x;
		mWdMtx._22 = _y;
		mWdMtx._23 = _z;
	}

	// Z 방향 저장
	void setLookDir(float _x, float _y, float _z) 
	{
		mWdMtx._11 = _x;
		mWdMtx._12 = _y;
		mWdMtx._13 = _z;
	}
};

// 플레이어
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

// 파싱한 매쉬 데이터
class InitMetaData
{
public:
	//--------------------------------------------------//
	// 맵 전용
	//--------------------------------------------------//
	vector<float> mHeightmap;

	//--------------------------------------------------//
	// 일반 오브젝트
	//--------------------------------------------------//
	// 버텍스, 인덱스
	vector<Vertex>   Vertices;
	vector<UINT>     Indices;

	// 정점늘리기 용도(파싱 데이터 같은경우는 안씀)
	vector<UINT>     TexIndices;
	vector<XMFLOAT3> TexVertices;

	// 월드 매트릭스 (각 캐릭터 구분용)
	map<int, ObjData> mObjData;

	// 텍스처 변환 매트릭스
	XMFLOAT4X4    mTexMtx;

	// TM 매트릭스
	XMFLOAT4X4    mLocTMMtx;
	XMFLOAT4X4    mWdTMMtx;
	
	// 리소스 뷰 (재질정보)
	ID3D11ShaderResourceView* mDiffuseSRV;
	ID3D11ShaderResourceView* mSpecularSRV;
	ID3D11ShaderResourceView* mNomalSRV;

	// 투명 (디퓨즈 맵)
	float mOpacity;

	// 오프셋
	UINT mVertexOffset;
	UINT mIndexOffset;

	// 카운트
	UINT mIndexCount;

	// 애니 데이터 (루트)
	AniData mAniData;

	// 애니데이터가 있는 모델인가
	bool mAniModel;

	// 스킨 텍스처 (포인터)
	map<int, SkinTexture*> mSkinTex;

	// 스킨 모델 (스킨 텍스처와 한쌍이되는 모델 데이터)
	map<int, SkinTexture*> mSkinModelTex;

	// 가중치 데이터
	vector<WeightVtx> weightVtx;

	// 모델을 어떤식으로 조립할 것인가
	D3D_PRIMITIVE_TOPOLOGY _PRIMITIVE_TOPOLOGY;

	// 모델의 종류
	MODEL_TYPE mModelType;

	// 쉐이더 모드
	SHADER_TYPE mShaderMode;

	// 바운딩 박스
	BoundBox mBoundingBox;

	//--------------------------------------------------//
	// 오브젝트 식별 정보
	//--------------------------------------------------//

	// 생성이름
	string mCreateName;

	// 이름
	char mMainName[BUF_SIZE];

	// 오브젝트이름
	int  mObjID;
	char mObjName[BUF_SIZE];
	char mObjClass[BUF_SIZE];

	// 상위 클래스 이름
	int  mParentID;
	char mParentName[BUF_SIZE];
	
private:
	// 유니크코드 할당용.
	int _inputcode;

private:
	cCoreStorage* mCoreStorage = cCoreStorage::GetInstance();

public:
	InitMetaData(const char* _Name, SHADER_TYPE _ShaderMode, D3D_PRIMITIVE_TOPOLOGY _D3D_PRIMITIVE_TOPOLOGY)
	{
		// 변수 초기화
		initValue();

		// 이름 등록
		strcpy(mMainName, _Name);

		// 쉐이더 모드
		mShaderMode = _ShaderMode;

		// 어떤식으로 모델을 구성할 것인가
		_PRIMITIVE_TOPOLOGY = _D3D_PRIMITIVE_TOPOLOGY;

		// 변수 초기화
		strcpy(mObjName   , "NULL\0");
		strcpy(mObjClass  , "NULL\0");
		strcpy(mParentName, "NULL\0");
	}

	InitMetaData(SHADER_TYPE _ShaderMode, D3D_PRIMITIVE_TOPOLOGY _D3D_PRIMITIVE_TOPOLOGY)
	{
		// 변수 초기화
		initValue();
		
		// 쉐이더 모드
		mShaderMode = _ShaderMode;

		// 어떤식으로 모델을 구성할 것인가
		_PRIMITIVE_TOPOLOGY = _D3D_PRIMITIVE_TOPOLOGY;

	}

	~InitMetaData()
	{
		printf("클리어 객체명: %17s ---> 생성명: %12s, 이름: %12s\n", mObjName, mCreateName.c_str(), mMainName);

		// 스킨 모델 제거
		for (auto itor = mSkinModelTex.begin(); itor != mSkinModelTex.end(); ++itor)
			SafeDelete(itor->second);
		mSkinModelTex.clear();

		// 모델 텍스처 제거
		ReleaseCOM(mDiffuseSRV);
		ReleaseCOM(mSpecularSRV);
		ReleaseCOM(mNomalSRV);

		initValue();
	}

	// 변수 초기화
	void initValue()
	{
		// 아이디 값
		mObjID    = -1;
		mParentID = -1;

		// 유니크 코드 할당용
		_inputcode = -1;

		// 애니가 있는 모델인가
		mAniModel = false;

		// 리소스
		mDiffuseSRV = mSpecularSRV = mNomalSRV = nullptr;

		// 디퓨즈맵 투명도
		mOpacity = 100.0f;

		// 매트릭스 초기화
		XMMATRIX I = XMMatrixIdentity();

		// 텍스처 변환 매트릭스 초기화
		XMStoreFloat4x4(&mTexMtx, I);
		XMStoreFloat4x4(&mLocTMMtx, I);
		XMStoreFloat4x4(&mWdTMMtx, I);

		// 버퍼 초기화
		memset(&mMainName  , '\0', sizeof(mMainName));
		memset(&mObjName   , '\0', sizeof(mObjName));
		memset(&mObjClass  , '\0', sizeof(mObjClass));
		memset(&mParentName, '\0', sizeof(mParentName));

		// STL 초기화
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

	// 텍스처 로드
	void LoadTex(string _TexName, TEXTURE_TYPE e_InitTex)
	{
		// string --> wString으로 변환
		wstring _WsTexName;
		StringToWchar_t(_TexName, _WsTexName);

		// 텍스처 로딩
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

	// 변환 함수
	void StringToWchar_t(string& _string, wstring& _wstring)
	{
		// 변환
		for (unsigned int i = 0; i < _string.length(); ++i)
			_wstring += wchar_t(_string[i]);

		_wstring += wchar_t('\0');
	}

	// 모델 (월드 매트릭스) 추가하기
	void AddModel(ObjData _ObjData)
	{
		mObjData[_ObjData.mUniqueCode] = _ObjData;
	}

	// 모델 (월드 매트릭스) 추가하기
	void AddModel(float _x, float _y, float _z, OBJ_MOVEABLE _moveAble)
	{
		// 오브젝트 초기화
		ObjData _ObjData;

		// 유니크 코드 부여
		_ObjData.mUniqueCode = ++_inputcode;

		// 좌표 저장
		_ObjData.mWdMtx._41 = _x;
		_ObjData.mWdMtx._42 = _y;
		_ObjData.mWdMtx._43 = _z;

		// 오브젝트 이동 가능한가
		_ObjData.mObjMoveAble = _moveAble;

		// 오브젝트 저장
		mObjData[_ObjData.mUniqueCode] = _ObjData;
	}

	// 모델 (월드 매트릭스) 추가하기
	void AddModel(XMFLOAT4X4 _mWdMtx, OBJ_MOVEABLE _moveAble)
	{
		// 오브젝트 초기화
		ObjData _ObjData;

		// 유니크 코드 부여
		_ObjData.mUniqueCode = ++_inputcode;

		// 좌표 저장
		_ObjData.mWdMtx = _mWdMtx;

		// 오브젝트 이동 가능한가
		_ObjData.mObjMoveAble = _moveAble;

		// 오브젝트 저장
		mObjData[_ObjData.mUniqueCode] = _ObjData;
	}

	// 모델 (월드 매트릭스) 추가하기
	void AddModel(int _key, float _x, float _y, float _z, OBJ_MOVEABLE _moveAble)
	{
		// 오브젝트 초기화
		ObjData _ObjData;

		// 유니크 코드 부여
		_ObjData.mUniqueCode = _key;

		// 좌표 저장
		_ObjData.mWdMtx._41 = _x;
		_ObjData.mWdMtx._42 = _y;
		_ObjData.mWdMtx._43 = _z;

		// 오브젝트 이동 가능한가
		_ObjData.mObjMoveAble = _moveAble;

		// 오브젝트 저장
		mObjData[_ObjData.mUniqueCode] = _ObjData;
	}

	// 모델 (월드 매트릭스) 추가하기
	void AddModel(int _key, XMFLOAT4X4 _mWdMtx, OBJ_MOVEABLE _moveAble)
	{
		// 오브젝트 초기화
		ObjData _ObjData;

		// 유니크 코드 부여
		_ObjData.mUniqueCode = _key;

		// 좌표 저장
		_ObjData.mWdMtx = _mWdMtx;

		// 오브젝트 이동 가능한가
		_ObjData.mObjMoveAble = _moveAble;

		// 오브젝트 저장
		mObjData[_ObjData.mUniqueCode] = _ObjData;
	}

	// 모델 (월드 매트릭스) 제거하기
	void EraseModel(int _key)
	{
		mObjData.erase(_key);
	}

	// 오브젝트 이동
	void SetPos(int _uniqueCode, float _x, float _y, float _z)
	{
		// 오브젝트 이동
		mObjData[_uniqueCode].setPos(_x, _y, _z);
	}

	// 오브젝트 이동
	void SetPosXZ(int _uniqueCode, float _x, float _z)
	{
		// 오브젝트 이동
		mObjData[_uniqueCode].setPosXZ(_x, _z);
	}

	// 해당 위치로 가는 벡터를 얻는다
	XMVECTOR GetPointDir(int _uniqueCode, float _x, float _y, float _z)
	{
		// 위치가 주어진다.
		XMVECTOR tPoint  = XMLoadFloat3(&XMFLOAT3(_x, 0.0f, _z));
		XMVECTOR tObjPos = XMLoadFloat3(&mObjData[_uniqueCode].getPosXZ());

		// 위치로 향하는 벡터를 얻는다.
		return XMVECTOR(XMVector3Normalize(tPoint - tObjPos));
	}

	// 해당 위치로 가는 벡터를 얻는다
	XMVECTOR GetPointDir(int _uniqueCode, XMFLOAT3 _Pos)
	{
		// 위치가 주어진다.
		XMVECTOR tPoint  = XMLoadFloat3(&XMFLOAT3(_Pos.x, 0.0f, _Pos.z));
		XMVECTOR tObjPos = XMLoadFloat3(&mObjData[_uniqueCode].getPosXZ());

		// 위치로 향하는 벡터를 얻는다.
		return XMVECTOR(XMVector3Normalize(tPoint - tObjPos));
	}
	
	// 오브젝트 회전
	void SetRotate(int _uniqueCode, float _x, float _y, float _z)
	{
		// 회전 보간 양 (클수록 빨리 돈다.)
		float InterpolationAmount = 12 * mCoreStorage->mTimer->DeltaTime();

		// 회전 범위제한
		float Rotatelimit = 0.05f;

		// 위치로 향하는 벡터를 얻는다.
		XMVECTOR tfromPointLookDir = GetPointDir(_uniqueCode, _x, _y, _z);

		// 오브젝트 각도를 얻는다.
		XMVECTOR tObjLook  = XMLoadFloat3(&mObjData[_uniqueCode].getLookDir());
		XMVECTOR tObjUp    = XMLoadFloat3(&mObjData[_uniqueCode].getUpDir());
		XMVECTOR tObjRight = XMLoadFloat3(&mObjData[_uniqueCode].getRightDir());
		XMVECTOR tEpsilon = { 0.05f, 0.05f, 0.05f };

		// 회전 방향을 얻는다(좌, 우) - 스칼라 삼중적
		float tRadius = XMVectorGetX(XMVector3Dot(tObjUp, XMVector3Cross(tObjLook, tfromPointLookDir)));

		// 회전할만한 가치가 있는가?
		if ((tRadius > Rotatelimit || tRadius < -Rotatelimit) || !XMVector3NearEqual(tfromPointLookDir, tObjLook, tEpsilon))
		{
			// 빠른쪽을 기준으로 회전 행렬을 만든다.
			XMMATRIX tRotMtx;
			tRotMtx = XMMatrixRotationAxis(tObjUp, tRadius * InterpolationAmount);

			// 회전 행렬 곱
			XMFLOAT3 tFinLookDir;
			XMStoreFloat3(&tFinLookDir, XMVector3TransformNormal(tObjLook, tRotMtx));

			XMFLOAT3 tFiRightDir;
			XMStoreFloat3(&tFiRightDir, XMVector3TransformNormal(tObjRight, tRotMtx));

			// 각 방향벡터에 적용한다.
			mObjData[_uniqueCode].setLookDir (tFinLookDir.x, tFinLookDir.y, tFinLookDir.z);
			mObjData[_uniqueCode].setRightDir(tFiRightDir.x, tFiRightDir.y, tFiRightDir.z);
		}
	}

	// 오브젝트 스케일
	void SetScale(int _uniqueCode, float _x, float _y, float _z)
	{
		// 오브젝트 이동
		mObjData[_uniqueCode].setScale(_x, _y, _z);
	}

	// 오브젝트 삭제
	void ClearWdMtx()
	{
		_inputcode = -1;
		mObjData.clear();
	}

	// 변수 계산하기
	void CalValue()
	{
		mVertexOffset = Vertices.size();
		mIndexOffset  = Indices.size();
		mIndexCount   = Indices.size();
	}

	// 오브젝트 데이터 1개 가져요기
	ObjData& getObj(int _uniqueCode)
	{
		return mObjData[_uniqueCode];
	}

	// 오브젝트 데이터 모두 가져오기
	map<int, ObjData>& getAllObj()
	{
		return mObjData;
	}

	XMFLOAT3 getPos(int _uniqueCode)
	{
		return mObjData[_uniqueCode].getPos();
	}

	// HP 설정
	void SetHp(int _unicode, float _hp)
	{
		mObjData[_unicode].SetHp(_hp);
	}
	
	// MP 설정
	void SetMp(int _unicode, float _Mp)
	{
		mObjData[_unicode].SetMp(_Mp);
	}

	// 점프
	void Jump(int _unicode)
	{
		mObjData[_unicode].Jump();
	}

	// 오브젝트 FSM 설정
	void SetFSM(int _unicode, FSM_TYPE _modelFsm)
	{
		float _newType   = mSkinTex[_modelFsm]->mAniType;
		float _StPoint   = mSkinTex[_modelFsm]->mStPoint;
		float _EdPoint   = mSkinTex[_modelFsm]->mEdPoint;
		float _TexWidth  = mSkinTex[_modelFsm]->mTexWidth;
		float _TexHeight = mSkinTex[_modelFsm]->mTexHeight;

		// 새로운 모델로 세팅
		mObjData[_unicode].SetFSM(_newType, _StPoint, _EdPoint, _TexWidth, _TexHeight);
	}

	// 오브젝트 FSM 설정
	void SetFSM(int _unicode, FSM_TYPE _modelFsm, float _Frame)
	{
		float _newType   = mSkinTex[_modelFsm]->mAniType;
		float _StPoint   = mSkinTex[_modelFsm]->mStPoint;
		float _EdPoint   = mSkinTex[_modelFsm]->mEdPoint;
		float _TexWidth  = mSkinTex[_modelFsm]->mTexWidth;
		float _TexHeight = mSkinTex[_modelFsm]->mTexHeight;

		// 새로운 모델로 세팅
		mObjData[_unicode].SetFSM(_newType, _StPoint, _EdPoint, _TexWidth, _TexHeight, _Frame);
	}
};

// 맵 정보
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
		// 총 지형 폭.
		return (mXwidth - 1) * mCellSize;
	}
	float GetDepth()
	{
		// 총 지형 깊이.
		return (mZdepth - 1) * mCellSize;
	}

	// 높이 얻기 (임시로 처리)
	float GetHeight(float& x, float& z)
	{
		// 셀 좌표계로 옮기기
		float c = (x + 0.5f * GetWidth()) /  mCellSize;
		float d = (z - 0.5f * GetDepth()) / -mCellSize;

		// 소숫점 버리기
		int row = (int)floorf(d);
		int col = (int)floorf(c);

		// 셀에서 하이트맵 추출
		// A*--*B
		//  | /|
		//  |/ |
		// C*--*D
		vector<float>& _heightMap = mModel->mHeightmap;

		float A = _heightMap[row * (int)mXwidth + col];
		float B = _heightMap[row * (int)mXwidth + col + 1];
		float C = _heightMap[(row + 1) * (int)mXwidth + col];
		float D = _heightMap[(row + 1) * (int)mXwidth + col + 1];

		// 셀과 관련된 상대적인 위치.
		float s = c - (float)col;
		float t = d - (float)row;

		// 위쪽 삼각형 ABC 인 경우.
		if (s + t <= 1.0f)
		{
			float uy = B - A;
			float vy = C - A;
			return A + s*uy + t*vy;
		}
		else // 아래쪽 삼각형 DCB
		{
			float uy = C - D;
			float vy = B - D;
			return D + (1.0f - s)*uy + (1.0f - t)*vy;
		}
	}
};

// 이펙트 변수
class EffectStorage
{
public:
	ID3DX11Effect* mFX;			   // 이펙트
	map<int, ID3DX11EffectTechnique*> mTech; // 테크닉
	map<string, ID3DX11EffectMatrixVariable*> mfxMtx; // 행렬
	map<string, ID3DX11EffectVectorVariable*> mfxVtx; // 벡터
	map<string, ID3DX11EffectVariable*>     mfxValue; // 변수

	map<string, ID3DX11EffectShaderResourceVariable*>  mfxResource; // 리소스
	
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

// 버퍼 종류
class BufferType
{
	cCoreStorage* mCoreStorage = cCoreStorage::GetInstance();
public:
	// 버퍼
	ID3D11Buffer* mVB;
	ID3D11Buffer* mIB;

	// 인스턴스용
	map<string, ID3D11Buffer*> mInstancedBuffer;

	// 쉐이더 모드
	SHADER_TYPE mShaderMode;

	// 해당 버퍼에 들어갈 모델 리스트
	map<string, InitMetaData*> mModelList;

public:
	BufferType()
	{
		initValue();

		// 버퍼
		mVB = mIB = nullptr;
	}

	BufferType(SHADER_TYPE _ShaderMode)
	{
		initValue();

		// 버퍼
		mVB = mIB = nullptr;

		// 쉐이더 모드 셋팅
		mShaderMode = _ShaderMode;
	}
	~BufferType()
	{
		initValue();

		ReleaseCOM(mVB);
		ReleaseCOM(mIB);

		// 버퍼
		mVB = mIB = nullptr;
	}

	void initValue()
	{
		// 인스턴스 버퍼 삭제
		ClearInsBuf();

		// 모델 리스트 삭제
		mModelList.clear();
	}

	void ClearInsBuf()
	{
		// 인스턴스 버퍼가 있다면
		if (mInstancedBuffer.size())
		{
			// 인스턴스 버퍼 삭제
			for (auto itor = mInstancedBuffer.begin(); itor != mInstancedBuffer.end(); ++itor)
				ReleaseCOM(itor->second);
			mInstancedBuffer.clear();
		}
	}

	void ClearInsBuf(string& _name)
	{
		// 인스턴스 버퍼가 있다면
		if (mInstancedBuffer[_name])
		{
			// 인스턴스 버퍼 삭제
			ReleaseCOM(mInstancedBuffer[_name]);
		}
	}

	// 버퍼 다시만들기
	void ReMakeBuf(string& _name)
	{
		ClearInsBuf(_name);
		MakeInsBuf (_name);
	}

	// 모델 추가
	void AddModel(string _ModelName, InitMetaData* _ModelData)
	{
		mModelList[_ModelName] = _ModelData;
	}

	// 모델 다시 추가
	void ReAddModel(string _ModelName, InitMetaData* _ModelData)
	{
		// 삭제 후, 추가
		mModelList.erase(_ModelName);
		mModelList[_ModelName] = _ModelData;
	}

	// 빌드
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
	// 스크린 전용
	//--------------------------------------------------------------------------------------------------//
	// 스크린 인스턴스 버퍼 생성
	void MakeScreenInsBuf(InitMetaData* _Screen)
	{
		// 인스턴스 버퍼 생성
		// 월드 매트릭스가 있어야만 생성
		if (_Screen->mObjData.size())
		{
			D3D11_BUFFER_DESC vbd;
			vbd.Usage               = D3D11_USAGE_DYNAMIC;
			vbd.ByteWidth           = sizeof(XMFLOAT4X4) * _Screen->mObjData.size();
			vbd.BindFlags           = D3D11_BIND_VERTEX_BUFFER; // 버텍스
			vbd.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
			vbd.MiscFlags           = 0;
			vbd.StructureByteStride = 0;

			if (_Screen->mObjData.size() != 1)
				cout << "스크린 버퍼가 1개가 아닙니다!!" << endl;

			// 공간할당
			//mInstancedBuffer[itor->second->mCreateName] = NULL;
			HR(mCoreStorage->md3dDevice->CreateBuffer(&vbd, 0, &mInstancedBuffer[_Screen->mCreateName]));
		}
	}

	// 인스턴스 모델 업데이트
	void UpdateScreenIns(InitMetaData* _Screen)
	{
		// 인스턴스 버퍼 열기 ( 다이나믹 버퍼 )
		D3D11_MAPPED_SUBRESOURCE mappedData;

		// 해당 모델의 월드 매트릭스가 있을때
		if (_Screen->mObjData.size())
		{
			mCoreStorage->md3dImmediateContext->Map(mInstancedBuffer[_Screen->mCreateName], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);

			// << 인스턴스 버퍼 >> 의 "인터페이스"를 얻어온다.
			XMFLOAT4X4* dataView = reinterpret_cast<XMFLOAT4X4*>(mappedData.pData);

			// 컬링 안함
			// 매트릭스 수만큼 등록
			auto tobjMtx = _Screen->mObjData;
			UINT i = -1;
			for (auto itor = tobjMtx.begin(); itor != tobjMtx.end(); ++itor)
			{
				dataView[++i] = itor->second.mWdMtx;
			}
			// 인스턴스 버퍼 닫기
			mCoreStorage->md3dImmediateContext->Unmap(mInstancedBuffer[_Screen->mCreateName], 0);
		}
	}

	//--------------------------------------------------------------------------------------------------//
	// 일반 인스턴스 전용
	//--------------------------------------------------------------------------------------------------//
	// 인스턴스 버퍼 생성
	void MakeInsBuf()
	{
		// 해당 쉐이더에 있는 모든 모델들은 인스턴스 버퍼를 만들어야 한다.
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// 월드 매트릭스가 있어야만 생성
			if (itor->second->mObjData.size())
			{
				D3D11_BUFFER_DESC vbd;

				// 애니 모델 버퍼
				if (itor->second->mShaderMode == e_ShaderPongTexAni)
				{
					vbd.Usage               = D3D11_USAGE_DYNAMIC;
					vbd.ByteWidth           = sizeof(InsAni) * itor->second->mObjData.size();
					vbd.BindFlags           = D3D11_BIND_VERTEX_BUFFER; // 버텍스
					vbd.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
					vbd.MiscFlags           = 0;
					vbd.StructureByteStride = 0;
				}
				// 맵 모델 버퍼
				else if (itor->second->mShaderMode == e_ShaderPongTexMap)
				{
					vbd.Usage               = D3D11_USAGE_DYNAMIC;
					vbd.ByteWidth           = sizeof(InsMap) * itor->second->mObjData.size();
					vbd.BindFlags           = D3D11_BIND_VERTEX_BUFFER; // 버텍스
					vbd.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
					vbd.MiscFlags           = 0;
					vbd.StructureByteStride = 0;
				}
				// 일반 모델 버퍼
				else
				{
					vbd.Usage               = D3D11_USAGE_DYNAMIC;
					vbd.ByteWidth           = sizeof(XMFLOAT4X4) * itor->second->mObjData.size();
					vbd.BindFlags           = D3D11_BIND_VERTEX_BUFFER; // 버텍스
					vbd.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
					vbd.MiscFlags           = 0;
					vbd.StructureByteStride = 0;
				}

				// 공간할당
				HR(mCoreStorage->md3dDevice->CreateBuffer(&vbd, 0, &mInstancedBuffer[itor->second->mCreateName]));
			}
		}
	}

	// 인스턴스 버퍼 생성
	void MakeInsBuf(string& _name)
	{
		// 모델 선택
		InitMetaData* _selectModel = mModelList[_name];

		// 월드 매트릭스가 있어야만 생성
		if (_selectModel->mObjData.size())
		{
			D3D11_BUFFER_DESC vbd;

			// 애니 모델 버퍼
			if (_selectModel->mShaderMode == e_ShaderPongTexAni)
			{
				vbd.Usage               = D3D11_USAGE_DYNAMIC;
				vbd.ByteWidth           = sizeof(InsAni) * _selectModel->mObjData.size();
				vbd.BindFlags           = D3D11_BIND_VERTEX_BUFFER; // 버텍스
				vbd.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
				vbd.MiscFlags           = 0;
				vbd.StructureByteStride = 0;
			}
			// 맵 모델 버퍼
			else if (_selectModel->mShaderMode == e_ShaderPongTexMap)
			{
				vbd.Usage               = D3D11_USAGE_DYNAMIC;
				vbd.ByteWidth           = sizeof(InsMap) * _selectModel->mObjData.size();
				vbd.BindFlags           = D3D11_BIND_VERTEX_BUFFER; // 버텍스
				vbd.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
				vbd.MiscFlags           = 0;
				vbd.StructureByteStride = 0;
			}
			// 일반 모델 버퍼
			else
			{
				vbd.Usage               = D3D11_USAGE_DYNAMIC;
				vbd.ByteWidth           = sizeof(XMFLOAT4X4) * _selectModel->mObjData.size();
				vbd.BindFlags           = D3D11_BIND_VERTEX_BUFFER; // 버텍스
				vbd.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
				vbd.MiscFlags           = 0;
				vbd.StructureByteStride = 0;
			}

			// 공간할당
			HR(mCoreStorage->md3dDevice->CreateBuffer(&vbd, 0, &mInstancedBuffer[_selectModel->mCreateName]));
		}
	}

	// 인스턴스 모델 업데이트
	void UpdateIns()
	{
		// 해당 쉐이더에 있는 모든 모델들은 인스턴스 버퍼를 갱신해야 한다.
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// 인스턴스 버퍼 열기 ( 다이나믹 버퍼 )
			D3D11_MAPPED_SUBRESOURCE mappedData;

			// 해당 모델의 월드 매트릭스가 있을때
			if (itor->second->mObjData.size())
			{
				// 인스턴스 버퍼 열기
				mCoreStorage->md3dImmediateContext->Map(mInstancedBuffer[itor->second->mCreateName], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);

				// 모델 데이터 가져오기
				map<int, ObjData>& tModelData = itor->second->mObjData;
				UINT i = -1;

				// 애니 모델 쓰기
				if (itor->second->mShaderMode == e_ShaderPongTexAni)
				{
					// << 인스턴스 버퍼 >> 의 "인터페이스"를 얻어온다.
					InsAni* dataView = reinterpret_cast<InsAni*>(mappedData.pData);

					// 등록된 모델 수만큼 등록 (컬링 안함)
					for (auto itor2 = tModelData.begin(); itor2 != tModelData.end(); ++itor2)
					{
						// 모델 매트릭스
						dataView[++i].mMtx = itor2->second.mWdMtx;

						// 모델 애니정보
						dataView[i].mInsAni.x = itor2->second.mAniType;    // 실행될 애니의 텍스처번호
						dataView[i].mInsAni.y = itor2->second.mFrame;      // 프레임 정보
						dataView[i].mInsAni.z = itor2->second.mTexWidth;   // 텍스처 너비
						dataView[i].mInsAni.w = itor2->second.mTexHeight;  // 텍스처 높이
					}
				}
				// 맵 버퍼 쓰기
				else if (itor->second->mShaderMode == e_ShaderPongTexMap)
				{
					// << 인스턴스 버퍼 >> 의 "인터페이스"를 얻어온다.
					InsMap* dataView = reinterpret_cast<InsMap*>(mappedData.pData);

					// 등록된 모델 수만큼 등록 (컬링 안함)
					for (auto itor2 = tModelData.begin(); itor2 != tModelData.end(); ++itor2)
					{
						// 모델 매트릭스
						dataView[++i].mMtx = itor2->second.mWdMtx;

						// 모델 애니정보
						dataView[i].mInsTex.x = 0;						   // 실행될 맵의 텍스처번호
						dataView[i].mInsTex.y = itor2->second.mTexWidth;   // 프레임 너비
						dataView[i].mInsTex.z = itor2->second.mTexHeight;  // 텍스처 높이
					}
				}
				// 일반 모델 쓰기
				else
				{
					// << 인스턴스 버퍼 >> 의 "인터페이스"를 얻어온다.
					XMFLOAT4X4* dataView = reinterpret_cast<XMFLOAT4X4*>(mappedData.pData);

					// 등록된 모델 수만큼 등록 (컬링 안함)
					for (auto itor2 = tModelData.begin(); itor2 != tModelData.end(); ++itor2)
					{
						dataView[++i] = itor2->second.mWdMtx;
					}
				}

				// 인스턴스 버퍼 닫기
				mCoreStorage->md3dImmediateContext->Unmap(mInstancedBuffer[itor->second->mCreateName], 0);
			}
		}
	}

	//-----------------------------------------------------------------------------------------------------------//
	// 빌드
	//-----------------------------------------------------------------------------------------------------------//
	// 스크린 버퍼 빌드
	void Build_GScreen(InitMetaData* _Screen)
	{
		_Screen->mVertexOffset = 0;
		_Screen->mIndexOffset  = 0;


		// 총 버텍스 사이즈 계산
		vector<VertexG> vertices; // 한곳으로 옮기는 작업 ( 가장 큰 버퍼 )
		UINT totalVertexCount = _Screen->Vertices.size();

		// 정점 늘리기
		CalVtxINCAndSet(_Screen);

		// 전체버퍼 사이즈 늘리기
		vertices.resize(totalVertexCount);

		// 가장 큰 버퍼로 복사
		// 이터레이터가 돌면서, 버텍스 크기만큼 더한다.
		for (unsigned int i = 0; i < _Screen->Vertices.size(); ++i)
		{
			vertices[i].Pos     = _Screen->Vertices[i].Position;
			vertices[i].Tex     = _Screen->Vertices[i].TexUV;
			vertices[i].Pedding = XMFLOAT3(0.0f, 0.0f, 0.0f);
		}

		// 버텍스 버퍼 만들기
		D3D11_BUFFER_DESC vbd;
		vbd.Usage          = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth      = sizeof(VertexG)* totalVertexCount;
		vbd.BindFlags      = D3D11_BIND_VERTEX_BUFFER; // 버텍스
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags      = 0;

		D3D11_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = &vertices[0];
		HR(cCoreStorage::GetInstance()->md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));

		// 총 인덱스 사이즈 계산
		UINT totalIndexCount = _Screen->Indices.size();

		// 인덱스 버퍼 만들기
		std::vector<UINT> indices;

		// 가장 큰 버퍼로 복사
		// 이터레이터가 돌면서, 인덱스 크기만큼 더한다.
		for (unsigned int i = 0; i < _Screen->Indices.size(); ++i)
		{
			indices.push_back(_Screen->Indices[i]);
		}

		// 인덱스 버퍼 만들기
		D3D11_BUFFER_DESC ibd;
		ibd.Usage          = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth      = sizeof(UINT)* totalIndexCount;
		ibd.BindFlags      = D3D11_BIND_INDEX_BUFFER; // 인덱스
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags      = 0;

		D3D11_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = &indices[0];
		HR(mCoreStorage->md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
	}

private:
	// 모델의 메타데이터( 정보를 가지고 있는) , 저장할 모델 
	void Build_PC()
	{
		// 한곳으로 옮기는 작업 ( 가장 큰 버퍼 )
		vector<VertexPC> vertices;

		// 총 버텍스 사이즈 계산
		UINT totalVertexCount = 0;
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			totalVertexCount += itor->second->Vertices.size();
		}

		// 총 인덱스 사이즈 계산
		UINT totalIndexCount = 0;
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			totalIndexCount += itor->second->Indices.size();
		}

		// 개별 버텍스 오프셋 계산
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// 직전 버텍스 저장
			int AfterVtxOffset = 0;

			// 처음이라면
			if (itor == mModelList.begin())
				itor->second->mVertexOffset = 0;
			else
				// 내것과, 내 직전 걸 더한다.
				itor->second->mVertexOffset = (itor->second->mVertexOffset + AfterVtxOffset);

			// 직전 버텍스 저장
			AfterVtxOffset = itor->second->mVertexOffset;
		}

		// 개별 인덱스 오프셋 계산
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// 직전 인덱스 저장
			int AfterIdxOffset = 0;

			// 처음이라면
			if (itor == mModelList.begin())
				itor->second->mIndexOffset = 0;
			else
				// 내것과, 내 직전 걸 더한다.
				itor->second->mIndexOffset = (itor->second->mIndexOffset + AfterIdxOffset);

			// 직전 인덱스 저장
			AfterIdxOffset = itor->second->mIndexOffset;
		}

		// 전체버퍼 사이즈 늘리기
		vertices.resize(totalVertexCount);

		// 가장 큰 버퍼로 복사
		UINT k = 0;
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// 이터레이터가 돌면서, 버텍스 크기만큼 더한다.
			for (unsigned int i = 0; i < itor->second->Vertices.size(); ++i, ++k)
			{
				vertices[k].Pos     = itor->second->Vertices[i].Position;
				vertices[k].Color   = itor->second->Vertices[i].Color;
				vertices[k].Pedding = 0.0f;			// 패딩 값
			}
		}

		// 버텍스 버퍼 만들기
		D3D11_BUFFER_DESC vbd;
		vbd.Usage          = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth      = sizeof(VertexPC)* totalVertexCount;
		vbd.BindFlags      = D3D11_BIND_VERTEX_BUFFER; // 버텍스
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags      = 0;

		D3D11_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = &vertices[0];
		HR(cCoreStorage::GetInstance()->md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));

		// 인덱스 버퍼 만들기
		std::vector<UINT> indices;

		// 가장 큰 버퍼로 복사
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// 이터레이터가 돌면서, 인덱스 크기만큼 더한다.
			for (unsigned int i = 0; i < itor->second->Indices.size(); ++i)
			{
				indices.push_back(itor->second->Indices[i]);
			}
		}

		// 인덱스 버퍼 만들기
		D3D11_BUFFER_DESC ibd;
		ibd.Usage          = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth      = sizeof(UINT)* totalIndexCount;
		ibd.BindFlags      = D3D11_BIND_INDEX_BUFFER; // 인덱스
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags      = 0;

		D3D11_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = &indices[0];
		HR(mCoreStorage->md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
	}
	void Build_PN()
	{
		// 한곳으로 옮기는 작업 ( 가장 큰 버퍼 )
		vector<VertexPN> vertices;

		// 총 버텍스 사이즈 계산
		UINT totalVertexCount = 0;
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			totalVertexCount += itor->second->Vertices.size();
		}

		// 총 인덱스 사이즈 계산
		UINT totalIndexCount = 0;
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			totalIndexCount += itor->second->Indices.size();
		}

		// 개별 버텍스 오프셋 계산
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// 직전 버텍스 저장
			int AfterVtxOffset = 0;

			// 처음이라면
			if (itor == mModelList.begin())
				itor->second->mVertexOffset = 0;
			else
				// 내것과, 내 직전 걸 더한다.
				itor->second->mVertexOffset = (itor->second->mVertexOffset + AfterVtxOffset);

			// 직전 버텍스 저장
			AfterVtxOffset = itor->second->mVertexOffset;
		}

		// 개별 인덱스 오프셋 계산
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// 직전 인덱스 저장
			int AfterIdxOffset = 0;

			// 처음이라면
			if (itor == mModelList.begin())
				itor->second->mIndexOffset = 0;
			else
				// 내것과, 내 직전 걸 더한다.
				itor->second->mIndexOffset = (itor->second->mIndexOffset + AfterIdxOffset);

			// 직전 인덱스 저장
			AfterIdxOffset = itor->second->mIndexOffset;
		}

		// 법선벡터 계산
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			CalNormalVtx(itor->second);
		}

		// 전체버퍼 사이즈 늘리기
		vertices.resize(totalVertexCount);

		// 가장 큰 버퍼로 복사
		UINT k = 0;
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// 이터레이터가 돌면서, 버텍스 크기만큼 더한다.
			for (unsigned int i = 0; i < itor->second->Vertices.size(); ++i, ++k)
			{
				vertices[k].Pos     = itor->second->Vertices[i].Position;
				vertices[k].Normal  = itor->second->Vertices[i].Normal;
				vertices[k].Pedding = XMFLOAT2(0.0f, 0.0f);			// 패딩 값
			}
		}

		// 버텍스 버퍼 만들기
		D3D11_BUFFER_DESC vbd;
		vbd.Usage          = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth      = sizeof(VertexPN)* totalVertexCount;
		vbd.BindFlags      = D3D11_BIND_VERTEX_BUFFER; // 버텍스
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags      = 0;

		D3D11_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = &vertices[0];
		HR(cCoreStorage::GetInstance()->md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));

		// 인덱스 버퍼 만들기
		std::vector<UINT> indices;

		// 가장 큰 버퍼로 복사
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// 이터레이터가 돌면서, 인덱스 크기만큼 더한다.
			for (unsigned int i = 0; i < itor->second->Indices.size(); ++i)
			{
				indices.push_back(itor->second->Indices[i]);
			}
		}

		// 인덱스 버퍼 만들기
		D3D11_BUFFER_DESC ibd;
		ibd.Usage          = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth      = sizeof(UINT)* totalIndexCount;
		ibd.BindFlags      = D3D11_BIND_INDEX_BUFFER; // 인덱스
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags      = 0;

		D3D11_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = &indices[0];
		HR(mCoreStorage->md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
	}
	void Build_PNT()
	{
		//-----------------------------------------------------------------------------------------------------------//
		// 직전 버텍스 저장용
		int AfterVtxOffset = 0;

		// 개별 버텍스 오프셋 계산
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// 처음이라면
			if (AfterVtxOffset == 0)
			{
				// 현재 버텍스 저장
				AfterVtxOffset = itor->second->mVertexOffset;
				itor->second->mVertexOffset = 0;
			}
			else
			{
				// 내것과, 내 직전 걸 더한다.
				itor->second->mVertexOffset = AfterVtxOffset;
				AfterVtxOffset = itor->second->mVertexOffset + itor->second->Vertices.size();
			}
		}
		// 직전 인덱스 저장용
		int AfterIdxOffset = 0;

		// 개별 인덱스 오프셋 계산
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// 처음이라면
			if (AfterIdxOffset == 0)
			{
				// 현재 인덱스 저장
				AfterIdxOffset = itor->second->mIndexCount;
				itor->second->mIndexOffset = 0;
			}
			else
			{
				// 내것과, 내 직전 걸 더한다.
				itor->second->mIndexOffset = AfterIdxOffset;
				AfterIdxOffset = itor->second->mIndexOffset + itor->second->mIndexCount;
			}
		}

		// 법선벡터 계산
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// itor이 파싱된 모델이라면 넘어간다.
			if (itor->second->mModelType == e_ParsingModel)
				continue;

			// 법선벡터 계산
			CalNormalVtx(itor->second);
		}

		// 정점 늘리기 & 버텍스 버퍼로 송신
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// itor이 파싱된 모델이라면 넘어간다.
			if (itor->second->mModelType == e_ParsingModel)
				continue;

			// 정점 늘리기
			CalVtxINCAndSet(itor->second);
		}

		// 탄젠트 공간 계산
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// itor이 파싱된 모델이라면 넘어간다.
			if (itor->second->mModelType == e_ParsingModel)
				continue;

			// 탄젠트 공간 계산
			CalTangentSpace(itor->second);
		}

		// 총 버텍스 사이즈 계산
		vector<VertexPNT> vertices; // 한곳으로 옮기는 작업 ( 가장 큰 버퍼 )
		UINT totalVertexCount = 0;
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			totalVertexCount += itor->second->Vertices.size();
		}

		// 전체버퍼 사이즈 늘리기
		vertices.resize(totalVertexCount);

		// 가장 큰 버퍼로 복사
		UINT k = 0;
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// 이터레이터가 돌면서, 버텍스 크기만큼 더한다.
			for (unsigned int i = 0; i < itor->second->Vertices.size(); ++i, ++k)
			{
				vertices[k].Pos      = itor->second->Vertices[i].Position;
				vertices[k].Tex      = itor->second->Vertices[i].TexUV;
				vertices[k].Normal   = itor->second->Vertices[i].Normal;
				vertices[k].Tangent  = itor->second->Vertices[i].TangentU;
				vertices[k].BiNormal = itor->second->Vertices[i].BiNormal;
				vertices[k].Pedding  = XMFLOAT2(0.0f, 0.0f);			// 패딩 값
			}
		}

		// 버텍스 버퍼 만들기
		D3D11_BUFFER_DESC vbd;
		vbd.Usage          = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth      = sizeof(VertexPNT)* totalVertexCount;
		vbd.BindFlags      = D3D11_BIND_VERTEX_BUFFER; // 버텍스
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags      = 0;

		D3D11_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = &vertices[0];
		HR(cCoreStorage::GetInstance()->md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));


		// 총 인덱스 사이즈 계산
		UINT totalIndexCount = 0;
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			totalIndexCount += itor->second->Indices.size();
		}

		// 인덱스 버퍼 만들기
		std::vector<UINT> indices;

		// 가장 큰 버퍼로 복사
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// 이터레이터가 돌면서, 인덱스 크기만큼 더한다.
			for (unsigned int i = 0; i < itor->second->Indices.size(); ++i)
			{
				indices.push_back(itor->second->Indices[i]);
			}
		}

		// 인덱스 버퍼 만들기
		D3D11_BUFFER_DESC ibd;
		ibd.Usage          = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth      = sizeof(UINT) * totalIndexCount;
		ibd.BindFlags      = D3D11_BIND_INDEX_BUFFER; // 인덱스
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags      = 0;

		D3D11_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = &indices[0];
		HR(mCoreStorage->md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
	}
	void Build_PNT_Ani()
	{
		//-----------------------------------------------------------------------------------------------------------//
		// 직전 버텍스 저장용
		int AfterVtxOffset = 0;

		// 개별 버텍스 오프셋 계산
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// 처음이라면
			if (AfterVtxOffset == 0)
			{
				// 현재 버텍스 저장
				AfterVtxOffset = itor->second->mVertexOffset;
				itor->second->mVertexOffset = 0;
			}
			else
			{
				// 내것과, 내 직전 걸 더한다.
				itor->second->mVertexOffset = AfterVtxOffset;
				AfterVtxOffset = itor->second->mVertexOffset + itor->second->Vertices.size();
			}
		}
		// 직전 인덱스 저장용
		int AfterIdxOffset = 0;

		// 개별 인덱스 오프셋 계산
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// 처음이라면
			if (AfterIdxOffset == 0)
			{
				// 현재 인덱스 저장
				AfterIdxOffset = itor->second->mIndexCount;
				itor->second->mIndexOffset = 0;
			}
			else
			{
				// 내것과, 내 직전 걸 더한다.
				itor->second->mIndexOffset = AfterIdxOffset;
				AfterIdxOffset = itor->second->mIndexOffset + itor->second->mIndexCount;
			}
		}

		// 법선벡터 계산
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// itor이 파싱된 모델이라면 넘어간다.
			if (itor->second->mModelType == e_ParsingModel)
				continue;

			// 법선벡터 계산
			CalNormalVtx(itor->second);
		}

		// 정점 늘리기 & 버텍스 버퍼로 송신
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// itor이 파싱된 모델이라면 넘어간다.
			if (itor->second->mModelType == e_ParsingModel)
				continue;

			// 정점 늘리기
			CalVtxINCAndSet(itor->second);
		}

		// 탄젠트 공간 계산
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// itor이 파싱된 모델이라면 넘어간다.
			if (itor->second->mModelType == e_ParsingModel)
				continue;

			// 탄젠트 공간 계산
			CalTangentSpace(itor->second);
		}


		// 총 버텍스 사이즈 계산
		vector<VertexPNTAni> vertices; // 한곳으로 옮기는 작업 ( 가장 큰 버퍼 )
		UINT totalVertexCount = 0;
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			totalVertexCount += itor->second->Vertices.size();
		}

		// 전체버퍼 사이즈 늘리기
		vertices.resize(totalVertexCount);

		// 가장 큰 버퍼로 복사
		UINT k = 0;
		int  count = 0;
		int  vtxCount = 0;
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// 이터레이터가 돌면서, 버텍스 크기만큼 더한다. (버텍스 별로 계산 되는 중)
			for (unsigned int i = 0; i < itor->second->Vertices.size(); ++i, ++k)
			{
				vertices[k].Tex     = itor->second->Vertices[i].TexUV;
				vertices[k].VtxInfo = XMFLOAT3((float)i, (float)itor->second->Vertices.size(), 0.0f); // 패딩값

				// 선택된 모델의 가중치 데이터
				vector<WeightVtx>& _sModelWeight = itor->second->weightVtx;

				// 정점별 가중치 갯수
				if (_sModelWeight.size() > i) // 혹시몰라 만들어둔 안전장치
				{
					for (int x = 0; x < 4; ++x)
					{
						// 가중치 3개랑, 본 인덱스 4개
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

		// 버텍스 버퍼 만들기
		D3D11_BUFFER_DESC vbd;
		vbd.Usage          = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth      = sizeof(VertexPNTAni)* totalVertexCount;
		vbd.BindFlags      = D3D11_BIND_VERTEX_BUFFER; // 버텍스
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags      = 0;

		D3D11_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = &vertices[0];
		HR(cCoreStorage::GetInstance()->md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));


		// 총 인덱스 사이즈 계산
		UINT totalIndexCount = 0;
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			totalIndexCount += itor->second->Indices.size();
		}

		// 인덱스 버퍼 만들기
		std::vector<UINT> indices;

		// 가장 큰 버퍼로 복사
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// 이터레이터가 돌면서, 인덱스 크기만큼 더한다.
			for (unsigned int i = 0; i < itor->second->Indices.size(); ++i)
			{
				indices.push_back(itor->second->Indices[i]);
			}
		}

		// 인덱스 버퍼 만들기
		D3D11_BUFFER_DESC ibd;
		ibd.Usage          = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth      = sizeof(UINT) * totalIndexCount;
		ibd.BindFlags      = D3D11_BIND_INDEX_BUFFER; // 인덱스
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags      = 0;

		D3D11_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = &indices[0];
		HR(mCoreStorage->md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
	}
	void Build_PNT_Map()
	{
		//-----------------------------------------------------------------------------------------------------------//
		// 직전 버텍스 저장용

		int AfterVtxOffset = 0;

		// 개별 버텍스 오프셋 계산
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// 처음이라면
			if (AfterVtxOffset == 0)
			{
				// 현재 버텍스 저장
				AfterVtxOffset = itor->second->mVertexOffset;
				itor->second->mVertexOffset = 0;
			}
			else
			{
				// 내것과, 내 직전 걸 더한다.
				itor->second->mVertexOffset = AfterVtxOffset;
				AfterVtxOffset = itor->second->mVertexOffset + itor->second->Vertices.size();
			}
		}
		// 직전 인덱스 저장용
		int AfterIdxOffset = 0;

		// 개별 인덱스 오프셋 계산
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// 처음이라면
			if (AfterIdxOffset == 0)
			{
				// 현재 인덱스 저장
				AfterIdxOffset = itor->second->mIndexCount;
				itor->second->mIndexOffset = 0;
			}
			else
			{
				// 내것과, 내 직전 걸 더한다.
				itor->second->mIndexOffset = AfterIdxOffset;
				AfterIdxOffset = itor->second->mIndexOffset + itor->second->mIndexCount;
			}
		}

		// 법선벡터 계산
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// itor이 파싱된 모델이라면 넘어간다.
			if (itor->second->mModelType == e_ParsingModel)
				continue;

			// 법선벡터 계산
			CalNormalVtx(itor->second);
		}

		// 정점 늘리기 & 버텍스 버퍼로 송신
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// itor이 파싱된 모델이라면 넘어간다.
			if (itor->second->mModelType == e_ParsingModel)
				continue;

			// 정점 늘리기
			CalVtxINCAndSet(itor->second);
		}

		// 탄젠트 공간 계산
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// itor이 파싱된 모델이라면 넘어간다.
			if (itor->second->mModelType == e_ParsingModel)
				continue;

			// 탄젠트 공간 계산
			CalTangentSpace(itor->second);
		}


		// 총 버텍스 사이즈 계산
		vector<VertexPNTMap> vertices; // 한곳으로 옮기는 작업 ( 가장 큰 버퍼 )
		UINT totalVertexCount = 0;
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			totalVertexCount += itor->second->Vertices.size();
		}

		// 전체버퍼 사이즈 늘리기
		vertices.resize(totalVertexCount);

		// 가장 큰 버퍼로 복사
		UINT k = 0;
		int  count = 0;
		int  vtxCount = 0;
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// 이터레이터가 돌면서, 버텍스 크기만큼 더한다. (버텍스 별로 계산 되는 중)
			for (unsigned int i = 0; i < itor->second->Vertices.size(); ++i, ++k)
			{
				vertices[k].Tex     = itor->second->Vertices[i].TexUV;
				vertices[k].VtxInfo = XMFLOAT2((float)i, (float)itor->second->Vertices.size()); // 버텍스 정보
			}
		}

		// 버텍스 버퍼 만들기
		D3D11_BUFFER_DESC vbd;
		vbd.Usage          = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth      = sizeof(VertexPNTMap)* totalVertexCount;
		vbd.BindFlags      = D3D11_BIND_VERTEX_BUFFER; // 버텍스
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags      = 0;

		D3D11_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = &vertices[0];
		HR(cCoreStorage::GetInstance()->md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));


		// 총 인덱스 사이즈 계산
		UINT totalIndexCount = 0;
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			totalIndexCount += itor->second->Indices.size();
		}

		// 인덱스 버퍼 만들기
		std::vector<UINT> indices;

		// 가장 큰 버퍼로 복사
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// 이터레이터가 돌면서, 인덱스 크기만큼 더한다.
			for (unsigned int i = 0; i < itor->second->Indices.size(); ++i)
			{
				indices.push_back(itor->second->Indices[i]);
			}
		}

		// 인덱스 버퍼 만들기
		D3D11_BUFFER_DESC ibd;
		ibd.Usage          = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth      = sizeof(UINT) * totalIndexCount;
		ibd.BindFlags      = D3D11_BIND_INDEX_BUFFER; // 인덱스
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags      = 0;

		D3D11_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = &indices[0];
		HR(mCoreStorage->md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
	}
	// 모델의 메타데이터( 정보를 가지고 있는) , 저장할 모델 
	void Build_SkyBox()
	{
		// 한곳으로 옮기는 작업 ( 가장 큰 버퍼 )
		vector<VertexSkyBox> vertices;

		// 총 버텍스 사이즈 계산
		UINT totalVertexCount = 0;
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			totalVertexCount += itor->second->Vertices.size();
		}

		// 총 인덱스 사이즈 계산
		UINT totalIndexCount = 0;
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			totalIndexCount += itor->second->Indices.size();
		}

		// 개별 버텍스 오프셋 계산
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// 직전 버텍스 저장
			int AfterVtxOffset = 0;

			// 처음이라면
			if (itor == mModelList.begin())
				itor->second->mVertexOffset = 0;
			else
				// 내것과, 내 직전 걸 더한다.
				itor->second->mVertexOffset = (itor->second->mVertexOffset + AfterVtxOffset);

			// 직전 버텍스 저장
			AfterVtxOffset = itor->second->mVertexOffset;
		}

		// 개별 인덱스 오프셋 계산
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// 직전 인덱스 저장
			int AfterIdxOffset = 0;

			// 처음이라면
			if (itor == mModelList.begin())
				itor->second->mIndexOffset = 0;
			else
				// 내것과, 내 직전 걸 더한다.
				itor->second->mIndexOffset = (itor->second->mIndexOffset + AfterIdxOffset);

			// 직전 인덱스 저장
			AfterIdxOffset = itor->second->mIndexOffset;
		}

		// 전체버퍼 사이즈 늘리기
		vertices.resize(totalVertexCount);

		// 가장 큰 버퍼로 복사
		UINT k = 0;
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// 이터레이터가 돌면서, 버텍스 크기만큼 더한다.
			for (unsigned int i = 0; i < itor->second->Vertices.size(); ++i, ++k)
			{
				vertices[k].Pos = itor->second->Vertices[i].Position;
				vertices[k].Pedding = 0.0f;			// 패딩 값
			}
		}

		// 버텍스 버퍼 만들기
		D3D11_BUFFER_DESC vbd;
		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = sizeof(VertexSkyBox) * totalVertexCount;
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // 버텍스
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = &vertices[0];
		HR(cCoreStorage::GetInstance()->md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));

		// 인덱스 버퍼 만들기
		std::vector<UINT> indices;

		// 가장 큰 버퍼로 복사
		for (map<string, InitMetaData*>::iterator itor = mModelList.begin(); itor != mModelList.end(); ++itor)
		{
			// 이터레이터가 돌면서, 인덱스 크기만큼 더한다.
			for (unsigned int i = 0; i < itor->second->Indices.size(); ++i)
			{
				indices.push_back(itor->second->Indices[i]);
			}
		}

		// 인덱스 버퍼 만들기
		D3D11_BUFFER_DESC ibd;
		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = sizeof(UINT) * totalIndexCount;
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER; // 인덱스
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = &indices[0];
		HR(mCoreStorage->md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
	}
	// 탄젠트 공간 계산
	void CalTangentSpace(InitMetaData* _MetaData)
	{
		// 저장 공간
		vector<XMVECTOR> tan1(_MetaData->Vertices.size());
		vector<XMVECTOR> tan2(_MetaData->Vertices.size());

		// 면 갯수 만큼
		for (int fi = 0; fi < (int)_MetaData->Indices.size() / 3; ++fi)
		{
			//	삼각형 데이터를 얻는다.
			WORD index1 = _MetaData->Indices[fi * 3 + 0];
			WORD index2 = _MetaData->Indices[fi * 3 + 1];
			WORD index3 = _MetaData->Indices[fi * 3 + 2];

			//	삼각형에 해당하는 정점3개를 구한다.
			XMVECTOR Vtx1 = XMLoadFloat3(&_MetaData->Vertices[index1].Position);
			XMVECTOR Vtx2 = XMLoadFloat3(&_MetaData->Vertices[index2].Position);
			XMVECTOR Vtx3 = XMLoadFloat3(&_MetaData->Vertices[index3].Position);

			// 삼각형에 해당하는 텍스터 정점3개를 구한다.
			XMVECTOR TexVtx1 = XMLoadFloat2(&_MetaData->Vertices[index1].TexUV);
			XMVECTOR TexVtx2 = XMLoadFloat2(&_MetaData->Vertices[index2].TexUV);
			XMVECTOR TexVtx3 = XMLoadFloat2(&_MetaData->Vertices[index3].TexUV);

			// 탄젠트 xyz 구하기
			float x1 = XMVectorGetX(Vtx2) - XMVectorGetX(Vtx1);
			float x2 = XMVectorGetX(Vtx3) - XMVectorGetX(Vtx1);
			float y1 = XMVectorGetY(Vtx2) - XMVectorGetY(Vtx1);
			float y2 = XMVectorGetY(Vtx3) - XMVectorGetY(Vtx1);
			float z1 = XMVectorGetZ(Vtx2) - XMVectorGetZ(Vtx1);
			float z2 = XMVectorGetZ(Vtx3) - XMVectorGetZ(Vtx1);

			// 탄젠트 w 구하기
			float s1 = XMVectorGetX(TexVtx2) - XMVectorGetX(TexVtx1);
			float s2 = XMVectorGetX(TexVtx3) - XMVectorGetX(TexVtx1);
			float t1 = XMVectorGetY(TexVtx2) - XMVectorGetY(TexVtx1);
			float t2 = XMVectorGetY(TexVtx3) - XMVectorGetY(TexVtx1);

			// 예외 셋팅
			XMVECTOR sdir;
			XMVECTOR tdir;
			if ((s1 * t2 - s2 * t1) == 0.0f)
			{
				sdir = XMLoadFloat3(&XMFLOAT3(1.0f, 0.0f, 0.0f));
				tdir = XMLoadFloat3(&XMFLOAT3(0.0f, 1.0f, 0.0f));
			}
			else
			{
				// 공식 대입
				float r = 1.0F / (s1 * t2 - s2 * t1);
				sdir = XMLoadFloat3(&XMFLOAT3((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r));
				tdir = XMLoadFloat3(&XMFLOAT3((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r));
			}

			// 값 누적
			tan1[index1] += sdir;
			tan1[index2] += sdir;
			tan1[index3] += sdir;

			tan2[index1] += tdir;
			tan2[index2] += tdir;
			tan2[index3] += tdir;
		}

		// 값 복사
		for (int v = 0; v < (int)_MetaData->Vertices.size(); ++v)
		{
			const XMVECTOR& n = XMLoadFloat3(&_MetaData->Vertices[v].Normal);
			const XMVECTOR& t = tan1[v];

			// Gram-Schmidt orthogonalize
			// 탄젠트 Vtx 저장하기 (xyz)
			XMStoreFloat3(&_MetaData->Vertices[v].TangentU, XMVector3Normalize((t - n * XMVector3Dot(n, t))));

			// Calculate handedness
			//// 텍스처 좌표 방향벡터 구한다. (w) <-- 일단 안구함 (나중에 구해서 쓸꺼)
			float TangentU = (XMVectorGetX(XMVector3Dot(XMVector3Cross(n, t), tan2[v])) < 0.0F) ? -1.0f : 1.0f;

			// 외적하여 계산한다
			XMVECTOR BiNormal = XMVector3Cross(XMLoadFloat3(&_MetaData->Vertices[v].TangentU), n) * TangentU;

			// 결과 값 저장
			XMStoreFloat3(&_MetaData->Vertices[v].BiNormal, BiNormal);
		}

		tan1.clear();
		tan2.clear();
	}


	// 정점 증가코드 1

	//// 정점 늘리기 & 버텍스 버퍼로 송신
	////template <class T>
	////void CalNormalVtx(T& _vertices, cTestModel::InitMetaData& _MetaData)
	//void CalVtxINCAndSet(vector<VertexPNT>& vertices, cTestModel::InitMetaData& _MetaData)
	//{
	//
	//	// 메타데이터를 변경해서, 정점을 늘려줘야한다.
	//	// 의도는: 버텍스 좌표와, uv좌표는 불일치 할수 있다는 것에서 출발
	//	//         그러므로, 버텍스 좌표와 uv 좌표를 각각 뽑아내, 두개가 일치하면 내비두고,
	//	//         불일치시, 버텍스 좌표는 그대로 사용하고, uv 좌표를 수정하여 정점을 추가로 후미에 배치한다.
	//	//         이후, 추가한 정점정보를 바탕으로, 모델 인덱스리스트를 다시 작성한다.
	//	
	//	// *핵심: uv 좌표 넣기전, 비어있는 공간인지 확인
	//	//        1) 비어있는 공간이라면 삽입,
	//	//        2) 비어 있지 않는 공간이라면, 비교
	//	//           -> 넣으려는 uv 좌표가 일치한다면, 아무작업 안하고, 다음 좌표로
	//	//           -> 넣으려는 uv 좌표가 일치하지 않는다면, 새로 생성 ( 버텍스 좌표 복사, uv 좌표 복사 )
	//	//        3) 정점을 추가하면, 현재 검색하고 있는 모델 인덱스에, 해당 정점 번호를 넣는다.
	//
	//	// 정점 늘리기에 사용되는 계산용
	//	vector<TexUV> _TexUV;
	//
	//	// 현재 생성되어있는 정점 수
	//	unsigned int CntVtx = _MetaData->Vertices.size();
	//	for (unsigned int v = 0; v < CntVtx; ++v)
	//	{
	//		// 모델의 인덱스 크기만큼 반복 ( 텍스처 인덱스도 일반적으로는 동일하므로, 비동일한 모델 처리시 예외 처리 필요 )
	//		for (unsigned int fi = 0; fi < _MetaData->Indices.size(); ++fi)
	//		{
	//			// 순서대로 저장한 정점번호 안에 해당 정점과 같은 정점이 있냐
	//			if (_MetaData->Indices[fi] == v)
	//				CalTexVtx(_MetaData, v, _MetaData->Indices[fi], _MetaData->TexIndices[fi], _TexUV);
	//		}// 반복
	//		//해당 버텍스에 사용된 UV번호, 생성된 정점번호 클리어
	//		_TexUV.clear();
	//	}// 반복
	//}
	//
	//void CalTexVtx(cTestModel::InitMetaData& _MetaData, int v, UINT& Modelfi, UINT& Texfi, vector<TexUV>& _TexUV)
	//{
	//	// 모델 정보
	//	Vertex  & ThisModel = _MetaData->Vertices[v];
	//	XMFLOAT3& ThisTex   = _MetaData->TexVertices[Texfi];
	//
	//	// 현재 검색하고 있는 버텍스 정점의 Uv정점이 비었는가?(사용한 흔적이 없는, 초기 셋팅 값인가?)
	//	if (ThisModel.TexUV.x == FLT_MAX || ThisModel.TexUV.y == FLT_MAX)
	//	{
	//		// 예
	//		// 모델 uv <= 텍스처 uv;
	//		ThisModel.TexUV.x = ThisTex.x;
	//		ThisModel.TexUV.y = ThisTex.y;
	//
	//		// 사용한 uv와 버텍스 저장
	//		TexUV tUv;	
	//		tUv.Uv  = Texfi;
	//		tUv.Vtx = v;
	//		_TexUV.push_back(tUv);
	//	}
	//	else
	//	{
	//		// 아니오
	//		// 사용한 기록이 있다면,
	//		// 추가할 UV가 이미 추가 된 UV는 아닌가?
	//		// - 더 늘릴필요가 없기에 인덱스만 변경
	//
	//		//검사
	//		UINT ItorUV = 0;
	//		bool UseUVCheck = false;
	//		for (unsigned int i = 0; i < _TexUV.size(); ++i)
	//		{
	//			if (_TexUV[i].Uv == Texfi) //저장된 버텍스와, tex인덱스가 가르키는 버텍스가 같은가?
	//			{
	//				ItorUV     = i;
	//				UseUVCheck = true;
	//				break;
	//			}
	//		}
	//
	//		//검사 결과
	//		if (!UseUVCheck)
	//		{
	//			// 인덱스[현재 인덱스 번호].몇번째 정점 = 새로 추가 된 버텍스 정점의 번호
	//			Modelfi = _MetaData->Vertices.size();
	//
	//			// 예 - 새로운 정점이다
	//			// 정점 늘리기
	//			Vertex vtx =  ThisModel;
	//			vtx.TexUV.x = ThisTex.x;
	//			vtx.TexUV.y = ThisTex.y;
	//			_MetaData->Vertices.push_back(vtx);
	//
	//			// 사용한 정점으로 표시
	//			// 새로 추가된 정점 저장 = 새로 추가된 정점은 항상 각 벡터 리스트의 마지막에 있을 것이므로.
	//			TexUV tUv;
	//			tUv.Uv  = Texfi;
	//			tUv.Vtx = _MetaData->Vertices.size(); // -1 일부로 안함
	//			_TexUV.push_back(tUv);
	//		}
	//		else
	//		{// 아니오 - 추가된 정점이다
	//			Modelfi = _TexUV[ItorUV].Vtx;
	//		}
	//	}
	//	return;
	//}

	// 정점 증가코드 2
	void CalVtxINCAndSet(InitMetaData* _MetaData)
	{
		// 메타데이터를 변경해서, 정점을 늘려줘야한다.
		// 의도는: 버텍스 좌표와, uv좌표는 불일치 할수 있다는 것에서 출발
		//         그러므로, 버텍스 좌표와 uv 좌표를 각각 뽑아내, 두개가 일치하면 내비두고,
		//         불일치시, 버텍스 좌표는 그대로 사용하고, uv 좌표를 수정하여 정점을 추가로 후미에 배치한다.
		//         이후, 추가한 정점정보를 바탕으로, 모델 인덱스리스트를 다시 작성한다.
		//
		// *핵심: uv 좌표 넣기전, 비어있는 공간인지 확인
		//        1) 비어있는 공간이라면 삽입,
		//        2) 비어 있지 않는 공간이라면, 비교
		//           -> 넣으려는 uv 좌표가 일치한다면, 아무작업 안하고, 다음 좌표로
		//           -> 넣으려는 uv 좌표가 일치하지 않는다면, 새로 생성 ( 버텍스 좌표 복사, uv 좌표 복사 )
		//        3) 정점을 추가하면, 현재 검색하고 있는 모델 인덱스에, 해당 정점 번호를 넣는다.


		// 모델의 인덱스 크기만큼 반복 ( 텍스처 인덱스도 일반적으로는 동일하므로, 비동일한 모델 처리시 예외 처리 필요 )
		for (unsigned int i = 0; i < _MetaData->Indices.size(); ++i)
		{
			// 현재 인덱스 번호를 검색한다.
			UINT _ModelIdx = _MetaData->Indices[i];    //  모델의 현재 인덱스
			UINT _TexIdx   = _MetaData->TexIndices[i]; // 텍스처의 현재 인덱스

			// 각각의 인덱스 번호에 해당하는 uv값을 꺼낸다.
			XMFLOAT2& ModelTexUV = _MetaData->Vertices[_ModelIdx].TexUV; //   모델 인덱스의 UV 값
			XMFLOAT3&      TexUV = _MetaData->TexVertices[_TexIdx];      // 텍스처 인덱스의 UV 값

			// 모델의 현재 인덱스가 가르키는 버텍스의 uv가 비어있는가?
			if (ModelTexUV.x == FLT_MAX || ModelTexUV.y == FLT_MAX)
			{
				// 비어있다면,
				// 텍스처 uv 값 바로 삽입
				ModelTexUV.x = TexUV.x;
				ModelTexUV.y = TexUV.y;
			}
			else
			{
				// 비어 있지 않다면
				// 기존 모델의 UV와 현재 텍스처의 UV가 일치하는가?
				if (ModelTexUV.x == TexUV.x && ModelTexUV.y == TexUV.y)
				{
					// 일치한다면, 

				}
				else
				{
					// 일치하지 않는다면, 정점 늘리기
					// 해당 버텍스의 정보를 꺼낸다
					Vertex ModelVtx = _MetaData->Vertices[_ModelIdx];

					// 텍스처 정보 변경
					ModelTexUV.x = TexUV.x;
					ModelTexUV.y = TexUV.y;

					// 복사
					_MetaData->Vertices.push_back(ModelVtx);
				}
			}
		}
	}

	// 법선 계산
	void CalNormalVtx(InitMetaData* _MetaData)
	{
		vector< vector<XMVECTOR> > face_normal;

		//면 노멀
		face_normal.resize(_MetaData->Indices.size() / 3);
		for (unsigned int i = 0; i < face_normal.size(); ++i)
			face_normal[i].resize(2);

		//-----------------------------------//
		//면 노멀법선
		//-----------------------------------//
		//삼각형 갯수만큼 반복
		for (int fi = 0; fi < (int)_MetaData->Indices.size() / 3; ++fi)
		{
			//	삼각형 데이터를 얻는다.
			WORD index1 = _MetaData->Indices[fi * 3 + 0];
			WORD index2 = _MetaData->Indices[fi * 3 + 1];
			WORD index3 = _MetaData->Indices[fi * 3 + 2];

			//	삼각형에 해당하는 정점3개를 구한다.
			XMVECTOR vtx1 = XMLoadFloat3(&_MetaData->Vertices[index1].Position);
			XMVECTOR vtx2 = XMLoadFloat3(&_MetaData->Vertices[index2].Position);
			XMVECTOR vtx3 = XMLoadFloat3(&_MetaData->Vertices[index3].Position);

			// 면벡터의 중간 지점을 저장한다.
			face_normal[fi][0] = vtx1 + vtx2 + vtx3;

			//	한개 정점에서 다른 두개 정점을 향하는 방향벡터 두개를 구한다.
			XMVECTOR vtx_dir1 = vtx2 - vtx1;
			XMVECTOR vtx_dir2 = vtx3 - vtx1;

			//	방향 벡터 2개를 정규화 한다.
			vtx_dir1 = XMVector3Normalize(vtx_dir1);
			vtx_dir2 = XMVector3Normalize(vtx_dir2);

			//	외적하여 법선벡터를 구한다.
			XMVECTOR normal_dir = XMVector3Cross(vtx_dir1, vtx_dir2);

			// 면벡터의 뻣친 지점을 저장한다.
			face_normal[fi][1] = face_normal[fi][0] + normal_dir * 1.0f;

		} //반복

		//-----------------------------------//
		//정점 노멀법선
		//-----------------------------------//
		//정점갯수만큼 반복 
		for (unsigned int vi = 0; vi < _MetaData->Vertices.size(); ++vi) //<- 정점번호 0부터 순차검색
		{
			//	누적벡터 0,0,0 초기화 
			XMVECTOR result_vec = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

			//	누적갯수 0 초기화 
			int result_count = 0;

			//	삼각형 갯수만큼 반복
			for (unsigned int fi = 0; fi < _MetaData->Indices.size() / 3; ++fi)
			{
				//		삼각형 데이터를 얻는다.
				WORD index1 = _MetaData->Indices[fi * 3 + 0];
				WORD index2 = _MetaData->Indices[fi * 3 + 1];
				WORD index3 = _MetaData->Indices[fi * 3 + 2];

				//		삼각형 데이터 중에 정점 인덱스가 잇냐?
				if ((index1 == (WORD)vi) || (index2 == (WORD)vi) || (index3 == (WORD)vi)) //<- vi는 정점번호, index에는 각각의 정점번호가 들어가있다.
				{   //		예
					//			누적벡터 에 면 법선벡터를 더한다.
					XMVECTOR tmp_dir = face_normal[fi][1] - face_normal[fi][0]; //방향벡터를 누적해야함.
					tmp_dir = XMVector3Normalize(tmp_dir);

					// 결과 벡터에 누적
					result_vec += tmp_dir;

					// 누적갯수 1 증가.
					++result_count;
				}
			}//	반복
			//	누적벡터 /= 누적갯수
			result_vec /= (float)result_count;

			//노멀라이즈
			result_vec = XMVector3Normalize(result_vec);

			//정점(법선)에 법선벡터 저장
			XMStoreFloat3(&_MetaData->Vertices[vi].Normal, result_vec);

		}//반복

		// 자원 제거
		// 인덱스
		for (unsigned int i = 0; i < face_normal.size(); ++i)
		{
			face_normal[i].clear();
		}
		face_normal.clear();
	}
};

// 구성된 매쉬 데이터
class MyMeshData
{
public:
	// 정점 버퍼
	vector<Vertex> vertices;

	// 인덱스 리스트
	vector<XMFLOAT3> indices;

	// 애니 데이터 ( 모델 자체 )
	AniData mAniData;

	// 가중치 데이터
	vector<WeightVtx> weightVtx;

	// 오프셋
	UINT vertexOffset;
	UINT indexOffset;

	// 카운트
	UINT indexCount;

	// 재질 정보
	MyMat mMyMat;

	// TM 행렬
	XMFLOAT4X4 mTMLocalMtx;
	XMFLOAT4X4 mTMWorldMtx;

	// 바운딩 박스
	BoundBox mBoundingBox;

	//--------------------------------------------------//
	// 오브젝트 식별 정보
	//--------------------------------------------------//

	// 이름
	char mMainName[BUF_SIZE];

	// 오브젝트이름
	int  mObjID;
	char mObjName[BUF_SIZE];
	char mObjClass[BUF_SIZE];

	// 상위 클래스 이름
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

// 부모 정보를 가지고 있는 값
class BoneParentData
{
public:
	string mName;    // 내 부모의 이름
	XMFLOAT4X4 mLAP; // 내가 아래 자식에게 보낼 매트릭스
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


// 본 데이터
class NodeBone
{
public:
	// TM 행렬
	XMFLOAT4X4 mTMLocalMtx;
	XMFLOAT4X4 mTMWorldMtx;

	// 역 월드(B역)
	XMFLOAT4X4 mInvWorldTMMtx;

	// 애니 데이터
	AniData mAniData;

	// 바운딩 박스
	BoundBox mBoundingBox;

	//--------------------------------------------------//
	// 오브젝트 식별 정보
	//--------------------------------------------------//
	// 오브젝트이름
	int  mObjID;
	char mObjName[BUF_SIZE];
	char mObjClass[BUF_SIZE];

	// 상위 클래스 이름
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

	// 애니 데이터
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
		
		// 애니메이션 데이터 계산
		//XMVECTOR _scaleKey = XMLoadFloat3(&mAniData.Scale[_key].Vtx);        // 스케일 값
		XMVECTOR _rotKey   = XMLoadFloat4(&mAniData.Quaternion[_key].Vtx);   // 회전
		//XMVECTOR _posKey   = XMLoadFloat3(&mAniData.Position[_key].Vtx);     // 위치
		//XMVECTOR _zero     = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);			 // 회전 중점

		// 아핀변환 행렬		
		//XMStoreFloat4x4(&mAniMtx, XMMatrixAffineTransformation(_scaleKey, _zero, _rotKey, _posKey));

		// 주어진 사원수로 회전행렬을 구한다.
		XMStoreFloat4x4(&mAniMtx, XMMatrixRotationQuaternion(_rotKey));

		// 이동 적용
		mAniMtx._41 = mAniData.Position[_key].Vtx.x;
		mAniMtx._42 = mAniData.Position[_key].Vtx.y;
		mAniMtx._43 = mAniData.Position[_key].Vtx.z;
		
		// 스케일 적용
		mAniMtx._11 *= mAniData.Scale[_key].Vtx.x;
		mAniMtx._22 *= mAniData.Scale[_key].Vtx.y;
		mAniMtx._33 *= mAniData.Scale[_key].Vtx.z;

		return mAniMtx;
	}
};

// 스키닝 트리
class SkinTree
{
public:
	// 부모 데이터 (가변 데이터 / 행렬만)
	BoneParentData mParentData;

	// 내 데이터 (변하지 않음)
	map<string, NodeBone> mData;

	// 자식 데이터 (변하지 않음)
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

	// 스킨행렬 만들기, 하향 계산하기 (자식 본)
	void MakeSkin(int _AniPoint, SkinTree& _SkinTree, vector< map<string, XMFLOAT4X4>>& _SkinMtx, vector< map<string, XMFLOAT4X4>>& _LAP)
	{
		// 선택된 자식 본
		auto itor = _SkinTree.mData.begin(); // 사실상 하나 밖에 없음

		// 애니 매트릭스
		auto _aniMtx = itor->second.getAniMtx(_AniPoint);

		XMMATRIX tLoclMtx = XMLoadFloat4x4(&itor->second.mTMLocalMtx);
		XMMATRIX tAniMtx  = XMLoadFloat4x4(&_aniMtx);
		XMMATRIX tParMtx, tResultMtx, tResult;

		// 현 본의 LA 처리
		tResultMtx = XMMatrixMultiply(tLoclMtx, tAniMtx);

		XMFLOAT4X4 _tResultMtx;
		XMStoreFloat4x4(&_tResultMtx, tResultMtx);

		//-------------------------------------------------------------------------------------//
		// 만약 pos키값이 없으면 local TM의 좌표를 사용한다
		//-------------------------------------------------------------------------------------//
		if (_aniMtx._41 == 0.0f && _aniMtx._42 == 0.0f && _aniMtx._43 == 0.0f)
		{
			_tResultMtx._41 = itor->second.mTMLocalMtx._41;
			_tResultMtx._42 = itor->second.mTMLocalMtx._42;
			_tResultMtx._43 = itor->second.mTMLocalMtx._43;
		}
		else	// pos키값을 좌표값으로 적용한다(이렇게 하지 않으면 TM의 pos성분이 두번적용된다)
		{
			_tResultMtx._41 = _aniMtx._41;
			_tResultMtx._42 = _aniMtx._42;
			_tResultMtx._43 = _aniMtx._43;
		}
		//-------------------------------------------------------------------------------------//
		tResultMtx = XMLoadFloat4x4(&_tResultMtx);

	
		// P처리 (부모 데이터 누적)
		tParMtx = XMLoadFloat4x4(&_SkinTree.mParentData.mLAP);
		tResult = XMMatrixMultiply(tResultMtx, tParMtx); // LAP
	
		// 자식에게 넘길 데이터로 저장
		XMStoreFloat4x4(&_SkinTree.mParentData.mLAP, tResult); // _SkinTree.mParentData.mLAP 는 내가 아래 자식에게 보낼 매트릭스 저장공간

		// LAP 에 저장
		_LAP[_AniPoint][itor->first] = _SkinTree.mParentData.mLAP;

		// SKIN 에 저장
		XMFLOAT4X4 tSkinMtx;
		XMMATRIX tInvWDMtx = XMLoadFloat4x4(&itor->second.mInvWorldTMMtx);
		XMStoreFloat4x4(&tSkinMtx, XMMatrixMultiply(tInvWDMtx, tResult));
		_SkinMtx[_AniPoint][itor->first] = tSkinMtx;

		// 자식 map 갯수만큼 반복
		for (auto itorChild = _SkinTree.mChildData.begin(); itorChild != _SkinTree.mChildData.end(); ++itorChild)
		{
			// 선택된 자식에, 부모 데이터 넘기기
			itorChild->second.mParentData.mLAP = _SkinTree.mParentData.mLAP;

			// 함수(자식본번지) 애니 키와, (애니메이션은) AP임..!!? 넘기기
			MakeSkin(_AniPoint, itorChild->second, _SkinMtx, _LAP);
		}
	}

	// 스킨행렬 만들기, 하향 계산하기 (루트 본)
	void MakeSkin(int _AniPoint, vector< map<string, XMFLOAT4X4>>& _SkinMtx, vector< map<string, XMFLOAT4X4>>& _LAP)
	{
		// 데이터가 다수일 경우.. (루트 빼곤 없을 듯..)
		//for (auto itor = mData.begin(); itor != mData.end(); ++itor)
		{
			auto itor = mData.begin();   // Dummy_root
			auto itor2 = ++itor; --itor; // NULL

			// 애니 매트릭스
			auto _aniMtx  = itor ->second.getAniMtx(_AniPoint);	  // Dummy_root
			auto _aniMtx2 = itor2->second.getAniMtx(_AniPoint);	  // NULL

			// Dummy_root
			XMMATRIX tLoclMtx = XMLoadFloat4x4(&itor->second.mTMLocalMtx);
			XMMATRIX tAniMtx  = XMLoadFloat4x4(&_aniMtx);

			// NULL
			XMMATRIX tLoclMtx2 = XMLoadFloat4x4(&itor2->second.mTMLocalMtx);
			XMMATRIX tAniMtx2  = XMLoadFloat4x4(&_aniMtx2);

			XMMATRIX tResultMtx, tResultMtx2;

			// 현 본의 LA 처리
			tResultMtx  = XMMatrixMultiply(tLoclMtx , tAniMtx);
			tResultMtx2 = XMMatrixMultiply(tLoclMtx2, tAniMtx2);

			XMFLOAT4X4 _tResultMtx, _tResultMtx2;
			XMStoreFloat4x4(&_tResultMtx , tResultMtx);	 // Dummy_root
			XMStoreFloat4x4(&_tResultMtx2, tResultMtx2); // NULL

			//-------------------------------------------------------------------------------------//
			// 만약 pos키값이 없으면 local TM의 좌표를 사용한다
			//-------------------------------------------------------------------------------------//
			// Dummy_root
			if (_aniMtx._41 == 0.0f && _aniMtx._42 == 0.0f && _aniMtx._43 == 0.0f)
			{
				_tResultMtx._41 = itor->second.mTMLocalMtx._41;
				_tResultMtx._42 = itor->second.mTMLocalMtx._42;
				_tResultMtx._43 = itor->second.mTMLocalMtx._43;
			}
			else	// pos키값을 좌표값으로 적용한다(이렇게 하지 않으면 TM의 pos성분이 두번적용된다)
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
			else	// pos키값을 좌표값으로 적용한다(이렇게 하지 않으면 TM의 pos성분이 두번적용된다)
			{
				_tResultMtx2._41 = _aniMtx2._41;
				_tResultMtx2._42 = _aniMtx2._42;
				_tResultMtx2._43 = _aniMtx2._43;
			}
			//-------------------------------------------------------------------------------------//

			tResultMtx  = XMLoadFloat4x4(&_tResultMtx);	  // Dummy_root
			tResultMtx2 = XMLoadFloat4x4(&_tResultMtx2);  // NULL

			// LAP 에 저장 (자식에게 넘길 데이터 저장)
			XMStoreFloat4x4(&mParentData.mLAP, tResultMtx);	    // Dummy_root
			_LAP[_AniPoint][itor ->first] = mParentData.mLAP;

			XMStoreFloat4x4(&mParentData.mLAP, tResultMtx2);	// NULL
			_LAP[_AniPoint][itor2->first] = mParentData.mLAP;

			tResultMtx = XMMatrixMultiply(tResultMtx2, tResultMtx);

			// SKIN 에 저장
			XMFLOAT4X4 tSkinMtx;
			XMMATRIX tInvWDMtx = XMLoadFloat4x4(&itor->second.mInvWorldTMMtx);
			XMStoreFloat4x4(&tSkinMtx, XMMatrixMultiply(tInvWDMtx, tResultMtx));
			_SkinMtx[_AniPoint][itor ->first] = tSkinMtx; // Dummy_root

			tInvWDMtx = XMLoadFloat4x4(&itor2->second.mInvWorldTMMtx);
			XMStoreFloat4x4(&tSkinMtx, XMMatrixMultiply(tInvWDMtx, tResultMtx2));
			_SkinMtx[_AniPoint][itor2->first] = tSkinMtx; // NULL

			// 자식 map 갯수만큼 반복
			for (auto itorChild = mChildData.begin(); itorChild != mChildData.end(); ++itorChild)
			{
				// 선택된 자식에, 부모 데이터 넘기기
				XMStoreFloat4x4(&mParentData.mLAP, tResultMtx); // Dummy_root
				itorChild->second.mParentData.mLAP = mParentData.mLAP;

				// 함수(자식본번지) 애니 키와, AP 넘기기
				MakeSkin(_AniPoint, itorChild->second, _SkinMtx, _LAP);
			}
		}
	}

	// 키 추가하기 (트리 만들때 사용)
	void AddKey(map<string, NodeBone>& _BoneData, vector<KeyString>& _KeyRoute, int _MaxSize, int _Count)
	{
		// 끝까지 순회했는가
		int tMaxSize = _MaxSize - 1;
		if (tMaxSize == _Count)
		{
			// 부모 이름 넣기
			int tParCount = _Count - 1;
			if (tParCount > 0)
				mParentData.mName = _KeyRoute[tParCount].Name;

			// 선택된 본 이름
			string tSelctBone = _KeyRoute[_Count].Name;

			// 해당 데이터를 찾아 넣는다
			mData[tSelctBone] = _BoneData[tSelctBone];

			// 이름 삭제
			tSelctBone.clear();
		}
		// 다음키가 있다면
		else
		{
			int tCount = _Count + 1;
			string _selectName = _KeyRoute[tCount].Name;

			if (mChildData.size() == 0)
				mChildData[_selectName] = SkinTree();

			// 부모 이름 넣기
			mChildData[_selectName].mParentData.mName = _KeyRoute[_Count].Name;

			mChildData[_selectName].AddKey(_BoneData, _KeyRoute, _MaxSize, tCount);
			_selectName.clear();
		}
	}

	// 키 찾아가기 (그냥 만들어둠)
	NodeBone* FindKey(string _key)
	{
		// 키가 일치하는 가
		if (mData[_key].mObjName == _key)
		{
			// 일치한 키
			return &mData[_key];
		}
		else
		{
			// 자식을 순서대로 찾아간다.
			for (auto itor = mChildData.begin(); itor != mChildData.end(); ++itor)
			{
				itor->second.FindKey(_key);
			}
		}
		_key.clear();
		return nullptr;
	}
};

// 각 모델들의 본, 애니메이션 정보를 담아둠
class MyBoneData
{
public:
	// 계층 구조를 담은 본 정보(파싱용)
	vector<NodeBone> mSaveBoneData;

	// 계층 구조를 담은 본 정보(실제 사용)
	map<string, NodeBone> mBoneData;

	// 계층 구조를 실질적으로 찾아가는 정보
	// 일종의 해쉬맵
	vector<vector<KeyString> > mBoneHierarchy;
	
	// 스킨 트리
	SkinTree mSkinTree;

	// 본 SKin 텍스처 (사용)
	SkinTexture mSkinTex;

	// 본 SKin 매트릭스 (계산용)
	vector<map<string, XMFLOAT4X4>> mSkinMtx;

	// 본 SKin 매트릭스 (계산용)
	vector<vector<XMFLOAT4X4>> mRelocSkinMtx;

	// LAP 행렬 (테스트용)
	vector<map<string, XMFLOAT4X4>> mLAP;

	// LAP 행렬 (테스트용)
	vector<vector<XMFLOAT4X4>> mRelocLAP;
	
	// 이름
	char mMainName[BUF_SIZE];
	char mAniName[BUF_SIZE];

public:
	~MyBoneData()
	{
		// 삭제
		ClearClass();
	}

	// 데이터 계산
	void CalData(string _TexName, FSM_TYPE _fsmType)
	{
		// 텍스트가 있냐 없냐
		string tTexName;

		tTexName += "Export/SkinTex/";
		tTexName += _TexName;
		tTexName += ".dds";

		//--------------------------------------------------------------------------------------------------//
		// 애니 데이터 채우기
		//--------------------------------------------------------------------------------------------------//
		// 이름 저장
		mSkinTex.mName = _TexName;

		// 애니 타입 저장
		mSkinTex.mAniType = (float)_fsmType;

		// 애니 처음, 마지막 위치 저장
		mSkinTex.mStPoint = 0.0f;
		mSkinTex.mEdPoint = (float)mSaveBoneData[0].mAniData.Position.size();

		// 텍스처 해상도
		mSkinTex.mTexWidth  = (float)mSaveBoneData.size() * 4;
		mSkinTex.mTexHeight = (float)mSaveBoneData[0].mAniData.Position.size();

		//--------------------------------------------------------------------------------------------------//
		// 파일 경로 문자열
		//--------------------------------------------------------------------------------------------------//
		wstring _WsTexName;
		StringToWchar_t(tTexName, _WsTexName);

		//--------------------------------------------------------------------------------------------------//
		// 텍스처 연결
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

		// 텍스처가 없다
		if (hr == D3D11_ERROR_FILE_NOT_FOUND)
		{
			// 없다면 텍스처 생성
			cout << "SkinTex 파일이 없어 생성합니다. 다소 시간이 걸릴 수 있습니다." << endl;
	
			// 데이터 옮기기
			RelocationData();

			//트리 만들기
			MakeTree();

			// 스킨 텍스처 만들기
			MakeSkinTex(_WsTexName);

			// 텍스처 스킨 다시 로딩
			goto RetryLoadTex;
		}
		else if (hr == S_OK)
			cout << "SkinTex 로딩 성공" << endl;
		else
			cout << ">> 명시되지 않은 에러 발생. SkinTex 파일 <<" << endl;

		_WsTexName.clear();
	}

	// LAP 리턴
	vector<XMFLOAT4X4>& GetLapStorage(int _aniKey)
	{
		return mRelocLAP[_aniKey];
	}

	// 스킨 행렬 리턴
	vector<XMFLOAT4X4>& GetSkinStorage(int _aniKey)
	{
		return mRelocSkinMtx[_aniKey];
	}

	void ClearClass()
	{
		// 본 데이터 삭제
		mSaveBoneData.clear();
		mBoneData.clear();
		
		// 계산용 변수 비우기
		ClearCalValue();

		// 하이라이키 삭제
		for (unsigned int i = 0; i < mBoneHierarchy.size(); ++i)
			mBoneHierarchy[i].clear();
		mBoneHierarchy.clear();
	}

private:
	// 데이터 재배치
	void RelocationData()
	{
		// map으로 옮김
		for (unsigned int i = 0; i < mSaveBoneData.size(); ++i)
		{
			// 회전 누적 계산
			vector<RotKeyVtx>& _QuaternionArray = mSaveBoneData[i].mAniData.Quaternion;

			_QuaternionArray[0].Vtx.x = 0.0f;
			_QuaternionArray[0].Vtx.y = 0.0f;
			_QuaternionArray[0].Vtx.z = 0.0f;
			_QuaternionArray[0].Vtx.w = 1.0f;

			for (unsigned int x = 1; x < _QuaternionArray.size(); ++x)
			{
				// 값을 꺼내온다
				XMVECTOR beforeRot  = XMLoadFloat4(&_QuaternionArray[x - 1].Vtx); // 직전
				XMVECTOR CurrentRot = XMLoadFloat4(&_QuaternionArray[x].Vtx);     // 현재
				XMVECTOR ResultRot;

				// 사원수 회전을 누적시킨다.
				ResultRot = XMQuaternionMultiply(beforeRot, CurrentRot);

				// 저장한다
				XMStoreFloat4(&_QuaternionArray[x].Vtx, ResultRot);
			}

			// 역행렬 만들기 및 저장
			XMMATRIX tWdMtx  = XMLoadFloat4x4(&mSaveBoneData[i].mTMWorldMtx);
			XMVECTOR tDet    = XMMatrixDeterminant(tWdMtx);
			XMMATRIX tInvMtx = XMMatrixInverse(&tDet, tWdMtx);

			XMStoreFloat4x4(&mSaveBoneData[i].mInvWorldTMMtx, tInvMtx);

			// 키에 맞게 재배치
			string tString = mSaveBoneData[i].mObjName;
			mBoneData[tString] = mSaveBoneData[i];
			tString.clear();
		}
	}

	
	// 스킨 텍스처 만들기
	void MakeSkinTex(wstring& _TexName)
	{
		// 모든 키는 동일한 갯수를 가진다.
		int tAniSize = mSaveBoneData[0].mAniData.Position.size();

		// 텍스처 생성
		CreateTex(tAniSize);

		// 텍스처 계산
		CalTex(tAniSize);

		// 텍스처 저장
		SaveTex(_TexName);
	}

	// 텍스처 생성
	void CreateTex(int _AniSize)
	{
		D3D11_TEXTURE2D_DESC TextureDesc;
		ZeroMemory(&TextureDesc, sizeof(TextureDesc));

		TextureDesc.Width              = mSaveBoneData.size() * 4; // 옆으로 행렬 나열 (행 4칸씩, 1개)
		TextureDesc.Height             = _AniSize;				    // 애니 사이즈 만큼 (초)
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

	// 텍스처 계산
	void CalTex(int _AniSize)
	{
		// 계산용 변수 비우기
		ClearCalValue();

		// 계산용 변수 공간확보
		ResizeCalValue();

		// 스킨 데이터만들기
		MakeSkinMtx(_AniSize);

		// 스킨 데이터 순서에 맞게 재배치
		RelocSkinData();

		// 텍스처 쓰기
		WriteTex(_AniSize);
	}

	// 스킨 데이터 만들기
	void MakeSkinMtx(int _AniSize)
	{
		for (int i = 0; i < _AniSize; ++i)
			mSkinTree.MakeSkin(i, mSkinMtx, mLAP);
	}

	// 스킨 데이터 순서에 맞게 재배치
	void RelocSkinData()
	{
		// 애니메이션 키 갯수 만큼
		for (unsigned int i = 0; i < mSkinMtx.size(); ++i)
		{
			// 본 갯수
			for (unsigned int x = 0; x < mSaveBoneData.size(); ++x)
			{
				// 이름 선택
				string _getName = mSaveBoneData[x].mObjName;
				mRelocSkinMtx[i].push_back(mSkinMtx[i][_getName]); // i번키에, 선택된 본을 넣어라 
				mRelocLAP    [i].push_back(mLAP    [i][_getName]);
			}
		}
	}

	
	// 텍스처 쓰기
	void WriteTex(int _AniSize)
	{
		D3D11_MAPPED_SUBRESOURCE MappedResource;

		// 버퍼 열기
		HRESULT hr = cCoreStorage::GetInstance()->md3dImmediateContext->Map(mSkinTex.mTexture, //매핑할 텍스처
			D3D11CalcSubresource(0, 0, 1),													   //서브 리소스 번호
			D3D11_MAP_WRITE_DISCARD,														   //리소스에 쓴다
			0,
			&MappedResource);																   //데이터를 쓸 포인터

		// 열기 성공했을때만
		if (hr == S_OK)
		{
			// 데이터 맵핑 레퍼런스 얻기
			float* pTexels = (float*)MappedResource.pData;
			
			// 열 반복 (애니 키)
			for (int y = 0; y < _AniSize; ++y) 
			{
				// 행 쓰기 ( 4칸씩 )
				for (UINT x = 0; x < mSaveBoneData.size(); ++x)
				{
					for (int i = 0; i < 4; ++i)
					{
						// 픽셀 선택
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
							cout << "행렬 범위 초과" << endl;
							break;
						}
					}
				}
			}

			// 버퍼 닫기
			cCoreStorage::GetInstance()->md3dImmediateContext->Unmap(mSkinTex.mTexture, D3D11CalcSubresource(0, 0, 1));
		}
		else
			cout << "스킨 텍스처 맵핑 실패" << endl;
	}


	// 텍스처 저장
	void SaveTex(wstring& _TexName)
	{
		D3DX11SaveTextureToFile(cCoreStorage::GetInstance()->md3dImmediateContext,
			mSkinTex.mTexture,      //저장할 텍스처
			D3DX11_IFF_DDS,         //dds로 저장
			_TexName.c_str());      //이름

		// 텍스처를 만들었으니, 기존 데이터는 삭제
		mSkinTex.ClearTex();
	}

	// 트리 만들기
	void MakeTree()
	{
		// 각 본마다 순회
		for (unsigned int y = 0; y < mBoneHierarchy.size(); ++y)
		{
			// 키를 추가한다.
			mSkinTree.AddKey(mBoneData, mBoneHierarchy[y], mBoneHierarchy[y].size(), 0);
		}
	}

	// 계산용 변수 비우기
	void ClearCalValue()
	{
		// Skin 비우기
		for (unsigned int i = 0; i < mSkinMtx.size(); ++i)
			mSkinMtx[i].clear();
		mSkinMtx.clear();

		// Skin 비우기2
		for (unsigned int i = 0; i < mRelocSkinMtx.size(); ++i)
			mRelocSkinMtx[i].clear();
		mRelocSkinMtx.clear();

		// LAP 비우기
		for (unsigned int i = 0; i < mLAP.size(); ++i)
			mLAP[i].clear();
		mLAP.clear();

		// LAP 비우기
		for (unsigned int i = 0; i < mRelocLAP.size(); ++i)
			mRelocLAP[i].clear();
		mRelocLAP.clear();
	}

	// 계산용 변수 공간확보
	void ResizeCalValue()
	{
		// 모든 키는 동일한 갯수를 가진다.
		int _size = mSaveBoneData[0].mAniData.Position.size();

		// 스킨 & LAP 매트릭스
		mRelocSkinMtx.resize(_size);
		mSkinMtx     .resize(_size);
		mLAP         .resize(_size);
		mRelocLAP    .resize(_size);
	}

	// 변환 함수
	void StringToWchar_t(string& _string, wstring& _wstring)
	{
		// 변환
		for (unsigned int i = 0; i < _string.length(); ++i)
			_wstring += wchar_t(_string[i]);

		// 마무리
		_wstring += wchar_t('\0');
	}
};
