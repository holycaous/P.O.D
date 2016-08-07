// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
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

using namespace std;

// 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#define WIN32_LEAN_AND_MEAN           

// 보조 헤더
#include "d3dx11Effect.h"

// << 구조체 >>
#include "StructData.h"

