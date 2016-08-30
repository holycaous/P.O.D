#pragma once
#include "UtilD3D.h"

#define BUF_SIZE 1024

// 버텍스 구조
struct VertexPNT
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT2 Tex;
	XMFLOAT3 TangentU;
	XMFLOAT3 BiNormal;
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

// 버퍼
struct CharBuf
{
	char Data[BUF_SIZE];
};

// 바운딩 박스
class BoundBox
{
public:
	XMFLOAT3 Max;
	XMFLOAT3 Min;
	XMFLOAT3 Center;
};

// 식별코드를 가진 버텍스 리스트
class KeyVtx
{
public:
	int Key;      // 키값
	XMFLOAT3 Vtx; // 버텍스
};

// 재질 인덱스 리스트
class MatIdx
{
public:
	vector<XMFLOAT3> Idx;  // 인덱스 리스트
	vector<XMFLOAT3> tIdx; // 텍스처 인덱스 리스트

public:
	~MatIdx()
	{
		Idx.clear();
		tIdx.clear();
	}
};

// 텍스처 정보
class TextuerInfo
{
public:
	// 재질 키 값
	int Key;

	// 리소스 뷰 ( 위치 )
	char mDiffuseMap [BUF_SIZE];
	char mSpecularMap[BUF_SIZE];
	char mNoamleMap  [BUF_SIZE];

	// 재질 인덱스
	MatIdx mMatIdx;

	// 투명 (디퓨즈 맵)
	float mOpacity;

public:
	TextuerInfo()
	{
		Key      = 0;
		mOpacity = 100.0f;
		strcpy(mDiffuseMap,  "NULL");
		strcpy(mSpecularMap, "NULL");
		strcpy(mNoamleMap,   "NULL");   
	}
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
		strcpy(Name, "NULL");
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

class MyMat
{
public:
	int key; // 재질번호
	
	// 리소스 뷰 ( 위치 )
	char mDiffuseMap [BUF_SIZE];
	char mSpecularMap[BUF_SIZE];
	char mNoamleMap  [BUF_SIZE];

	// 투명 (디퓨즈 맵)
	float mOpacity;
public:
	MyMat()
	{
		strcpy(mDiffuseMap,  "NULL");
		strcpy(mSpecularMap, "NULL");
		strcpy(mNoamleMap,   "NULL");
		mOpacity     = 100.0f;
	}
};

// 키를 가진 문자열
class KeyString
{
public:
	int  Key;
	char Name[BUF_SIZE];
	char objClass[BUF_SIZE];
};

// 파싱한 매쉬 데이터
class InitMetaData
{
public:
	// 모든 정보
	vector<Vertex>   AllVertices;

	// 임시 버퍼
	vector<XMFLOAT3> Vertices;
	vector<KeyVtx>   Indices;
	vector<KeyVtx>   TexIndices;
	vector<XMFLOAT2> TexVertices;
	vector<XMFLOAT3> NVertices;

	// 모델 대표 이름
	char mMainName[BUF_SIZE];

	// 오브젝트이름
	int  mObjID;
	char mObjName[BUF_SIZE];
	char mObjClass[BUF_SIZE];
	
	// 상위 클래스 이름
	int  mParentID;
	char mParentName[BUF_SIZE];

	// 변환(TM) 매트릭스
	XMFLOAT4X4    mTMLocalMtx;
	XMFLOAT4X4    mTMWorldMtx;

	// 바운딩 박스
	BoundBox mBoundingBox;

	// 애니 데이터
	AniData mAniData;

	// 가중치 데이터
	vector<WeightVtx> mWeightVtx;

	// 사용된 재질 데이터
	vector<int> mMatNum;
public:
	
	InitMetaData()
	{
		// 매트릭스 초기화
		XMMATRIX I = XMMatrixIdentity();

		// TM 매트릭스 초기화
		XMStoreFloat4x4(&mTMLocalMtx, I);
		XMStoreFloat4x4(&mTMWorldMtx, I);

		// 변수 초기화
		mObjID    = -1;
		mParentID = -1;
		strcpy(mObjName   , "NULL");
		strcpy(mObjClass  , "NULL");
		strcpy(mParentName, "NULL");
	}

	~InitMetaData()
	{
		AllVertices .clear();
		Vertices    .clear();
		Indices     .clear();
		TexIndices  .clear();
		TexVertices .clear();
		mWeightVtx  .clear();
		NVertices   .clear();
		mMatNum     .clear();
	}

	void AddMat(int _num)
	{
		for (unsigned int i = 0; i < mMatNum.size(); ++i)
		{
			if (mMatNum[i] == _num)
				return;
		}
		mMatNum.push_back(_num);
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
		vertices .clear();
		indices  .clear();
		weightVtx.clear();
	}

