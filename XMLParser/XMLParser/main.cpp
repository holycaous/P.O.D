#pragma once
#include "stdafx.h"
#include "cXMLParser.h"


void main()
{
	cXMLParser mXMLParser;

	//-----------------------------------------------------------------------------------------------------------//
	// 모델 추출  (파일명, 이름은 반드시 둘다 바꿔줘야함.)
	//
	// ★★★★★ 반드시 텍스처 바꿨나 확인★★★★★
	//
	//-----------------------------------------------------------------------------------------------------------//
	//mXMLParser.Init("Import/mob2.xml", "bbb");
	//mXMLParser.SetTexture("Goral_diff.dds", 100.0f, "Goral_norm.dds", "Goral_spec.dds");
	//mXMLParser.LoadXMLModel();


	//-----------------------------------------------------------------------------------------------------------//
	// 본 전용 파싱 (파일명, 이름은 반드시 둘다 바꿔줘야함.)
	//-----------------------------------------------------------------------------------------------------------//
	// 1.
	//mXMLParser.InitSkinModel("Import/mob2.xml", "bbb", "Idle");
	//mXMLParser.LoadXMLSkinModel();
	
	//mXMLParser.Init("Import/mob2.xml", "bbb", "Idle");
	//mXMLParser.LoadXMLBone();

	//// 2
	//mXMLParser.InitSkinModel("Import/mob2.xml", "bbb", "Damage");
	//mXMLParser.LoadXMLSkinModel();

	//mXMLParser.Init("Import/mob2.xml", "bbb", "Damage");
	//mXMLParser.LoadXMLBone();

	//// 3
	//mXMLParser.InitSkinModel("Import/mob2.xml", "bbb", "Run");
	//mXMLParser.LoadXMLSkinModel();

	//mXMLParser.Init("Import/mob2.xml", "bbb", "Run");
	//mXMLParser.LoadXMLBone();

	//// 4
	//mXMLParser.InitSkinModel("Import/mob2.xml", "bbb", "Walk");
	//mXMLParser.LoadXMLSkinModel();

	//mXMLParser.Init("Import/mob2.xml", "bbb", "Walk");
	//mXMLParser.LoadXMLBone();

	//// 5
	//mXMLParser.InitSkinModel("Import/mob2.xml", "bbb", "Death");
	//mXMLParser.LoadXMLSkinModel();
	
	//mXMLParser.Init("Import/mob2.xml", "bbb", "Death");
	//mXMLParser.LoadXMLBone();

	//// 6
	//mXMLParser.InitSkinModel("Import/mob2.xml", "bbb", "Attack");
	//mXMLParser.LoadXMLSkinModel();
	
	//mXMLParser.Init("Import/mob2.xml", "bbb", "Attack");
	//mXMLParser.LoadXMLBone();



	////-------------------------------------------------------------------------------------------------------------------//
	//// 예제 1
	//mXMLParser.Init("Import/Cyclops.xml", "eee");
	//mXMLParser.SetTexture("Cyclops01_diff.dds", 100.0f, "Cyclops01_norm.dds", "Cyclops01_spec.dds");
	//mXMLParser.SetTexture("Cyclops02_diff.dds", 100.0f, "Cyclops02_norm.dds", "Cyclops02_spec.dds");
	//mXMLParser.LoadXMLModel();
	//
	//mXMLParser.Init("Import/Cyclops.xml", "eee", "Idle");
	//mXMLParser.LoadXMLBone();
	//
	////-------------------------------------------------------------------------------------------------------------------//
	//
	//// 맵 테스트 용
	//mXMLParser.Init("Import/TestMap.xml", "fff");
	//mXMLParser.SetTexture("ground_diff.dds", 100.0f, "ground_norm.dds", "ground_spec.dds");
	//mXMLParser.LoadXMLModel();

}

