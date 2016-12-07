// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//
#pragma once
#pragma warning(disable:4996)

// ����� ��� Ȱ��
//#define DEBUG_MODE

// ���α׷��� ����� ������ ����
//#define WINDOW_DISABLE_NO_RENDER

#include <iostream>
#include <windows.h>
#include <algorithm>
#include <vector>
#include <list>
#include <map>
#include "targetver.h"

using namespace std;

// ������ ������
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

// ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
#define WIN32_LEAN_AND_MEAN           

// ���� ���
#include "cMathHelper.h"
#include "cXnaHelper.h"
#include "d3dx11Effect.h"
#include "vld.h"

// ����� ���� Ŭ���� 
// << ��ü ���� >>
#include "cSingleton.h"
#include "cCoreStorage.h"

// << ����ü >>
#include "StructData.h"

// << �Ϲ� Ŭ���� >>
#include "cCam.h"
#include "cLightManager.h"
#include "cShadowMap.h"
#include "cAniManager.h"
#include "cMapManager.h"
#include "cModelManager.h"
#include "cShaderManager.h"
#include "cMaterialManager.h"
#include "cDrawManager.h"