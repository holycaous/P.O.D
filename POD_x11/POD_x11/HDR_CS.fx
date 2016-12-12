//=============================================================================
//
// HDR CS ���̴�
//
//=============================================================================

#include "LightHelper.fx"

#define MAX_GROUPS 64 // 1024 ������ ũ�� / 16 �ȼ� == 64

// �߰� ����� ������ ���� �޸� �׷�ȭ
groupshared float SharedAvgFinal[MAX_GROUPS];

// �߰� ����� ������ ���� �޸� �׷�ȭ
groupshared float SharedPositions[1024];


//---------------------------------------------------------------------------------------------------------------//
//
// �ٿ� �����Ͽ� �ʿ��� �Լ���
//
//---------------------------------------------------------------------------------------------------------------//
float DownScale4x4(uint2 CurPixel, uint groupThreadId)
{
	float avgLum = 0.0;

	// �ٿ�� �� �ȼ��� �ǳ� �ݴϴ�.
	if (CurPixel.y < gTDownScaleCB.Res.y)
	{
		// 4x4 �ȼ� �׷� �հ�
		int3 nFullResPos = int3(CurPixel * 4, 0);
			float4 downScaled = float4(0.0, 0.0, 0.0, 0.0);
			[unroll]
		for (int i = 0; i < 4; i++)
		{
			[unroll]
			for (int j = 0; j < 4; j++)
			{
				downScaled += gHDRTex.Load(nFullResPos, int2(j, i));
			}
		}
		downScaled /= 16.0;

		//�� �ȼ��� �ֵ� ���� ����մϴ�.
		avgLum = dot(downScaled, LUM_FACTOR);

		//�� �ȼ��� �ֵ� ���� ����մϴ�.
		SharedPositions[groupThreadId] = avgLum;
	}

	// ����� ���� �޸𸮿� ���ϴ�.
	GroupMemoryBarrierWithGroupSync();

	return avgLum;
}

float DownScale1024to4(uint dispatchThreadId, uint groupThreadId, float avgLum)
{
	// �������� �ٿ� ������ �ڵ带 �����ϴ�.
	[unroll]
	for (uint groupSize = 4, step1 = 1, step2 = 2, step3 = 3; groupSize < 1024; groupSize *= 4, step1 *= 4, step2 *= 4, step3 *= 4)
	{
		// �ٿ�� �� �ȼ��� �ǳ� �ݴϴ�.
		if (groupThreadId % groupSize == 0)
		{
			//�� �ܰ��� �ֵ� �հ踦 ����մϴ�.
			float stepAvgLum = avgLum;
			stepAvgLum += dispatchThreadId + step1 < gTDownScaleCB.Domain ? SharedPositions[groupThreadId + step1] : avgLum;
			stepAvgLum += dispatchThreadId + step2 < gTDownScaleCB.Domain ? SharedPositions[groupThreadId + step2] : avgLum;
			stepAvgLum += dispatchThreadId + step3 < gTDownScaleCB.Domain ? SharedPositions[groupThreadId + step3] : avgLum;

			// ��� ����
			avgLum = stepAvgLum;
			SharedPositions[groupThreadId] = stepAvgLum;
		}

		// ���� �ܰ� ���� ����ȭ
		GroupMemoryBarrierWithGroupSync();
	}

	return avgLum;
}

void DownScale4to1(uint dispatchThreadId, uint groupThreadId, uint groupId, float avgLum)
{
	if (groupThreadId == 0)
	{
		//�� ������ �׷��� ��� ��ེ�� ����մϴ�.
		float fFinalAvgLum = avgLum;
		fFinalAvgLum += dispatchThreadId + 256 < gTDownScaleCB.Domain ? SharedPositions[groupThreadId + 256] : avgLum;
		fFinalAvgLum += dispatchThreadId + 512 < gTDownScaleCB.Domain ? SharedPositions[groupThreadId + 512] : avgLum;
		fFinalAvgLum += dispatchThreadId + 768 < gTDownScaleCB.Domain ? SharedPositions[groupThreadId + 768] : avgLum;
		fFinalAvgLum /= 1024.0;

		// ���� �ܰ迡�� ���� 1D UAV�� ���� ���� ���ϴ�.
		gAverageLum1[groupId] = fFinalAvgLum;
	}
}


