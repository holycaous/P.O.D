#include "stdafx.h"
#include "cCam.h"


cCam::cCam()
{
	// 카메라 초기 위치
	mPosition.x = 0.0f;
	mPosition.y = 0.0f;
	mPosition.z = 0.0f;

	// 각 방향벡터 초기화
	initDir();

	// 3인칭 모드 기본 카메라 거리 설정
	m3PersonLength = CAM_3PERSON_LENGTH;

	// 카메라 거리 디폴트
	mNearZ = CAM_NEAR;
	mFarZ  = CAM_FAR;
}

cCam::~cCam()
{
}

// 카메라 초기화
void cCam::initCam(float _x, float _y, float _z)
{
	// 위치
	if (mCamMode == e_3Person)
		SetPosition(_x, _y, _z - m3PersonLength);
	else
		SetPosition(_x, _y, _z);

	// 방향
	initDir();
	
	// UI 매트릭스 초기화
	initUIViewMtx();

	// 회전
	Pitch(0.71f);
	RotateY(1.5f);

	// 업데이트
	UpdateViewMatrix();
}

void cCam::initUIViewMtx()
{
	// UI View 초기화
	mUIView(0, 0) = mRight.x;
	mUIView(1, 0) = mRight.y;
	mUIView(2, 0) = mRight.z;
	mUIView(3, 0) = 0.0f;

	mUIView(0, 1) = mUp.x;
	mUIView(1, 1) = mUp.y;
	mUIView(2, 1) = mUp.z;
	mUIView(3, 1) = CAM_ORTHO_Y;

	mUIView(0, 2) = mLook.x;
	mUIView(1, 2) = mLook.y;
	mUIView(2, 2) = mLook.z;
	mUIView(3, 2) = 0.0f;

	mUIView(0, 3) = 0.0f;
	mUIView(1, 3) = 0.0f;
	mUIView(2, 3) = 0.0f;
	mUIView(3, 3) = 1.0f;
}

void cCam::initDir()
{
	// 각 방향벡터 초기화
	mRight.x = 1.0f;
	mRight.y = 0.0f;
	mRight.z = 0.0f;

	mUp.x = 0.0f;
	mUp.y = 1.0f;
	mUp.z = 0.0f;

	mLook.x = 0.0f;
	mLook.y = 0.0f;
	mLook.z = 1.0f;
}


void cCam::FrustumProjection()
{
	ComputeFrustumFromProjection(&mCamFrustum, &Proj());
}

// 카메라 조절
void cCam::SetLens(float fovY, float aspect, float zn, float zf)
{
	// cache properties
	mFovY = fovY;
	mAspect = aspect;
	mNearZ = zn;
	mFarZ = zf;

	mNearWindowHeight = 2.0f * mNearZ * tanf(0.5f * mFovY);
	mFarWindowHeight = 2.0f * mFarZ  * tanf(0.5f * mFovY);

	// 원근 투영 Proj
	XMMATRIX P = XMMatrixIdentity();
	P = XMMatrixPerspectiveFovLH(mFovY, mAspect, mNearZ, mFarZ);
	XMStoreFloat4x4(&mProj, P);

	// 직교투영 Proj
	P = XMMatrixOrthographicLH(mFovY, mAspect, mNearZ, mFarZ);
	XMStoreFloat4x4(&mUIProj, P);

}

// 카메라 조절
void cCam::SetLens(float fovY, float aspect)
{
	// cache properties
	mFovY = fovY;
	mAspect = aspect;

	mNearWindowHeight = 2.0f * mNearZ * tanf(0.5f * mFovY);
	mFarWindowHeight = 2.0f * mFarZ  * tanf(0.5f * mFovY);

	// 원근 투영 Proj
	XMMATRIX P = XMMatrixIdentity();
	P = XMMatrixPerspectiveFovLH(mFovY, mAspect, mNearZ, mFarZ);
	XMStoreFloat4x4(&mProj, P);

	// 직교투영 Proj
	P = XMMatrixOrthographicLH(mFovY, mAspect, mNearZ, mFarZ);
	XMStoreFloat4x4(&mUIProj, P);
}


void cCam::LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp)
{
	XMVECTOR L = XMVector3Normalize(XMVectorSubtract(target, pos));
	XMVECTOR R = XMVector3Normalize(XMVector3Cross(worldUp, L));
	XMVECTOR U = XMVector3Cross(L, R);

	XMStoreFloat3(&mPosition, pos);
	XMStoreFloat3(&mLook, L);
	XMStoreFloat3(&mRight, R);
	XMStoreFloat3(&mUp, U);
}

void cCam::LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up)
{
	XMVECTOR P = XMLoadFloat3(&pos);
	XMVECTOR T = XMLoadFloat3(&target);
	XMVECTOR U = XMLoadFloat3(&up);

	LookAt(P, T, U);
}

