// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//
#pragma once
#pragma warning(disable:4996)

// 디버그 모드 활성
//#define DEBUG_MODE

// 프로그램을 벗어나면 렌더링 중지
//#define WINDOW_DISABLE_NO_RENDER

#include <iostream>
#include <windows.h>
#include <algorithm>
#include <vector>
#include <list>
#include <map>
#include "targetver.h"

using namespace std;

// 윈도우 사이즈
#define WIN_X 800
#define WIN_Y 600

#define CAM_NEAR 1
#define CAM_FAR 5100
#define CAM_3PERSON_LENGTH 375

#define GRAVITY 800

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

// 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#define WIN32_LEAN_AND_MEAN           

// 보조 헤더
#include "cMathHelper.h"
#include "cXnaHelper.h"
#include "d3dx11Effect.h"
#include "vld.h"

// 사용자 정의 클래스 
// << 전체 공통 >>
#include "cSingleton.h"
#include "cCoreStorage.h"

// << 구조체 >>
#include "StructData.h"

// << 일반 클래스 >>
#include "cCam.h"
#include "cLightManager.h"
#include "cShadowMap.h"
#include "cAniManager.h"
#include "cMapManager.h"
#include "cModelManager.h"
#include "cShaderManager.h"
#include "cMaterialManager.h"
#include "cDrawManager.h"