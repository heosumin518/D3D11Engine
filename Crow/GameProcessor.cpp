#include "pch.h"
#include "GameProcessor.h"
#include "WinApp.h"

GameProcessor::GameProcessor(const int32 &width, const int32 &height, const wstring &name)
	: m_width(width)
	, m_height(height)
	, m_title(name)
{
	
}

void GameProcessor::Initialize()
{
	// TO DO
	// ���⼭ �Ŵ��� ���� �� �ʱⰪ ����
}

void GameProcessor::Update()
{
	// �ð� ������Ʈ
	m_timer.Tick();

}

void GameProcessor::RenderBegin()
{
	//m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), nullptr);
	m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), m_clearColor);
	m_deviceContext->ClearDepthStencilView(m_depthStancilView.Get(), D3D11_CLEAR_DEPTH, 1.f, 0);	// �������� 1.0f�� �ʱ�ȭ
	m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStancilView.Get());
	m_deviceContext->RSSetViewports(1, &m_viewport);
}

void GameProcessor::RenderEnd()
{
	HR_T(m_swapChain->Present(1, 0));
}

// ��ġ ����, ����ü�� ����, ��ġ ���ؽ�Ʈ ����
void GameProcessor::CreateDeviceAndSwapChain()
{
	// ����ü�� �Ӽ� ���� ����ü ���� �� �ʱ�ȭ
	DXGI_SWAP_CHAIN_DESC swapDesc;
	ZeroMemory(&swapDesc, sizeof(swapDesc));	// swapDesc �� ��� ��������� 0���� �ʱ�ȭ
	{
		// �����(�ؽ�ó)�� ����/���� ũ�� ����.
		swapDesc.BufferDesc.Width = m_width;
		swapDesc.BufferDesc.Height = m_height;

		// ȭ�� �ֻ��� ����
		swapDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapDesc.BufferDesc.RefreshRate.Denominator = 1;

		swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		// ���ø� ���� ����
		swapDesc.SampleDesc.Count = 1;
		swapDesc.SampleDesc.Quality = 0;

		swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapDesc.BufferCount = 1;
		swapDesc.OutputWindow = WinApp::GetHandleWnd(); 	// ����ü�� ����� â �ڵ� ��
		swapDesc.Windowed = true;	// â��� ����
		swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	}

	UINT creationFlags = 0;

#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HR_T(::D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		creationFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&swapDesc,
		m_swapChain.GetAddressOf(),
		m_device.GetAddressOf(),
		nullptr,
		m_deviceContext.GetAddressOf()
	));
}

// ����۸� �̿��ϴ� ����Ÿ�ٺ� ����
void GameProcessor::CreateRenderTargetView()
{
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	HR_T(m_swapChain->GetBuffer( 0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf())));
	HR_T(m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.GetAddressOf()));
}

void GameProcessor::SetViewport()
{
	m_viewport.TopLeftX = 0.f;
	m_viewport.TopLeftY = 0.f;
	m_viewport.Width = static_cast<float>(m_width);
	m_viewport.Height = static_cast<float>(m_height);
	m_viewport.MinDepth = 0.f;
	m_viewport.MaxDepth = 1.f;
}

void GameProcessor::CreateDepthStencilView()
{
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	desc.Width = m_width;
	desc.Height = m_height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	ComPtr<ID3D11Texture2D> textureDepthStencil = nullptr;
	HR_T(m_device->CreateTexture2D(&desc, nullptr, textureDepthStencil.GetAddressOf()));

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	dsvDesc.Format = desc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	HR_T(m_device->CreateDepthStencilView(textureDepthStencil.Get(), &dsvDesc, m_depthStancilView.GetAddressOf()));
}