void cCam::Strafe(float d)
{
	// mPosition += d * mRight
	XMVECTOR s = XMVectorReplicate(d); 
	XMVECTOR r = XMLoadFloat3(&mRight);
	XMVECTOR p = XMLoadFloat3(&mPosition);
	XMStoreFloat3(&mPosition, XMVectorMultiplyAdd(s, r, p));
}

void cCam::Walk(float d)
{
	// mPosition += d * mLook
	XMVECTOR s = XMVectorReplicate(d);

	// 3인칭 시점일때는 y축을 고정시킨다.
	// 카메라의 높낮이는 캐릭터의 위치로 조정한다.
	XMVECTOR l = { 0.0f, 0.0f, 0.0f, 0.0f };
	// 3인칭 모드
	if (mCamMode == e_3Person)
	{
		m3PersonLook = mLook;

		XMVECTOR tRight = XMLoadFloat3(&mRight);
		XMVECTOR tUp = XMLoadFloat3(&XMFLOAT3(0.0f, 1.0f, 0.0f));

		l = XMVector3Cross(tRight, tUp);
	}
	// 1인칭 모드
	else if (mCamMode == e_1Person)
	{
		l = XMLoadFloat3(&mLook);
	}
	XMVECTOR p = XMLoadFloat3(&mPosition);
	XMStoreFloat3(&mPosition, XMVectorMultiplyAdd(s, l, p));
}


void cCam::Pitch(float angle)
{
	// Rotate up and look vector about the right vector.
	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&mRight), angle);


	// 3인칭 모드
	if (mCamMode == e_3Person)
	{
		//--------------------------------------------------------//
		// 이동
		mPosition.x = (mPosition.x + mLook.x * m3PersonLength);
		mPosition.y = (mPosition.y + mLook.y * m3PersonLength);
		mPosition.z = (mPosition.z + mLook.z * m3PersonLength);

		//--------------------------------------------------------//
		// 회전
		XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), R));

		//--------------------------------------------------------//
		// 이동
		mPosition.x = (mPosition.x + mLook.x * -m3PersonLength);
		mPosition.y = (mPosition.y + mLook.y * -m3PersonLength);
		mPosition.z = (mPosition.z + mLook.z * -m3PersonLength);
		//--------------------------------------------------------//

		XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
	}
	else if (mCamMode == e_1Person)
	{
		// 회전
		XMStoreFloat3(&mUp  , XMVector3TransformNormal(XMLoadFloat3(&mUp)  , R));
		XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), R));
	}

}

void cCam::RotateY(float angle)
{
	// Rotate the basis vectors about the world y-axis.

	XMMATRIX R = XMMatrixRotationY(angle);

	// 3인칭 모드
	if (mCamMode == e_3Person)
	{
		//--------------------------------------------------------//
		// 이동
		mPosition.x = (mPosition.x + mLook.x * m3PersonLength);
		mPosition.y = (mPosition.y + mLook.y * m3PersonLength);
		mPosition.z = (mPosition.z + mLook.z * m3PersonLength);

		//--------------------------------------------------------//
		// 회전
		XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), R));

		//--------------------------------------------------------//
		// 이동
		mPosition.x = (mPosition.x + mLook.x * -m3PersonLength);
		mPosition.y = (mPosition.y + mLook.y * -m3PersonLength);
		mPosition.z = (mPosition.z + mLook.z * -m3PersonLength);
		//--------------------------------------------------------//

		XMStoreFloat3(&mUp   , XMVector3TransformNormal(XMLoadFloat3(&mUp)   , R));
		XMStoreFloat3(&mRight, XMVector3TransformNormal(XMLoadFloat3(&mRight), R));
	}
	else if (mCamMode == e_1Person)
	{
		// 회전
		XMStoreFloat3(&mLook , XMVector3TransformNormal(XMLoadFloat3(&mLook) , R));
		XMStoreFloat3(&mUp   , XMVector3TransformNormal(XMLoadFloat3(&mUp)   , R));
		XMStoreFloat3(&mRight, XMVector3TransformNormal(XMLoadFloat3(&mRight), R));
	}
}

void cCam::UpdateViewMatrix()
{
	XMVECTOR R = XMLoadFloat3(&mRight);
	XMVECTOR U = XMLoadFloat3(&mUp);
	XMVECTOR L = XMLoadFloat3(&mLook);
	XMVECTOR P = XMLoadFloat3(&mPosition);

	// Keep camera's axes orthogonal to each other and of unit length.
	L = XMVector3Normalize(L);
	U = XMVector3Normalize(XMVector3Cross(L, R));

	// U, L already ortho-normal, so no need to normalize cross product.
	R = XMVector3Cross(U, L);

	// Fill in the view matrix entries.
	float x = -XMVectorGetX(XMVector3Dot(P, R));
	float y = -XMVectorGetX(XMVector3Dot(P, U));
	float z = -XMVectorGetX(XMVector3Dot(P, L));

	XMStoreFloat3(&mRight, R);
	XMStoreFloat3(&mUp, U);
	XMStoreFloat3(&mLook, L);

	mView(0, 0) = mRight.x;
	mView(1, 0) = mRight.y;
	mView(2, 0) = mRight.z;
	mView(3, 0) = x;

	mView(0, 1) = mUp.x;
	mView(1, 1) = mUp.y;
	mView(2, 1) = mUp.z;
	mView(3, 1) = y;

	mView(0, 2) = mLook.x;
	mView(1, 2) = mLook.y;
	mView(2, 2) = mLook.z;
	mView(3, 2) = z;

	mView(0, 3) = 0.0f;
	mView(1, 3) = 0.0f;
	mView(2, 3) = 0.0f;
	mView(3, 3) = 1.0f;
}

