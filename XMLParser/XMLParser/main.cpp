#pragma once
#include "stdafx.h"
#include "cXMLParser.h"


void main()
{
	cXMLParser mXMLParser;
	////-------------------------------------------------------------------------------------------------------------------//
	//// �׽�Ʈ 1
	//mXMLParser.Init("Import/Skinning_Test.xml", "aaa");
	//mXMLParser.SetTexture(0, "WoodCrate01_diff.dds", 100.0f, "WoodCrate01_norm.dds", "WoodCrate01_spec.dds");
	//mXMLParser.LoadXMLModel();

	//mXMLParser.Init("Import/Skinning_Test.xml", "aaa", "Idel");
	//mXMLParser.LoadXMLBone();

	////-------------------------------------------------------------------------------------------------------------------//
	//// �׽�Ʈ 2
	//mXMLParser.Init("Import/Aman_boy.xml", "bbb");
	//mXMLParser.SetTexture(1, "Aman_boy_diff.dds", 100.0f, "Aman_boy_norm.dds", "Aman_boy_spec.dds");
	//mXMLParser.LoadXMLModel();

	//mXMLParser.Init("Import/Aman_boy.xml", "bbb", "Idel");
	//mXMLParser.LoadXMLBone();

	////-------------------------------------------------------------------------------------------------------------------//
	//// �׽�Ʈ 3
	//mXMLParser.Init("Import/Cat1.xml", "ccc");
	//mXMLParser.SetTexture(1, "Cat1_diff.dds", 100.0f, "Cat1_norm.dds", "Cat1_spec.dds");
	//mXMLParser.LoadXMLModel();

	//mXMLParser.Init("Import/Cat1.xml", "ccc", "Idel");
	//mXMLParser.LoadXMLBone();
	//
	////-------------------------------------------------------------------------------------------------------------------//
	//// �׽�Ʈ 4
	//mXMLParser.Init("Import/AnonSoldier.xml", "ddd");
	//mXMLParser.SetTexture(1, "AnonSoldier_diff.dds", 100.0f, "AnonSoldier_norm.dds", "AnonSoldier_spec.dds");
	//mXMLParser.LoadXMLModel();

	//mXMLParser.Init("Import/AnonSoldier.xml", "ddd", "Idel");
	//mXMLParser.LoadXMLBone();

	////-------------------------------------------------------------------------------------------------------------------//
	//// �׽�Ʈ 5
	//mXMLParser.Init("Import/Cyclops.xml", "eee");
	//mXMLParser.SetTexture(1, "Cyclops01_diff.dds", 100.0f, "Cyclops01_norm.dds", "Cyclops01_spec.dds");
	//mXMLParser.SetTexture(2, "Cyclops02_diff.dds", 100.0f, "Cyclops02_norm.dds", "Cyclops02_spec.dds");
	//mXMLParser.LoadXMLModel();

	//mXMLParser.Init("Import/Cyclops.xml", "eee", "Idel");
	//mXMLParser.LoadXMLBone();
	//
	////-------------------------------------------------------------------------------------------------------------------//

	// �� �׽�Ʈ ��
	mXMLParser.Init("Import/TestMap.xml", "fff");
	mXMLParser.SetTexture(0, "WoodCrate01_diff.dds", 100.0f, "WoodCrate01_norm.dds", "WoodCrate01_spec.dds");
	mXMLParser.LoadXMLModel();


}

