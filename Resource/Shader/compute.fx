#ifndef _COMPUTE_FX_
#define _COMPUTE_FX_

#include "params.fx"
/*
* u = unordered access view 
* ���� ��������, �Ϲ� �׷��� ���꿡 ������ ����.
* RW = Read Write : shader �ڵ带 ���� �Ӹ� �ƴ϶� ������ ����. �а� ���Ⱑ �� �����ϴ�.
* �Ϲ� ���̴��� �б�(Read)��
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
	GPU�� Tread�� ? :

	compute shader�� �̿��� �� ����� �� Dispatch��� �Լ��� ����Ѵ�.
	void Dispatch(Params : UINT ThreadGroupCountx, y, z);

	cpu�� Thread�� ���α׷��� �����ϴ� ��ȥ ��ü ��ü?.. ���α׷��� �����ϰ� �ִ� �� ��ü
	GPU�� �ϳ��� �ϰ��� �������� ǥ���Ѵ�.

	GPU�� Thread�� ���� ���̷� ��� �����Ѵ�.(��� ó�� ��ȣ�� �����Ѵ�. ex : 3 x 4)

	�����尡 �ϳ��� �ϰ��̶�� �Ѵٸ�,
	�����带 �׷����Ͽ� ������� ���� ���(������ �׷�)�̶�� �Ѵ�.
	�̷��� ��ϵ��� �ٽ� �׷����Ͽ� 2���� �Ǵ� 3�������� ������� ���� �׸������Ѵ�.
	Dispatch�� �̷��� �׸��带 ȣ���Ѵ�.
*/
/*
	[numthreads(x, y, z)] �� ?
	�ϳ��� ������ �׷�� �������� ������ �ǹ��Ѵ�.
	�ִ��� ������ 1024�� x, y, z�� �� ���� ���� 1024�� �ѱ�� �ȵȴ�.
	���ÿ��� 1024, 1, 1 = 1024������ �ִ� ���ڸ� ����Ѵ�.
	
	�翬�ϰԵ� �̷��� x, y, z�� ���� �����ִµ��� �ǹ̰� �ִ�. : �ѹ����� �ϴ� ��.

	(x, y, z) = �׸��� �ϳ��� x, y ��ǥ, z�� �� ��° �׸����ΰ�.


*/