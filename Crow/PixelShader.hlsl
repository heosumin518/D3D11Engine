#include "Shared.fxh"

float4 PS(VS_OUTPUT input) : SV_Target
{
	// ���⼭ �ȼ����� ���� ���� ó���� �� ����..

	return input.color;
}