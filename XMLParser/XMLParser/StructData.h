#pragma once
#include "UtilD3D.h"

#define BUF_SIZE 1024

// ���ؽ� ����
struct VertexPNT
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT2 Tex;
	XMFLOAT3 TangentU;
	XMFLOAT3 BiNormal;
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

// ����
struct CharBuf
{
	char Data[BUF_SIZE];
};

// �ٿ�� �ڽ�
class BoundBox
{
public:
	XMFLOAT3 Max;
	XMFLOAT3 Min;
	XMFLOAT3 Center;
};

// �ĺ��ڵ带 ���� ���ؽ� ����Ʈ
class KeyVtx
{
public:
	int Key;      // Ű��
	XMFLOAT3 Vtx; // ���ؽ�
};

// ���� �ε��� ����Ʈ
class MatIdx
{
public:
	vector<XMFLOAT3> Idx;  // �ε��� ����Ʈ
	vector<XMFLOAT3> tIdx; // �ؽ�ó �ε��� ����Ʈ

public:
	~MatIdx()
	{
		Idx.clear();
		tIdx.clear();
	}
};

// �ؽ�ó ����
class TextuerInfo
{
public:
	// ���� Ű ��
	int Key;

	// ���ҽ� �� ( ��ġ )
	char mDiffuseMap [BUF_SIZE];
	char mSpecularMap[BUF_SIZE];
	char mNoamleMap  [BUF_SIZE];

	// ���� �ε���
	MatIdx mMatIdx;

	// ���� (��ǻ�� ��)
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
		strcpy(Name, "NULL");
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
		Position.clear();
		Quaternion.clear();
		Scale.clear();
	}
};

class MyMat
{
public:
	int key; // ������ȣ
	
	// ���ҽ� �� ( ��ġ )
	char mDiffuseMap [BUF_SIZE];
	char mSpecularMap[BUF_SIZE];
	char mNoamleMap  [BUF_SIZE];

	// ���� (��ǻ�� ��)
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

// Ű�� ���� ���ڿ�
class KeyString
{
public:
	int  Key;
	char Name[BUF_SIZE];
	char objClass[BUF_SIZE];
};

// �Ľ��� �Ž� ������
class InitMetaData
{
public:
	// ��� ����
	vector<Vertex>   AllVertices;

	// �ӽ� ����
	vector<XMFLOAT3> Vertices;
	vector<KeyVtx>   Indices;
	vector<KeyVtx>   TexIndices;
	vector<XMFLOAT2> TexVertices;
	vector<XMFLOAT3> NVertices;

	// �� ��ǥ �̸�
	char mMainName[BUF_SIZE];

	// ������Ʈ�̸�
	int  mObjID;
	char mObjName[BUF_SIZE];
	char mObjClass[BUF_SIZE];
	
	// ���� Ŭ���� �̸�
	int  mParentID;
	char mParentName[BUF_SIZE];

	// ��ȯ(TM) ��Ʈ����
	XMFLOAT4X4    mTMLocalMtx;
	XMFLOAT4X4    mTMWorldMtx;

	// �ٿ�� �ڽ�
	BoundBox mBoundingBox;

	// �ִ� ������
	AniData mAniData;

	// ����ġ ������
	vector<WeightVtx> mWeightVtx;

	// ���� ���� ������
	vector<int> mMatNum;
public:
	
