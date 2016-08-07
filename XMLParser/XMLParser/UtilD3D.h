#pragma once

//---------------------------------------------------------------------------------------
// 각종 헤더 파일들
//---------------------------------------------------------------------------------------
#ifndef UtilD3D_H
#define UtilD3D_H

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include <d3d11.h>
//#include "d3d11.h"
#include <dxgi.h>
//#include "d3dx11Effect.h"
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <cassert>
#include <ctime>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include "D3DX11tex.h"
#include "cMathHelper.h"

using namespace DirectX;
using namespace DirectX::PackedVector;

//---------------------------------------------------------------------------------------
// 함수의 인라인화를 도와주는 메크로
//---------------------------------------------------------------------------------------
#ifndef D3DX11INLINE
#ifdef _MSC_VER
#if (_MSC_VER >= 1200)
#define D3DX11INLINE __forceinline
#else
#define D3DX11INLINE __inline
#endif
#else
#ifdef __cplusplus
#define D3DX11INLINE inline
#else
#define D3DX11INLINE
#endif
#endif
#endif

//---------------------------------------------------------------------------------------
// 오류 체크를 도와주는 메크로
//---------------------------------------------------------------------------------------
#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)										\
{													\
	HRESULT hr = (x);								\
if (FAILED(hr))										\
{													\
	LPWSTR output;                                  \
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | \
	FORMAT_MESSAGE_IGNORE_INSERTS | \
	FORMAT_MESSAGE_ALLOCATE_BUFFER, \
	NULL, \
	hr, \
	MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), \
	(LPTSTR)&output, \
	0, \
	NULL);									        \
	MessageBox(NULL, output, L"Error", MB_OK);	    \
}										            \
}
#endif
#else
#ifndef HR
#define HR(x) (x)
#endif
#endif 

//---------------------------------------------------------------------------------------
// 기본 색상을 지정해둔다.
//---------------------------------------------------------------------------------------
namespace Colors
{
	XMGLOBALCONST XMVECTORF32 White = { 1.0f, 1.0f, 1.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Black = { 0.0f, 0.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Red = { 1.0f, 0.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Green = { 0.0f, 1.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Blue = { 0.0f, 0.0f, 1.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Yellow = { 1.0f, 1.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Cyan = { 0.0f, 1.0f, 1.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Magenta = { 1.0f, 0.0f, 1.0f, 1.0f };

	XMGLOBALCONST XMVECTORF32 Silver = { 0.75f, 0.75f, 0.75f, 1.0f };
	XMGLOBALCONST XMVECTORF32 LightSteelBlue = { 0.69f, 0.77f, 0.87f, 1.0f };
}

//---------------------------------------------------------------------------------------
// 컴 객체 릴리즈를 도와준다.
//---------------------------------------------------------------------------------------

#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }

//---------------------------------------------------------------------------------------
// 삭제를 도와주는 메크로
//---------------------------------------------------------------------------------------

#define SafeDelete(x) { delete x; x = 0; }

//---------------------------------------------------------------------------------------
// 유틸리티 클래스들
//---------------------------------------------------------------------------------------
// 텍스트 출력을 도와준다.
class TextHelper
{
public:

	template<typename T>
	static D3DX11INLINE std::wstring ToString(const T& s)
	{
		std::wostringstream oss;
		oss << s;

		return oss.str();
	}

	template<typename T>
	static D3DX11INLINE T FromString(const std::wstring& s)
	{
		T x;
		std::wistringstream iss(s);
		iss >> x;

		return x;
	}
};

// 자료형 간 형변환을 도와주는 클래스
class Convert
{
public:
	// FXMVECTOR -> XMCOLOR
	static D3DX11INLINE XMCOLOR ToXmColor(FXMVECTOR v)
	{
		XMCOLOR dest;
		XMStoreColor(&dest, v);
		return dest;
	}

	// FXMVECTOR -> XMFLOAT4
	static D3DX11INLINE XMFLOAT4 ToXmFloat4(FXMVECTOR v)
	{
		XMFLOAT4 dest;
		XMStoreFloat4(&dest, v);
		return dest;
	}

	// 아직은 왜 이렇게 바꿔야하는지 모르겠음..-_-
	static D3DX11INLINE UINT ArgbToAbgr(UINT argb)
	{
		BYTE A = (argb >> 24) & 0xff;
		BYTE R = (argb >> 16) & 0xff;
		BYTE G = (argb >> 8) & 0xff;
		BYTE B = (argb >> 0) & 0xff;

		return (A << 24) | (B << 16) | (G << 8) | (R << 0);
	}

};

#endif // UtilD3D_H


//---------------------------------------------------------------------------------------
// 땅바닥
//---------------------------------------------------------------------------------------
void ExtractFrustumPlanes(XMFLOAT4 planes[6], CXMMATRIX M);


