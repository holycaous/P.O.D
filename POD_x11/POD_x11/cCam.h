#pragma once
#include "UtilD3D.h"

class cCam/* : public cSingleton<cCam>*/
{
public:
	cCam();
	~cCam();

	// ÇÁ·¯½ºÅÒ »çÁø
	XNA::Frustum mCamFrustum;

	// Get/Set world camera position.
	void initCam(float _x, float _y, float _z);
	void initDir();
	XMVECTOR GetPositionXM()const;
	XMFLOAT3 GetPosition()const;
	void SetPosition(float x, float y, float z);
	void SetPosition(const XMFLOAT3& v);
	void SetPositionX(float x);
	void SetPositionY(float y);
	void SetPositionZ(float z);

	// Get camera basis vectors.
	XMVECTOR GetRightXM()const;
	XMFLOAT3 GetRight()const;
	XMVECTOR GetUpXM()const;
	XMFLOAT3 GetUp()const;
	XMVECTOR GetLookXM()const;
	XMFLOAT3 GetLook()const;
	XMFLOAT3 GetLookXZ();
	XMFLOAT3 GetThirdPosition()const;

	// Get frustum properties.
	float GetNearZ()const;
	float GetFarZ()const;
	float GetAspect()const;
	float GetFovY()const;
	float GetFovX()const;

	// Get near and far plane dimensions in view space coordinates.
	float GetNearWindowWidth()const;
	float GetNearWindowHeight()const;
	float GetFarWindowWidth()const;
	float GetFarWindowHeight()const;

	// Set frustum.
	void SetLens(float fovY, float aspect, float zn, float zf);
	void SetLens(float fovY, float aspect);

	//
	void FrustumProjection();

	// Define camera space via LookAt parameters.
	void LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp);
	void LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up);

	// Get View/Proj matrices.
	XMMATRIX View()const;
	XMMATRIX Proj()const;
	XMMATRIX ViewProj()const;

	// Strafe/Walk the camera a distance d.
	void Strafe(float d);
	void Walk(float d);

	// Rotate the camera.
	void Pitch(float angle);
	void RotateY(float angle);

	// After modifying camera position/orientation, call to rebuild the view matrix.
	void UpdateViewMatrix();

	void Change1PersonCam();
	void Change3PersonCam();

private:
	// Ä«¸Þ¶ó ¸ðµå (1ÀÎÄª, 3ÀÎÄª)
	CAM_MODE mCamMode = e_1Person;

	// Camera coordinate system with coordinates relative to world space.
	XMFLOAT3 mPosition;
	XMFLOAT3 mRight;
	XMFLOAT3 mUp;
	XMFLOAT3 mLook;
	XMFLOAT3 m3PersonLook;
	XMFLOAT3 m3PersonPosition;

	// Cache frustum properties.
	float mNearZ;
	float mFarZ;
	float mAspect;
	float mFovY;
	float mNearWindowHeight;
	float mFarWindowHeight;
	float m3PersonLength;

	// Cache View/Proj matrices.
	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;
};
