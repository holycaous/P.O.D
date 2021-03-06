#pragma once
#include "UtilD3D.h"
#include "cGameTimer.h"

class cCoreStorage : public cSingleton<cCoreStorage>
{
public:
	HINSTANCE* mInst;
	HWND* mhWnd;

	cGameTimer* mTimer;

	ID3D11Device* md3dDevice;
	ID3D11DeviceContext* md3dImmediateContext;
	IDXGISwapChain* mSwapChain;
	ID3D11Texture2D* mDepthStencilTexture;

	// SRV
	ID3D11ShaderResourceView* mNomalSRV;
	ID3D11ShaderResourceView* mPositionSRV;
	ID3D11ShaderResourceView* mColorSRV;
	ID3D11ShaderResourceView* mSpecularSRV;
	ID3D11ShaderResourceView* mDepthSRV;

	// RTV
	ID3D11RenderTargetView* mNomalRTV;
	ID3D11RenderTargetView* mColorRTV;
	ID3D11RenderTargetView* mPositionRTV;
	ID3D11RenderTargetView* mSpecularRTV;
	ID3D11RenderTargetView* mSreenRTV;
	ID3D11RenderTargetView* mDepthRTV;

	// DSV
	ID3D11DepthStencilView* mMainDSV;
	ID3D11DepthStencilView* mOnlyReadDSV;

	// DSV ����
	ID3D11DepthStencilState* mDepthStencilState;
	
	// �� ��Ʈ
	D3D11_VIEWPORT* mScreenViewport;


	// Ŭ���̾�Ʈ ũ��
	float mClientWidth;
	float mClientHeight;

	// ȭ�� ���� ���
	float AspectRatio() const
	{
		return static_cast<float>(mClientWidth) / mClientHeight;
	}

	// ���� Ÿ�� Ŭ����
	void PreRender()
	{
		// ���� ���� Ŭ����
		md3dImmediateContext->ClearDepthStencilView(mMainDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		// ���� Ÿ�� Ŭ����
		md3dImmediateContext->ClearRenderTargetView(mDepthRTV   , reinterpret_cast<const float*>(&Colors::LightSteelBlue));
		md3dImmediateContext->ClearRenderTargetView(mColorRTV   , reinterpret_cast<const float*>(&Colors::LightSteelBlue));
		md3dImmediateContext->ClearRenderTargetView(mNomalRTV   , reinterpret_cast<const float*>(&Colors::LightSteelBlue));
		md3dImmediateContext->ClearRenderTargetView(mPositionRTV, reinterpret_cast<const float*>(&Colors::LightSteelBlue));
		md3dImmediateContext->ClearRenderTargetView(mSpecularRTV, reinterpret_cast<const float*>(&Colors::LightSteelBlue));
		
		md3dImmediateContext->OMSetDepthStencilState(mDepthStencilState, 1);

		ID3D11RenderTargetView* rt[5] = { mDepthRTV, mColorRTV, mNomalRTV, mPositionRTV, mSpecularRTV };
		md3dImmediateContext->OMSetRenderTargets(5, rt, mMainDSV);
	}

	// �����ϱ� ���� �ؽ�ó ��
	void PostRender()
	{
		// ���� Ÿ�� Ŭ����
		md3dImmediateContext->ClearDepthStencilView(mMainDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		ID3D11RenderTargetView* rt[5] = { NULL, NULL, NULL, NULL, NULL };
		md3dImmediateContext->OMSetRenderTargets(5, rt, NULL); //mOnlyReadDSV
	}

	// ����Ÿ�� ��
	void SetRenderTaget()
	{
		md3dImmediateContext->OMSetRenderTargets(1, &mSreenRTV, mMainDSV);
		//md3dImmediateContext->ClearDepthStencilView(mOnlyReadDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	void ReleaseRenderTaget()
	{
		md3dImmediateContext->ClearRenderTargetView(mSreenRTV, reinterpret_cast<const float*>(&Colors::LightSteelBlue));
	}

	void ClearClass()
	{
		PostRender();
		ReleaseRenderTaget();
	}
};

