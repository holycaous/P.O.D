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
	e_ShaderColor      = 0,
	e_ShaderLight      = 1,
	e_ShaderPongTex    = 2,
	e_ShaderCartoonTex = 3,
	e_ShaderDeferred   = 4
}SHADER_TYPE;

// 쉐이더 변수 초기화
typedef enum
{
	e_ShaderValMtx      = 0,
	e_ShaderValVtx      = 1,
	e_ShaderVal         = 2,
	e_ShaderValResource = 3
}SHADER_VAL_TYPE;

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
	char mDiffuseSRV[BUF_SIZE];
	char mSpecularSRV[BUF_SIZE];
	char mNomalSRV[BUF_SIZE];

	// 투명 (디퓨즈 맵)
	float mOpacity;
public:
	MyMat()
	{
		strcpy(mDiffuseSRV , "NULL\0");
		strcpy(mSpecularSRV, "NULL\0");
		strcpy(mNomalSRV   , "NULL\0");
		mOpacity = 100.0f;
	}
	~MyMat()
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

// 본 데이터 (가중치)
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


// 버텍스 가중치 데이터
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

// 애니 데이터
class AniData
{
public:
	vector<KeyVtx> Position;
	vector<KeyVtx> Quaternion;
	vector<KeyVtx> Scale;
public:
	~AniData()
	{
		Position.clear();
		Quaternion.clear();
		Scale.clear();
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

class ObjData
{
public:
	int          mKey;
	XMFLOAT4X4   mWdMtx;
	OBJ_MOVEABLE mObjMoveAble;

	XMFLOAT3     mLookDir; // 룩벡터
	XMFLOAT3     mUpdir;   // 업벡터
public:
	ObjData()
	{
		// 키 
		mKey = -1;

		// 월드 매트릭스 초기화
		XMMATRIX I = XMMatrixIdentity();
		XMStoreFloat4x4(&mWdMtx, I);

		// 오브젝트가 동적인지 정적인지
		mObjMoveAble = e_StaticObj;

		// 룩벡터 & 업벡터
		mLookDir = XMFLOAT3(0.0f, 0.0f, 1.0f);
		mUpdir   = XMFLOAT3(0.0f, 1.0f, 0.0f);
	}
	~ObjData()
	{

	}
};


// 파싱한 매쉬 데이터
class InitMetaData
{
public:
	// 버텍스, 인덱스
	vector<Vertex>   Vertices;
	vector<UINT>     Indices;

	// 정점늘리기 용도(파싱 데이터 같은경우는 안씀)
	vector<UINT>     TexIndices;
	vector<XMFLOAT3> TexVertices;

	// 월드 매트릭스 (각 캐릭터 구분용) @@@@@@@@
	vector<ObjData> mObjData;

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
	int  mVertexOffset;
	UINT mIndexOffset;

	// 카운트
	UINT mIndexCount;

	// 애니 데이터
	AniData aniData;

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
	cCoreStorage* mCoreStorage = cCoreStorage::GetInstance();

public:
	InitMetaData(const char* _Name, SHADER_TYPE _ShaderMode, D3D_PRIMITIVE_TOPOLOGY _D3D_PRIMITIVE_TOPOLOGY)
	{
		// 리소스 뷰
		mDiffuseSRV = mSpecularSRV = mNomalSRV = nullptr;

		// 디퓨즈맵 투명도
		mOpacity = 100.0f;

		// 매트릭스 초기화
		XMMATRIX I = XMMatrixIdentity();

		// 텍스처 변환 매트릭스 초기화
		XMStoreFloat4x4(&mTexMtx, I);
		XMStoreFloat4x4(&mLocTMMtx, I);
		XMStoreFloat4x4(&mWdTMMtx, I);

		// 이름 등록
		strcpy(mMainName, _Name);

		// 쉐이더 모드
		mShaderMode = _ShaderMode;

		// 어떤식으로 모델을 구성할 것인가
		_PRIMITIVE_TOPOLOGY = _D3D_PRIMITIVE_TOPOLOGY;

		// 변수 초기화
		mObjID    = -1;
		mParentID = -1;
		strcpy(mObjName   , "NULL\0");
		strcpy(mObjClass  , "NULL\0");
		strcpy(mParentName, "NULL\0");

	}

	InitMetaData(SHADER_TYPE _ShaderMode, D3D_PRIMITIVE_TOPOLOGY _D3D_PRIMITIVE_TOPOLOGY)
	{
		// 리소스 뷰
		mDiffuseSRV = mSpecularSRV = mNomalSRV = nullptr;

		// 디퓨즈맵 투명도
		mOpacity = 100.0f;

		// 매트릭스 초기화
		XMMATRIX I = XMMatrixIdentity();

		// 텍스처 변환 매트릭스 초기화
		XMStoreFloat4x4(&mTexMtx, I);
		XMStoreFloat4x4(&mLocTMMtx, I);
		XMStoreFloat4x4(&mWdTMMtx, I);

		// 쉐이더 모드
		mShaderMode = _ShaderMode;

		// 어떤식으로 모델을 구성할 것인가
		_PRIMITIVE_TOPOLOGY = _D3D_PRIMITIVE_TOPOLOGY;

	}

	~InitMetaData()
	{
		Vertices   .clear();
		Indices    .clear();
		TexIndices .clear();
		TexVertices.clear();
		mObjData   .clear();
		mCreateName.clear();

		for (unsigned int i = 0; i < weightVtx.size(); ++i)
			weightVtx[i].Bone.clear();
		weightVtx.clear();

		ReleaseCOM(mDiffuseSRV);
		ReleaseCOM(mSpecularSRV);
		ReleaseCOM(mNomalSRV);
	}

	// 텍스처 로드
	void LoadTex(const wchar_t* _TexName, TEXTURE_TYPE e_InitTex)
	{
		switch (e_InitTex)
		{
		case e_DiffuseMap:
			HR(D3DX11CreateShaderResourceViewFromFile(mCoreStorage->md3dDevice, _TexName, 0, 0, &mDiffuseSRV, 0));
			break;
		case e_NomalMap:
			HR(D3DX11CreateShaderResourceViewFromFile(mCoreStorage->md3dDevice, _TexName, 0, 0, &mNomalSRV, 0));
			break;
		case e_SpecularMap:
			HR(D3DX11CreateShaderResourceViewFromFile(mCoreStorage->md3dDevice, _TexName, 0, 0, &mSpecularSRV, 0));
			break;
		}
	}

	// 파싱 데이터
	void ParsingData()
	{


	}

	// 모델 (월드 매트릭스) 추가하기
	void AddModel(ObjData _ObjData)
	{
		mObjData.push_back(_ObjData);
	}

	// 모델 (월드 매트릭스) 추가하기
	void AddModel(float _x, float _y, float _z, OBJ_MOVEABLE _moveAble)
	{
		// 오브젝트 초기화
		ObjData _ObjData;

		// 유니크 코드
		_ObjData.mKey = mObjData.size();

		// 좌표 저장
		_ObjData.mWdMtx._41 = _x;
		_ObjData.mWdMtx._42 = _y;
		_ObjData.mWdMtx._43 = _z;

		// 오브젝트 이동 가능한가
		_ObjData.mObjMoveAble = _moveAble;

		// 오브젝트 저장
		mObjData.push_back(_ObjData);
	}

	// 오브젝트 이동
	void SetPos(int _uniqueCode, float _x, float _y, float _z)
	{
		// 오브젝트 이동
		mObjData[_uniqueCode].mWdMtx._41 = _x;
		mObjData[_uniqueCode].mWdMtx._42 = _y;
		mObjData[_uniqueCode].mWdMtx._43 = _z;
	}


	// 오브젝트 삭제
	void ClearWdMtx()
	{
		mObjData.clear();
	}

	// 변수 계산하기
	void CalValue()
	{
		mVertexOffset = Vertices.size();
		mIndexOffset = Indices.size();
		mIndexCount = Indices.size();
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

// 이펙트 변수
class EffectStorage
{
public:
	ID3DX11Effect* mFX;			   // 이펙트
	ID3DX11EffectTechnique* mTech; // 테크닉
	map<string, ID3DX11EffectMatrixVariable*> mfxMtx; // 행렬
	map<string, ID3DX11EffectVectorVariable*> mfxVtx; // 벡터
	map<string, ID3DX11EffectVariable*>     mfxValue; // 변수

	map<string, ID3DX11EffectShaderResourceVariable*>  mfxResource; // 리소스

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

	// 해당 버퍼에 들어갈 모델 리스트 ( 계산용, 빌드 후 삭제하고 있음)
	map<string, InitMetaData*> mModelList;

public:
	BufferType()
	{
		// 버퍼
		mVB = mIB = nullptr;
	}

	BufferType(SHADER_TYPE _ShaderMode)
	{
		// 버퍼
		mVB = mIB = nullptr;

		// 쉐이더 모드 셋팅
		mShaderMode = _ShaderMode;
	}
	~BufferType()
	{
		// 인스턴스 버퍼 삭제
		ClearInsBuf();

		// 모델 리스트 삭제
		mModelList.clear();

		ReleaseCOM(mVB);
		ReleaseCOM(mIB);
	}

	void ClearInsBuf()
	{
		// 인스턴스 버퍼 삭제
		for (auto itor = mInstancedBuffer.begin(); itor != mInstancedBuffer.end(); ++itor)
			ReleaseCOM(itor->second);
		mInstancedBuffer.clear();
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

		default:
			break;
		}
	}

	// 스크린 버퍼 빌드
	void Build_GScreen(InitMetaData* _Screen)
	{
		//-----------------------------------------------------------------------------------------------------------//
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
			vertices[i].Pos = _Screen->Vertices[i].Position;
			vertices[i].Tex = _Screen->Vertices[i].TexUV;
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

	// 스크린 인스턴스 버퍼 생성
	void MakeScreenInsBuf(InitMetaData* _Screen)
	{
		// 인스턴스 버퍼 생성
		// 월드 매트릭스가 있어야만 생성
		if (_Screen->mObjData.size())
		{
			D3D11_BUFFER_DESC vbd;
			vbd.Usage               = D3D11_USAGE_DYNAMIC;
			vbd.ByteWidth           = sizeof(XMFLOAT4X4)* _Screen->mObjData.size();
			vbd.BindFlags           = D3D11_BIND_VERTEX_BUFFER; // 버텍스
			vbd.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
			vbd.MiscFlags           = 0;
			vbd.StructureByteStride = 0;

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
			for (UINT i = 0; i < _Screen->mObjData.size(); ++i)
				dataView[i] = _Screen->mObjData[i].mWdMtx;

			// 인스턴스 버퍼 닫기
			mCoreStorage->md3dImmediateContext->Unmap(mInstancedBuffer[_Screen->mCreateName], 0);
		}
	}

	// 모델 추가
	void AddModel(string _ModelName, InitMetaData* _ModelData)
	{
		mModelList[_ModelName] = _ModelData;
	}

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
				vbd.Usage               = D3D11_USAGE_DYNAMIC;
				vbd.ByteWidth           = sizeof(XMFLOAT4X4)* 100;
				vbd.BindFlags           = D3D11_BIND_VERTEX_BUFFER; // 버텍스
				vbd.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
				vbd.MiscFlags           = 0;
				vbd.StructureByteStride = 0;

				// 공간할당
				//mInstancedBuffer[itor->second->mCreateName] = NULL;
				HR(mCoreStorage->md3dDevice->CreateBuffer(&vbd, 0, &mInstancedBuffer[itor->second->mCreateName]));
			}
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
				mCoreStorage->md3dImmediateContext->Map(mInstancedBuffer[itor->second->mCreateName], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);

				// << 인스턴스 버퍼 >> 의 "인터페이스"를 얻어온다.
				XMFLOAT4X4* dataView = reinterpret_cast<XMFLOAT4X4*>(mappedData.pData);

				// 컬링 안함
				// 매트릭스 수만큼 등록
				for (UINT i = 0; i < itor->second->mObjData.size(); ++i)
					dataView[i] = itor->second->mObjData[i].mWdMtx;

				// 인스턴스 버퍼 닫기
				mCoreStorage->md3dImmediateContext->Unmap(mInstancedBuffer[itor->second->mCreateName], 0);
			}
		}
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
				vertices[k].Pos = itor->second->Vertices[i].Position;
				vertices[k].Color = itor->second->Vertices[i].Color;
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
				vertices[k].Pos = itor->second->Vertices[i].Position;
				vertices[k].Normal = itor->second->Vertices[i].Normal;
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

			// 탄젠트 공간 계싼
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
				vertices[k].Pos = itor->second->Vertices[i].Position;
				vertices[k].Tex = itor->second->Vertices[i].TexUV;
				vertices[k].Normal = itor->second->Vertices[i].Normal;
				vertices[k].Tangent = itor->second->Vertices[i].TangentU;
				vertices[k].BiNormal = itor->second->Vertices[i].BiNormal;
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
		ibd.ByteWidth      = sizeof(UINT)* totalIndexCount;
		ibd.BindFlags      = D3D11_BIND_INDEX_BUFFER; // 인덱스
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags      = 0;

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
			UINT _TexIdx = _MetaData->TexIndices[i]; // 텍스처의 현재 인덱스

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
	AniData aniData;

	// 가중치 데이터
	vector<WeightVtx> weightVtx;

	// 오프셋
	int  vertexOffset;
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
	~MyMeshData()
	{
		vertices.clear();
		indices.clear();
		weightVtx.clear();
	}
};


// 본 데이터
class NodeBone
{
public:
	// TM 행렬
	XMFLOAT4X4 mTMLocalMtx;
	XMFLOAT4X4 mTMWorldMtx;

	// 애니 데이터
	AniData aniData;

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
};

// 본들의 정보를 담아둠
class MyBoneData
{
public:
	// 계층 구조를 담은 본 정보
	vector<NodeBone> mBoneData;

	// 계층 구조를 실질적으로 찾아가는 정보
	// 일종의 해쉬맵
	vector<vector<KeyString> > mBoneHierarchy;

	// 이름
	char mMainName[BUF_SIZE];

public:
	~MyBoneData()
	{
		// 삭제
		ClearClass();
	}

	void ClearClass()
	{
		// 본 데이터 삭제
		mBoneData.clear();

		// 하이라이키 삭제
		for (unsigned int i = 0; i < mBoneHierarchy.size(); ++i)
			mBoneHierarchy[i].clear();
		mBoneHierarchy.clear();
	}
};