#include "pch.h"
#include "RasterizerState.h"

RasterizerState::RasterizerState(ComPtr<ID3D11Device> device)
	: _device(device)
{
}

RasterizerState::~RasterizerState()
{
}

void RasterizerState::Create()
{
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.FillMode = D3D11_FILL_SOLID;	// �ﰢ�� ���� ä�� ������ �ƴϸ� �����Ӹ� ���̰� �� ������ ����.
	desc.CullMode = D3D11_CULL_BACK;	// ��� ���� �׸��� ���� ������ ����.     // D3D11_CALL_BACK : �ĸ��� �׸��� �ʵ��� ����.
	desc.FrontCounterClockwise = false;	// �̰ɷ� �� �ڸ� �Ǵ�. false �� �ð������ �չ������� ����.

	HRESULT hr = _device->CreateRasterizerState(&desc, _rasterizerState.GetAddressOf());
	CHECK(hr);
}