// Render() ���� ���������ο� ���ε��� ���� ���� �غ�
void GameProcessor::CreateGeometry()
{
	// VertexData �� �ڽ� Ŭ�������� ��������. TODO : ���� ������ �� �ʿ��ҵ�

	// VertexBuffer
	{
		// ���� ���� ���� ����
		D3D11_BUFFER_DESC vbDesc;
		ZeroMemory(&vbDesc, sizeof(vbDesc));
		vbDesc.Usage = D3D11_USAGE_DEFAULT;
		vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;		// vertex buffer�� ����ϴµ� �� ���̶�� �� �˷��ֱ�.
		vbDesc.ByteWidth = static_cast<uint32>(sizeof(Vertex) * m_vertices.size());
		vbDesc.CPUAccessFlags = 0;

		// ���� ���� ����
		D3D11_SUBRESOURCE_DATA vbData;
		ZeroMemory(&vbData, sizeof(vbData));
		vbData.pSysMem = m_vertices.data();		// �迭 ������ �Ҵ�.
		HR_T(m_device->CreateBuffer(&vbDesc, &vbData, m_vertexBuffer.GetAddressOf()));
	}

	// IndexBuffer
	{
		// �ε��� ���� ���� ����
		D3D11_BUFFER_DESC ibDesc;
		ZeroMemory(&ibDesc, sizeof(ibDesc));
		ibDesc.Usage = D3D11_USAGE_DEFAULT;
		ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibDesc.ByteWidth = static_cast<uint32>(sizeof(WORD) * m_indices.size());
		ibDesc.CPUAccessFlags = 0;

		// �ε��� ���� ����
		D3D11_SUBRESOURCE_DATA ibData;
		ZeroMemory(&ibData, sizeof(ibData));
		ibData.pSysMem = m_indices.data();		// �迭 ������ �Ҵ�.
		HR_T(m_device->CreateBuffer(&ibDesc, &ibData, m_indexBuffer.GetAddressOf()));
	}
}

void GameProcessor::SetTransformMatrix()
{
	// TODO : �̰� �θ𿡴� �س����� �ƴ� �ڽĿ��� �س�����
}

// Render() ���� ���������ο� ���ε��� InputLayout ���� 	
void GameProcessor::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC layout[] =  // ��ǲ ���̾ƿ��� ���ؽ� ���̴��� �Է¹��� �������� ������ �����Ѵ�.
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	const int32 count = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);
	HR_T(m_device->CreateInputLayout(layout, count, m_vsBlob->GetBufferPointer(), m_vsBlob->GetBufferSize(), m_inputLayout.GetAddressOf()));
}

// Render���� ���������ο� ���ε���  ���ؽ� ���̴� ����
void GameProcessor::CreateVertexShader()
{
	HR_T(CompileShaderFromFile(L"VertexShader.hlsl", "VS", "vs_5_0", m_vsBlob));
	HR_T(m_device->CreateVertexShader(m_vsBlob->GetBufferPointer(), m_vsBlob->GetBufferSize(), nullptr, m_vertexShader.GetAddressOf()));
}

// Render���� ���������ο� ���ε��� �ȼ� ���̴� ����
void GameProcessor::CreatePixelShader()
{
	HR_T(CompileShaderFromFile(L"PixelShader.hlsl", "PS", "ps_5_0", m_psBlob));
	HR_T(m_device->CreatePixelShader(m_psBlob->GetBufferPointer(), m_psBlob->GetBufferSize(), nullptr, m_pixelShader.GetAddressOf()));
}

void GameProcessor::CreateConstantBuffer()
{
	// ��� ���� ���� ����
	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(cbDesc));
	cbDesc.Usage = D3D11_USAGE_DEFAULT; // D3D11_USAGE_DYNAMIC;	// CPU_Write + GPU_Read �� �� �����ϰ� �������. �̰ɷ� �ٲٱ� .���� �ȳ���.
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// ��� ���� �뵵�� Ȱ��
	cbDesc.ByteWidth = sizeof(TransformData);
	cbDesc.CPUAccessFlags = 0;		// cpu �� ���ٰ����ϰ�.

	// ��� ���� ����
	HR_T(m_device->CreateBuffer(&cbDesc, nullptr, m_constantBuffer.GetAddressOf()));
}