// 캠 전환
void cCam::Change1PersonCam()
{
	mCamMode = e_1Person;
}
void cCam::Change3PersonCam()
{
	mCamMode = e_3Person;
}


// Get
XMVECTOR cCam::GetPositionXM()const
{
	return XMLoadFloat3(&mPosition);
}

XMFLOAT3 cCam::GetPosition()const
{
	return mPosition;
}

XMFLOAT3 cCam::GetThirdPosition() const
{
	XMFLOAT3 tPos;
	tPos.x = mPosition.x + mLook.x * m3PersonLength;
	tPos.y = mPosition.y + mLook.y * m3PersonLength;
	tPos.z = mPosition.z + mLook.z * m3PersonLength;
	return tPos;
}

void cCam::SetCamY(float _y)
{
	mPosition.y = _y - mLook.y * m3PersonLength;
}

void cCam::SetPosition(float x, float y, float z)
{
	mPosition = XMFLOAT3(x, y, z);
}

void cCam::SetPositionXZ(float x, float z)
{
	mPosition.x = x - mLook.x * m3PersonLength;
	mPosition.z = z - mLook.z * m3PersonLength;
}

void cCam::SetPosition(const XMFLOAT3& v)
{
	mPosition = v;
}

void cCam::SetPositionX(float x)
{
	mPosition.x = x;
}

void cCam::SetPositionY(float y)
{
	mPosition.y = y;
}

void cCam::SetPositionZ(float z)
{
	mPosition.z = z;
}

XMVECTOR cCam::GetRightXM()const
{
	return XMLoadFloat3(&mRight);
}

XMFLOAT3 cCam::GetRight()const
{
	return mRight;
}

XMVECTOR cCam::GetUpXM()const
{
	return XMLoadFloat3(&mUp);
}

XMFLOAT3 cCam::GetUp()const
{
	return mUp;
}

XMVECTOR cCam::GetLookXM()const
{
	return XMLoadFloat3(&mLook);
}

XMFLOAT3 cCam::GetLook()const
{
	return mLook;
}

XMFLOAT3 cCam::GetLookXZ()
{
	mLook.y = 0.0f;
	return mLook;
}

float cCam::GetNearZ()const
{
	return mNearZ;
}

float cCam::GetFarZ()const
{
	return mFarZ;
}

float cCam::GetAspect()const
{
	return mAspect;
}

float cCam::GetFovY()const
{
	return mFovY;
}

float cCam::GetFovX()const
{
	float halfWidth = 0.5f * GetNearWindowWidth();
	return 2.0f*atan(halfWidth / mNearZ);
}

float cCam::GetNearWindowWidth()const
{
	return mAspect * mNearWindowHeight;
}

float cCam::GetNearWindowHeight()const
{
	return mNearWindowHeight;
}

float cCam::GetFarWindowWidth()const
{
	return mAspect * mFarWindowHeight;
}

float cCam::GetFarWindowHeight()const
{
	return mFarWindowHeight;
}

XMMATRIX cCam::View()const
{
	return XMLoadFloat4x4(&mView);
}

XMMATRIX cCam::Proj()const
{
	return XMLoadFloat4x4(&mProj);
}

XMMATRIX cCam::ViewProj()const
{
	return XMMatrixMultiply(View(), Proj());
}

XMMATRIX cCam::UIView()const
{
	return XMLoadFloat4x4(&mUIView);
}

XMMATRIX cCam::UIProj()const
{
	return XMLoadFloat4x4(&mUIProj);
}

XMMATRIX cCam::UIViewProj()const
{
	return XMMatrixMultiply(UIView(), UIProj());
}

void cCam::Setm3PersonLength(float _amount)
{
	XMVECTOR l = { 0.0f, 0.0f, 0.0f, 0.0f };
	XMVECTOR s = XMVectorReplicate(_amount);
	XMVECTOR p = XMLoadFloat3(&mPosition);

	l = XMLoadFloat3(&mLook);

	m3PersonLength -= _amount;
	XMStoreFloat3(&mPosition, XMVectorMultiplyAdd(s, l, p));
}