//=============================================================================
//
// HDR CS 쉐이더
//
//=============================================================================

#include "LightHelper.fx"

#define MAX_GROUPS 64 // 1024 스레드 크기 / 16 픽셀 == 64

// 중간 결과를 저장할 공유 메모리 그룹화
groupshared float SharedAvgFinal[MAX_GROUPS];

// 중간 결과를 저장할 공유 메모리 그룹화
groupshared float SharedPositions[1024];


//---------------------------------------------------------------------------------------------------------------//
//
// 다운 스케일에 필요한 함수들
//
//---------------------------------------------------------------------------------------------------------------//
float DownScale4x4(uint2 CurPixel, uint groupThreadId)
{
	float avgLum = 0.0;

	// 바운드 된 픽셀을 건너 뜁니다.
	if (CurPixel.y < gTDownScaleCB.Res.y)
	{
		// 4x4 픽셀 그룹 합계
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

		//이 픽셀의 휘도 값을 계산합니다.
		avgLum = dot(downScaled, LUM_FACTOR);

		//이 픽셀의 휘도 값을 계산합니다.
		SharedPositions[groupThreadId] = avgLum;
	}

	// 결과를 공유 메모리에 씁니다.
	GroupMemoryBarrierWithGroupSync();

	return avgLum;
}

float DownScale1024to4(uint dispatchThreadId, uint groupThreadId, float avgLum)
{
	// 루프에서 다운 스케일 코드를 보냅니다.
	[unroll]
	for (uint groupSize = 4, step1 = 1, step2 = 2, step3 = 3; groupSize < 1024; groupSize *= 4, step1 *= 4, step2 *= 4, step3 *= 4)
	{
		// 바운드 된 픽셀을 건너 뜁니다.
		if (groupThreadId % groupSize == 0)
		{
			//이 단계의 휘도 합계를 계산합니다.
			float stepAvgLum = avgLum;
			stepAvgLum += dispatchThreadId + step1 < gTDownScaleCB.Domain ? SharedPositions[groupThreadId + step1] : avgLum;
			stepAvgLum += dispatchThreadId + step2 < gTDownScaleCB.Domain ? SharedPositions[groupThreadId + step2] : avgLum;
			stepAvgLum += dispatchThreadId + step3 < gTDownScaleCB.Domain ? SharedPositions[groupThreadId + step3] : avgLum;

			// 결과 저장
			avgLum = stepAvgLum;
			SharedPositions[groupThreadId] = stepAvgLum;
		}

		// 다음 단계 전에 동기화
		GroupMemoryBarrierWithGroupSync();
	}

	return avgLum;
}

void DownScale4to1(uint dispatchThreadId, uint groupThreadId, uint groupId, float avgLum)
{
	if (groupThreadId == 0)
	{
		//이 스레드 그룹의 평균 루멘스를 계산합니다.
		float fFinalAvgLum = avgLum;
		fFinalAvgLum += dispatchThreadId + 256 < gTDownScaleCB.Domain ? SharedPositions[groupThreadId + 256] : avgLum;
		fFinalAvgLum += dispatchThreadId + 512 < gTDownScaleCB.Domain ? SharedPositions[groupThreadId + 512] : avgLum;
		fFinalAvgLum += dispatchThreadId + 768 < gTDownScaleCB.Domain ? SharedPositions[groupThreadId + 768] : avgLum;
		fFinalAvgLum /= 1024.0;

		// 다음 단계에서 사용될 1D UAV에 최종 값을 씁니다.
		gAverageLum1[groupId] = fFinalAvgLum;
	}
}


//---------------------------------------------------------------------------------------------------------------//
//
// 첫번째 패스 - 다운 스케일 하는 과정
//
// 1024 == 계산쉐이더에서 지원하는 최대 스레드 크기 (x*y*z == 1024)
//---------------------------------------------------------------------------------------------------------------//
[numthreads(1024, 1, 1)] 
void DownScaleFirstPass(uint3 dispatchThreadId : SV_DispatchThreadID, uint3 groupThreadId : SV_GroupThreadID, uint3 groupId : SV_GroupID)
{
	uint2 CurPixel = uint2(dispatchThreadId.x % gTDownScaleCB.Res.x, dispatchThreadId.x / gTDownScaleCB.Res.x);

	// 16 픽셀의 그룹을 단일 픽셀로 줄이고 공유 메모리에 저장
	float avgLum = DownScale4x4(CurPixel, groupThreadId.x);

	// 1024에서 4로 스케일을 내림
	avgLum = DownScale1024to4(dispatchThreadId.x, groupThreadId.x, avgLum);

	// 4에서 1로 다운 스케일
	DownScale4to1(dispatchThreadId.x, groupThreadId.x, groupId.x, avgLum);
}

