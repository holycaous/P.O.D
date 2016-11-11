#pragma once
#include "stdafx.h"
#include "cXMLParser.h"


void main()
{
	cXMLParser mXMLParser;
	
	//-------------------------------------------------------------------------------------------------------------------//
	// �׽�Ʈ 1
	mXMLParser.Init("Import/Skinning_Test.xml", "aaa");
	mXMLParser.SetTexture("WoodCrate01_diff.dds", 100.0f, "WoodCrate01_norm.dds", "WoodCrate01_spec.dds");
	mXMLParser.LoadXMLModel();

	mXMLParser.Init("Import/Skinning_Test.xml", "aaa", "Idle");
	mXMLParser.LoadXMLBone();

	//-------------------------------------------------------------------------------------------------------------------//
	// �׽�Ʈ 2
	mXMLParser.Init("Import/Aman_boy.xml", "bbb");
	mXMLParser.SetTexture("Aman_boy_diff.dds", 100.0f, "Aman_boy_norm.dds", "Aman_boy_spec.dds");
	mXMLParser.LoadXMLModel();

	mXMLParser.Init("Import/Aman_boy.xml", "bbb", "Idle");
	mXMLParser.LoadXMLBone();

	//-------------------------------------------------------------------------------------------------------------------//
	// �׽�Ʈ 3
	mXMLParser.Init("Import/Cat1.xml", "ccc");
	mXMLParser.SetTexture("Cat1_diff.dds", 100.0f, "Cat1_norm.dds", "Cat1_spec.dds");
	mXMLParser.LoadXMLModel();

	mXMLParser.Init("Import/Cat1.xml", "ccc", "Idle");
	mXMLParser.LoadXMLBone();
	
	//-------------------------------------------------------------------------------------------------------------------//
	// �׽�Ʈ 4
	mXMLParser.Init("Import/AnonSoldier.xml", "ddd");
	mXMLParser.SetTexture("AnonSoldier_diff.dds", 100.0f, "AnonSoldier_norm.dds", "AnonSoldier_spec.dds");
	mXMLParser.LoadXMLModel();

	mXMLParser.Init("Import/AnonSoldier.xml", "ddd", "Idle");
	mXMLParser.LoadXMLBone();

	//-------------------------------------------------------------------------------------------------------------------//
	// �׽�Ʈ 5
	mXMLParser.Init("Import/Cyclops.xml", "eee");
	mXMLParser.SetTexture("Cyclops01_diff.dds", 100.0f, "Cyclops01_norm.dds", "Cyclops01_spec.dds");
	mXMLParser.SetTexture("Cyclops02_diff.dds", 100.0f, "Cyclops02_norm.dds", "Cyclops02_spec.dds");
	mXMLParser.LoadXMLModel();

	mXMLParser.Init("Import/Cyclops.xml", "eee", "Idle");
	mXMLParser.LoadXMLBone();
	

	//-------------------------------------------------------------------------------------------------------------------//
	// �׽�Ʈ 6
	mXMLParser.Init("Import/TestSkin.xml", "ggg");
	mXMLParser.SetTexture("WoodCrate012_diff.dds", 100.0f, "WoodCrate012_norm.dds", "WoodCrate012_spec.dds");
	mXMLParser.LoadXMLModel();

	mXMLParser.Init("Import/TestSkin.xml", "ggg", "Idle");
	mXMLParser.LoadXMLBone();
	//-------------------------------------------------------------------------------------------------------------------//

	// �� �׽�Ʈ ��
	mXMLParser.Init("Import/TestMap.xml", "fff");
	mXMLParser.SetTexture("ground_diff.dds", 100.0f, "ground_norm.dds", "ground_spec.dds");
	mXMLParser.LoadXMLModel();



}

