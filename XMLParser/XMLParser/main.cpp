#pragma once
#include "stdafx.h"
#include "cXMLParser.h"


void main()
{
	cXMLParser mXMLParser;

	//-----------------------------------------------------------------------------------------------------------//
	// �� ����  (���ϸ�, �̸��� �ݵ�� �Ѵ� �ٲ������.)
	//
	// �ڡڡڡڡ� �ݵ�� �ؽ�ó �ٲ峪 Ȯ�Ρڡڡڡڡ�
	//
	//-----------------------------------------------------------------------------------------------------------//
	//mXMLParser.Init("Import/mob2.xml", "bbb");
	//mXMLParser.SetTexture("Goral_diff.dds", 100.0f, "Goral_norm.dds", "Goral_spec.dds");
	//mXMLParser.LoadXMLModel();


	//-----------------------------------------------------------------------------------------------------------//
	// �� ���� �Ľ� (���ϸ�, �̸��� �ݵ�� �Ѵ� �ٲ������.)
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
	//// ���� 1
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
	//// �� �׽�Ʈ ��
	//mXMLParser.Init("Import/TestMap.xml", "fff");
	//mXMLParser.SetTexture("ground_diff.dds", 100.0f, "ground_norm.dds", "ground_spec.dds");
	//mXMLParser.LoadXMLModel();

}