	InitMetaData()
	{
		// ��Ʈ���� �ʱ�ȭ
		XMMATRIX I = XMMatrixIdentity();

		// TM ��Ʈ���� �ʱ�ȭ
		XMStoreFloat4x4(&mTMLocalMtx, I);
		XMStoreFloat4x4(&mTMWorldMtx, I);

		// ���� �ʱ�ȭ
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
	int  vertexOffset;
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
	~MyMeshData()
	{
		vertices .clear();
		indices  .clear();
		weightVtx.clear();
	}

	// ������Ʈ �ʱ�ȭ
	void init(InitMetaData& _InitMetaData, TextuerInfo _textuerInfo)
	{
		// �ִ� ������ ����
		aniData = _InitMetaData.mAniData;

		// ����ġ ������ ����
		weightVtx = _InitMetaData.mWeightVtx;

		// ���ؽ� ���� ����
		vertices = _InitMetaData.AllVertices;

		// TM ���
		mTMLocalMtx  = _InitMetaData.mTMLocalMtx;
		mTMWorldMtx  = _InitMetaData.mTMWorldMtx;

		// �ٿ�� �ڽ�
		mBoundingBox = _InitMetaData.mBoundingBox;

		// �� ��ǥ�̸�
		strcpy(mMainName, _InitMetaData.mMainName);

		// ������Ʈ�̸�
		mObjID = _InitMetaData.mObjID;
		strcpy(mObjName , _InitMetaData.mObjName);
		strcpy(mObjClass, _InitMetaData.mObjClass);

		// ���� Ŭ���� �̸�
		mParentID = _InitMetaData.mParentID;
		strcpy(mParentName, _InitMetaData.mParentName);


		// ���� Ŭ������ �ִ� �ε��� ������ ��ŭ, ���� Ȯ��
		indices.resize(_textuerInfo.mMatIdx.Idx.size());

		// ���� Ŭ������ �ε�����ŭ ��ȸ
		for (unsigned int i = 0; i < _textuerInfo.mMatIdx.Idx.size(); ++i)
			indices[i] = _textuerInfo.mMatIdx.Idx[i];

		// ���� Ŭ���� ä�� �ֱ�
		mMyMat.key      = _textuerInfo.Key;
		mMyMat.mOpacity = _textuerInfo.mOpacity;

		strcpy(mMyMat.mDiffuseMap , _textuerInfo.mDiffuseMap);
		strcpy(mMyMat.mSpecularMap, _textuerInfo.mSpecularMap);
		strcpy(mMyMat.mNoamleMap  , _textuerInfo.mNoamleMap);

		// ���� ����ϱ�
		vertexOffset = vertices.size();
		indexOffset  = indices.size() * 3;
		indexCount   = indices.size() * 3;
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
	// ���� �ʱ�ȭ
	void init(InitMetaData& _InitMetaData)
	{
		// �ִ� ������ ����
		aniData = _InitMetaData.mAniData;

		// TM ���
		mTMLocalMtx = _InitMetaData.mTMLocalMtx;
		mTMWorldMtx = _InitMetaData.mTMWorldMtx;

		// �ٿ�� �ڽ�
		mBoundingBox = _InitMetaData.mBoundingBox;

		// ������Ʈ�̸�
		mObjID = _InitMetaData.mObjID;
		strcpy(mObjName, _InitMetaData.mObjName);
		strcpy(mObjClass, _InitMetaData.mObjClass);

		// ���� Ŭ���� �̸�
		mParentID = _InitMetaData.mParentID;
		strcpy(mParentName, _InitMetaData.mParentName);
	}
};

// ������ ������ ��Ƶ�
class MyBoneData
{
public:
	// ���� ������ ���� �� ����
	vector<NodeBone> mBoneData;

	// ���� ������ ���������� ã�ư��� ����
	// ������ �ؽ���
	vector<vector<KeyString> > mBoneHierarchy;

	// �̸�
	char mMainName[BUF_SIZE];
	char mAniName[BUF_SIZE];
public:
	~MyBoneData()
	{
		// ����
		ClearClass();
	}

	void ClearClass()
	{
		// �� ������ ����
		mBoneData.clear();

		// ���̶���Ű ����
		for (unsigned int i = 0; i < mBoneHierarchy.size(); ++i)
			mBoneHierarchy[i].clear();
		mBoneHierarchy.clear();
	}

	// �ε��� ���ġ
	void SetRelocationIdx()
	{
		// �θ� �̸��� �ȹٲ��, �ε����� ���ΰ� ���̿�.
		// �ε����� ���������� ���ġ�ϰ�, 
		// n������ŭ ���������ν� �θ����� id ���� ���������� �����ش�.

		// �ε��� ���� ��ġ
		for (unsigned int i = 0; i < mBoneData.size(); ++i)
		{
			// ������� ��ġ�ϸ� ����.
			mBoneData[i].mObjID = i;
		}

		// �θ� ��� �� ��ġ
		// ��ü ��ȸ
		for (unsigned int i = 0; i < mBoneData.size(); ++i)
		{
			// �� ������ŭ ���� 
			for (unsigned int idx = 0; idx < mBoneData.size(); ++idx)
			{
				// ã�� ������ ��ġ�Ѵٸ�
				if (strstr(mBoneData[i].mParentName, mBoneData[idx].mObjName))
				{
					// �����.
					mBoneData[i].mParentID = mBoneData[idx].mObjID;
					break;
				}
			}
		}
	}

	// Ʈ���� �����Ѵ�
	void SetTree()
	{
		// Ʈ���� �����ϱ���
		// �� �ε����� �������ش� -0- 
		// �� �����Ϳ� ������Ʈ ���̵�� �ʿ� �����Ƿ�, 
		// �� ���̵� ������Ʈ ���̵�� ��ü�Ѵ�.
		SetRelocationIdx();

		// �ε����� ���������Ƿ�, ���������� Ʈ�� ����.
		// �� ������ ũ�⸸ŭ ���� �Ҵ�
		mBoneHierarchy.resize(mBoneData.size());
		
		// ��ü �������͸� �ݺ��Ѵ�.
		for (unsigned int i = 0; i < mBoneData.size(); ++i)
		{
			// ��Ʈ��带 ��´�.
			GetRootNode(i, mBoneData[i]);
		}

		// ������������ ������ �Ѵ�.
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
	// ����Լ�
	NodeBone GetRootNode(int _idx, NodeBone _SelectNode)
	{
		// �θ��尡, ��Ʈ�����?
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

			// �θ� ����� ���� ��� ���ȣ�� �Ѵ�.
			return GetRootNode(_idx ,mBoneData[_SelectNode.mParentID]);
		}
	}
};