//---------------------------------------------------------------------------------------------------------------//
//
// ù��° �н� - �ٿ� ������ �ϴ� ����
//
// 1024 == ��꽦�̴����� �����ϴ� �ִ� ������ ũ�� (x*y*z == 1024)
//---------------------------------------------------------------------------------------------------------------//
[numthreads(1024, 1, 1)] 
void DownScaleFirstPass(uint3 dispatchThreadId : SV_DispatchThreadID, uint3 groupThreadId : SV_GroupThreadID, uint3 groupId : SV_GroupID)
{
	uint2 CurPixel = uint2(dispatchThreadId.x % gTDownScaleCB.Res.x, dispatchThreadId.x / gTDownScaleCB.Res.x);

	// 16 �ȼ��� �׷��� ���� �ȼ��� ���̰� ���� �޸𸮿� ����
	float avgLum = DownScale4x4(CurPixel, groupThreadId.x);

	// 1024���� 4�� �������� ����
	avgLum = DownScale1024to4(dispatchThreadId.x, groupThreadId.x, avgLum);

	// 4���� 1�� �ٿ� ������
	DownScale4to1(dispatchThreadId.x, groupThreadId.x, groupId.x, avgLum);
}

//---------------------------------------------------------------------------------------------------------------//
//
// �ι�° �н� - 1D ��� ���� ���� ������ ��ȯ (��ġ�� ����)
//
//---------------------------------------------------------------------------------------------------------------//
[numthreads(MAX_GROUPS, 1, 1)]
void DownScaleSecondPass(uint3 groupId          : SV_GroupID         , uint3 groupThreadId : SV_GroupThreadID,
						 uint3 dispatchThreadId : SV_DispatchThreadID, uint groupIndex     : SV_GroupIndex)
{
	// ���� �޸𸮸� 1D ������ ä ��ϴ�.
	float avgLum = 0.0;
	if (dispatchThreadId.x < gTDownScaleCB.GroupSize)
	{
		avgLum = gAverageValues1D[dispatchThreadId.x];
	}
	SharedAvgFinal[dispatchThreadId.x] = avgLum;

	GroupMemoryBarrierWithGroupSync(); // ���� �ܰ� ���� ����ȭ

	// 64���� 16���� �ٿ� ������
	if (dispatchThreadId.x % 4 == 0)
	{
		//�� �ܰ��� �ֵ� �հ踦 ����մϴ�.
		float stepAvgLum = avgLum;
		stepAvgLum += dispatchThreadId.x + 1 < gTDownScaleCB.GroupSize ? SharedAvgFinal[dispatchThreadId.x + 1] : avgLum;
		stepAvgLum += dispatchThreadId.x + 2 < gTDownScaleCB.GroupSize ? SharedAvgFinal[dispatchThreadId.x + 2] : avgLum;
		stepAvgLum += dispatchThreadId.x + 3 < gTDownScaleCB.GroupSize ? SharedAvgFinal[dispatchThreadId.x + 3] : avgLum;

		// ��� ����
		avgLum = stepAvgLum;
		SharedAvgFinal[dispatchThreadId.x] = stepAvgLum;
	}

	GroupMemoryBarrierWithGroupSync();  // ���� �ܰ� ���� ����ȭ

	// 16���� 4�� �ٿ� ������
	if (dispatchThreadId.x % 16 == 0)
	{
		//�� �ܰ��� �ֵ� �հ踦 ����մϴ�.
		float stepAvgLum = avgLum;
		stepAvgLum += dispatchThreadId.x + 4  < gTDownScaleCB.GroupSize ? SharedAvgFinal[dispatchThreadId.x + 4]  : avgLum;
		stepAvgLum += dispatchThreadId.x + 8  < gTDownScaleCB.GroupSize ? SharedAvgFinal[dispatchThreadId.x + 8]  : avgLum;
		stepAvgLum += dispatchThreadId.x + 12 < gTDownScaleCB.GroupSize ? SharedAvgFinal[dispatchThreadId.x + 12] : avgLum;

		// ��� ����
		avgLum = stepAvgLum;
		SharedAvgFinal[dispatchThreadId.x] = stepAvgLum;
	}

	GroupMemoryBarrierWithGroupSync();  // ���� �ܰ� ���� ����ȭ

	// 4���� 1�� �ٿ� ������
	if (dispatchThreadId.x == 0)
	{
		//�� �ܰ��� �ֵ� �հ踦 ����մϴ�.
		float fFinalLumValue = avgLum;
		fFinalLumValue += dispatchThreadId.x + 16 < gTDownScaleCB.GroupSize ? SharedAvgFinal[dispatchThreadId.x + 16] : avgLum;
		fFinalLumValue += dispatchThreadId.x + 32 < gTDownScaleCB.GroupSize ? SharedAvgFinal[dispatchThreadId.x + 32] : avgLum;
		fFinalLumValue += dispatchThreadId.x + 48 < gTDownScaleCB.GroupSize ? SharedAvgFinal[dispatchThreadId.x + 48] : avgLum;
		fFinalLumValue /= 64.0;

		// ���� �� ����
		gAverageLum2[0] = max(fFinalLumValue, 0.0001);
	}
}

technique11 HDRDownScale
{
	pass P0
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, DownScaleFirstPass()));
	}
}

technique11 CombineHDR
{
	pass P0
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, DownScaleSecondPass()));
	}
}