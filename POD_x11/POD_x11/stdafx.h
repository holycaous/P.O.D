// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//
#pragma once
#pragma warning(disable:4996)

#define DEBUG_MODE

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

#define CAM_FAR 1500.0f

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
#include "cAniManager.h"
#include "cModelManager.h"
#include "cShaderManager.h"
#include "cMaterialManager.h"
#include "cDrawManager.h"