	// 오브젝트 초기화
	void init(InitMetaData& _InitMetaData, TextuerInfo _textuerInfo)
	{
		// 애니 데이터 복사
		aniData = _InitMetaData.mAniData;

		// 가중치 데이터 복사
		weightVtx = _InitMetaData.mWeightVtx;

		// 버텍스 버퍼 복사
		vertices = _InitMetaData.AllVertices;

		// TM 행렬
		mTMLocalMtx  = _InitMetaData.mTMLocalMtx;
		mTMWorldMtx  = _InitMetaData.mTMWorldMtx;

		// 바운딩 박스
		mBoundingBox = _InitMetaData.mBoundingBox;

		// 모델 대표이름
		strcpy(mMainName, _InitMetaData.mMainName);

		// 오브젝트이름
		mObjID = _InitMetaData.mObjID;
		strcpy(mObjName , _InitMetaData.mObjName);
		strcpy(mObjClass, _InitMetaData.mObjClass);

		// 상위 클래스 이름
		mParentID = _InitMetaData.mParentID;
		strcpy(mParentName, _InitMetaData.mParentName);


		// 재질 클래스에 있는 인덱스 사이즈 만큼, 공간 확보
		indices.resize(_textuerInfo.mMatIdx.Idx.size());

		// 재질 클래스의 인덱스만큼 순회
		for (unsigned int i = 0; i < _textuerInfo.mMatIdx.Idx.size(); ++i)
			indices[i] = _textuerInfo.mMatIdx.Idx[i];

		// 재질 클래스 채워 넣기
		mMyMat.key      = _textuerInfo.Key;
		mMyMat.mOpacity = _textuerInfo.mOpacity;

		strcpy(mMyMat.mDiffuseMap , _textuerInfo.mDiffuseMap);
		strcpy(mMyMat.mSpecularMap, _textuerInfo.mSpecularMap);
		strcpy(mMyMat.mNoamleMap  , _textuerInfo.mNoamleMap);

		// 변수 계산하기
		vertexOffset = vertices.size();
		indexOffset  = indices.size() * 3;
		indexCount   = indices.size() * 3;
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

public:
	// 더미 초기화
	void init(InitMetaData& _InitMetaData)
	{
		// 애니 데이터 복사
		aniData = _InitMetaData.mAniData;

		// TM 행렬
		mTMLocalMtx = _InitMetaData.mTMLocalMtx;
		mTMWorldMtx = _InitMetaData.mTMWorldMtx;

		// 바운딩 박스
		mBoundingBox = _InitMetaData.mBoundingBox;

		// 오브젝트이름
		mObjID = _InitMetaData.mObjID;
		strcpy(mObjName, _InitMetaData.mObjName);
		strcpy(mObjClass, _InitMetaData.mObjClass);

		// 상위 클래스 이름
		mParentID = _InitMetaData.mParentID;
		strcpy(mParentName, _InitMetaData.mParentName);
	}
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
	char mAniName[BUF_SIZE];
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

	// 인덱스 재배치
	void SetRelocationIdx()
	{
		// 부모 이름은 안바뀌고, 인덱스만 꼬인걸 역이용.
		// 인덱스를 정상적으로 재배치하고, 
		// n제곱만큼 수행함으로써 부모노드의 id 값을 정상적으로 맞춰준다.

		// 인덱스 정상 배치
		for (unsigned int i = 0; i < mBoneData.size(); ++i)
		{
			// 순서대로 배치하면 맞음.
			mBoneData[i].mObjID = i;
		}

		// 부모 노드 재 배치
		// 전체 순회
		for (unsigned int i = 0; i < mBoneData.size(); ++i)
		{
			// 엔 제곱만큼 수행 
			for (unsigned int idx = 0; idx < mBoneData.size(); ++idx)
			{
				// 찾는 정보가 일치한다면
				if (strstr(mBoneData[i].mParentName, mBoneData[idx].mObjName))
				{
					// 덮어쓴다.
					mBoneData[i].mParentID = mBoneData[idx].mObjID;
					break;
				}
			}
		}
	}

	// 트리를 구성한다
	void SetTree()
	{
		// 트리를 구성하기전
		// 본 인덱스를 보정해준다 -0- 
		// 본 데이터에 오브젝트 아이디는 필요 없으므로, 
		// 본 아이디를 오브젝트 아이디로 대체한다.
		SetRelocationIdx();

		// 인덱스를 보정했으므로, 본격적으로 트리 생성.
		// 본 데이터 크기만큼 공간 할당
		mBoneHierarchy.resize(mBoneData.size());
		
		// 전체 본데이터를 반복한다.
		for (unsigned int i = 0; i < mBoneData.size(); ++i)
		{
			// 루트노드를 얻는다.
			GetRootNode(i, mBoneData[i]);
		}

		// 오름차순으로 소팅을 한다.
		for (unsigned int i = 0; i < mBoneHierarchy.size(); ++i)
		{
			sort(mBoneHierarchy[i].begin(), mBoneHierarchy[i].end(), 
			[](const KeyString &a, const KeyString &b)
			{
				return a.Key < b.Key;
			});
		}
	}

private:
	// 재귀함수
	NodeBone GetRootNode(int _idx, NodeBone _SelectNode)
	{
		// 부모노드가, 루트노드라면?
		if (_SelectNode.mParentID == -1)
		{
			KeyString tKeyString;
			tKeyString.Key  = _SelectNode.mObjID;
			strcpy(tKeyString.Name, _SelectNode.mObjName);
			strcpy(tKeyString.objClass, _SelectNode.mObjClass);
			mBoneHierarchy[_idx].push_back(tKeyString);
			return _SelectNode;
		}
		else
		{
			KeyString tKeyString;
			tKeyString.Key = _SelectNode.mObjID;
			strcpy(tKeyString.Name, _SelectNode.mObjName);
			strcpy(tKeyString.objClass, _SelectNode.mObjClass);
			mBoneHierarchy[_idx].push_back(tKeyString);

			// 부모 노드의 값을 얻어 재귀호출 한다.
			return GetRootNode(_idx ,mBoneData[_SelectNode.mParentID]);
		}
	}
};