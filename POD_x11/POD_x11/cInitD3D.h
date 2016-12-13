#pragma once

#ifndef cInitD3D_H
#define cInitD3D_H

#include "UtilD3D.h"
#include "cGameTimer.h"
#include "cGameStateManager.h"
#include <string>

class cInitD3D
{
public:
	cInitD3D(HINSTANCE hInstance);
	virtual ~cInitD3D();

	HINSTANCE AppInst()const;
	HWND      MainWnd()const;
	float     AspectRatio()const;

	int Run();

	// 프레임 워크 방법. 
	// 파생 클라이언트 클래스는 이러한 메소드를 오버라이드 (override)하여,
	// 특정 애플리케이션의 요구 사항을 구현한다.

	virtual bool Init();
	virtual void OnResize();
	virtual void UpdateScene(float dt) = 0;
	virtual void DrawScene() = 0;
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	bool InitMainWindow();
	bool InitDirect3D();

	void CalculateFrameStats();
	void SetCoreStorage();

protected:
	HINSTANCE mhAppInst;
	HWND      mhMainWnd;
	bool      mAppPaused;
	bool      mMinimized;
	bool      mMaximized;
	bool      mResizing;
	UINT      m4xMsaaQuality;

	cGameTimer mTimer;

	ID3D11Device* md3dDevice;
	ID3D11DeviceContext* md3dImmediateContext;
	IDXGISwapChain* mSwapChain;
	ID3D11Texture2D* mDepthStencilTexture;
	ID3D11Texture2D* mHDRTexture;

	// SRV
	ID3D11ShaderResourceView* mNomalSRV;
	ID3D11ShaderResourceView* mColorSRV;
	ID3D11ShaderResourceView* mPositionSRV;
	ID3D11ShaderResourceView* mSpecularSRV;
	ID3D11ShaderResourceView* mDepthSRV;
	ID3D11ShaderResourceView* mShadowSRV;
	ID3D11ShaderResourceView* mHDRSRV;

	// RTV
	ID3D11RenderTargetView* mNomalRTV;
	ID3D11RenderTargetView* mColorRTV;
	ID3D11RenderTargetView* mPositionRTV;
	ID3D11RenderTargetView* mSpecularRTV;
	ID3D11RenderTargetView* mSreenRTV;
	ID3D11RenderTargetView* mHDRRTV;
	ID3D11RenderTargetView* mDepthRTV;
	ID3D11RenderTargetView* mShadowRTV;

	// DSV
	ID3D11DepthStencilView* mMainDSV;
	ID3D11DepthStencilView* mOnlyReadDSV;

	// DSV 상태
	ID3D11DepthStencilState* mDepthStencilState;
	
	D3D11_VIEWPORT mScreenViewport;

	// Derived class should set these in derived constructor to customize starting values.
	std::wstring mMainWndCaption;
	D3D_DRIVER_TYPE md3dDriverType;
	int mClientWidth;
	int mClientHeight;
	bool mEnable4xMsaa;

	void ClearResourceView();
	void ClearDevice();
	bool ReCreateDevice();
	void CheckDeviceLost();
};

#endif // cInitD3D_H