//---------------------------------------------------------------------------------------------------------------//
//
// 두번째 패스 - 1D 평균 값을 단일 값으로 변환 (합치는 과정)
//
//---------------------------------------------------------------------------------------------------------------//
[numthreads(MAX_GROUPS, 1, 1)]
void DownScaleSecondPass(uint3 groupId          : SV_GroupID         , uint3 groupThreadId : SV_GroupThreadID,
						 uint3 dispatchThreadId : SV_DispatchThreadID, uint groupIndex     : SV_GroupIndex)
{
	// 공유 메모리를 1D 값으로 채 웁니다.
	float avgLum = 0.0;
	if (dispatchThreadId.x < gTDownScaleCB.GroupSize)
	{
		avgLum = gAverageValues1D[dispatchThreadId.x];
	}
	SharedAvgFinal[dispatchThreadId.x] = avgLum;

	GroupMemoryBarrierWithGroupSync(); // 다음 단계 전에 동기화

	// 64에서 16으로 다운 스케일
	if (dispatchThreadId.x % 4 == 0)
	{
		//이 단계의 휘도 합계를 계산합니다.
		float stepAvgLum = avgLum;
		stepAvgLum += dispatchThreadId.x + 1 < gTDownScaleCB.GroupSize ? SharedAvgFinal[dispatchThreadId.x + 1] : avgLum;
		stepAvgLum += dispatchThreadId.x + 2 < gTDownScaleCB.GroupSize ? SharedAvgFinal[dispatchThreadId.x + 2] : avgLum;
		stepAvgLum += dispatchThreadId.x + 3 < gTDownScaleCB.GroupSize ? SharedAvgFinal[dispatchThreadId.x + 3] : avgLum;

		// 결과 저장
		avgLum = stepAvgLum;
		SharedAvgFinal[dispatchThreadId.x] = stepAvgLum;
	}

	GroupMemoryBarrierWithGroupSync();  // 다음 단계 전에 동기화

	// 16에서 4로 다운 스케일
	if (dispatchThreadId.x % 16 == 0)
	{
		//이 단계의 휘도 합계를 계산합니다.
		float stepAvgLum = avgLum;
		stepAvgLum += dispatchThreadId.x + 4  < gTDownScaleCB.GroupSize ? SharedAvgFinal[dispatchThreadId.x + 4]  : avgLum;
		stepAvgLum += dispatchThreadId.x + 8  < gTDownScaleCB.GroupSize ? SharedAvgFinal[dispatchThreadId.x + 8]  : avgLum;
		stepAvgLum += dispatchThreadId.x + 12 < gTDownScaleCB.GroupSize ? SharedAvgFinal[dispatchThreadId.x + 12] : avgLum;

		// 결과 저장
		avgLum = stepAvgLum;
		SharedAvgFinal[dispatchThreadId.x] = stepAvgLum;
	}

	GroupMemoryBarrierWithGroupSync();  // 다음 단계 전에 동기화

	// 4에서 1로 다운 스케일
	if (dispatchThreadId.x == 0)
	{
		//이 단계의 휘도 합계를 계산합니다.
		float fFinalLumValue = avgLum;
		fFinalLumValue += dispatchThreadId.x + 16 < gTDownScaleCB.GroupSize ? SharedAvgFinal[dispatchThreadId.x + 16] : avgLum;
		fFinalLumValue += dispatchThreadId.x + 32 < gTDownScaleCB.GroupSize ? SharedAvgFinal[dispatchThreadId.x + 32] : avgLum;
		fFinalLumValue += dispatchThreadId.x + 48 < gTDownScaleCB.GroupSize ? SharedAvgFinal[dispatchThreadId.x + 48] : avgLum;
		fFinalLumValue /= 64.0;

		// 최종 값 저장
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