#ifndef _COMPUTE_FX_
#define _COMPUTE_FX_

#include "params.fx"
/*
* u = unordered access view 
* 전용 레지스터, 일반 그래픽 연산에 사용되진 않음.
* RW = Read Write : shader 코드를 읽을 뿐만 아니라 수정도 가능. 읽고 쓰기가 다 가능하다.
* 일반 쉐이더는 읽기(Read)만
*/
RWTexture2D<float4> g_rwtex_0 : register(u0);

[numthreads(1024, 1, 1)]
void CS_Main(int3 threadIndex : SV_DispatchThreadID)
{
	if (threadIndex.y % 2 == 0)
	{
		g_rwtex_0[threadIndex.xy] = float4(1.0f, 0.0f, 0.0f, 1.0f); //RGBA
	}
	else
	{
		g_rwtex_0[threadIndex.xy] = float4(0.0f, 1.0f, 0.0f, 1.0f);
	}
}

#endif

/*
	GPU의 Tread란 ? :

	compute shader를 이용할 때 사용할 때 Dispatch라는 함수를 사용한다.
	void Dispatch(Params : UINT ThreadGroupCountx, y, z);

	cpu의 Thread는 프로그램을 실행하는 영혼 주체 자체?.. 프로그램을 실행하고 있는 그 자체
	GPU는 하나의 일감을 쓰레드라고 표현한다.

	GPU는 Thread를 격자 무늬로 묶어서 관리한다.(행렬 처럼 번호가 존재한다. ex : 3 x 4)

	스레드가 하나의 일감이라고 한다면,
	스레드를 그룹핑하여 묶어놓은 것을 블록(쓰레드 그룹)이라고 한다.
	이러한 블록들을 다시 그룹핑하여 2차원 또는 3차원으로 만들어진 것을 그리드라고한다.
	Dispatch는 이러한 그리드를 호출한다.
*/
/*
	[numthreads(x, y, z)] 란 ?
	하나의 쓰레드 그룹당 쓰레드의 개수를 의미한다.
	최대의 개수는 1024로 x, y, z를 다 곱한 값이 1024를 넘기면 안된다.
	예시에는 1024, 1, 1 = 1024임으로 최대 숫자를 사용한다.
	
	당연하게도 이러한 x, y, z의 값을 정해주는데는 의미가 있다. : 넘버링을 하는 것.

	(x, y, z) = 그리드 하나의 x, y 좌표, z는 몇 번째 그리드인가.


*/