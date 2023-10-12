#include "pch.h"
#include "RenderCube.h"

#include "WinApp.h"

RenderCube::RenderCube(const int32& width, const int32& height, const std::wstring& name)
	: GameProcessor(width, height, name)
{
}

RenderCube::~RenderCube()
{
	GameProcessor::UnInitImGUI();
}

void RenderCube::Initialize()
{
	GameProcessor::CreateDeviceAndSwapChain();
	GameProcessor::CreateRenderTargetView();
	GameProcessor::SetViewport();
	GameProcessor::CreateDepthStencilView();

	CreateGeometry();
	GameProcessor::CreateVertexShader();
	CreateInputLayout();
	GameProcessor::CreatePixelShader();

	CreateConstantBuffer();

	GameProcessor::InitImGUI();
}

void RenderCube::Update()
{
	GameProcessor::Update();

	auto t = m_timer.TotalTime();

	// set camera
	{
		m_eye = XMVectorSet(m_cameraPos[0], m_cameraPos[1], m_cameraPos[2], 0.0f);
		m_at = XMVectorSet(m_cameraPos[0], m_cameraPos[1] + 1.0f, 100.0f, 0.0f);
		m_up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

		m_view = XMMatrixLookAtLH(m_eye, m_at, m_up);		// ViewTransform ��� ���ϱ�. XMMatrixLookToLH() �Լ��ε� ���� �� ����
		m_projection = XMMatrixPerspectiveFovLH(m_cameraFOV / 180.0f * 3.14f , g_winSizeX / static_cast<FLOAT>(g_winSizeY), m_cameraNear, m_cameraFar);
	}

	// set Sun, Earth, Moon
	{
		// 1st Cube: Rotate around the origin
		Matrix sunSpin = Matrix::CreateRotationY(-t);
		Matrix translation1 = Matrix::CreateTranslation(m_sunPos[0], m_sunPos[1], m_sunPos[2]); //imgui �� ����
		m_world1 = sunSpin * translation1;

		// 2nd Cube: Rotate around origin
		Matrix earthSpin = Matrix::CreateRotationY(t * 7.0f);
		//Matrix earthTranslate = Matrix::CreateTranslation(-4.0f, 0.0f, 0.0f);
		Matrix earthTranslate = Matrix::CreateTranslation(m_earthPos[0], m_earthPos[1], m_earthPos[2]);
		Matrix earthScale = Matrix::CreateScale(0.3f, 0.3f, 0.3f);
		m_world2 = earthScale * earthSpin * earthTranslate * m_world1;

		// 3rd
		Matrix moonSpin = Matrix::CreateRotationY(-t * 6.0f);
		Matrix moonTranslate = Matrix::CreateTranslation(m_moonPos[0], m_moonPos[1], m_moonPos[2]);
		Matrix moonScale = Matrix::CreateScale(0.3f, 0.3f, 0.3f);
		m_world3 = moonScale * moonSpin * moonTranslate * m_world2;
	}

	// ���� ����� ��ġ��ķ� ������ �Ѵ�. // dx������ ���궧��.
	// �������� transpose �Լ��� ��ġ�� �ѹ��� ����. ���� ���� ��������.
	{
		m_transformData1.world = XMMatrixTranspose(m_world1);				
		m_transformData1.view = XMMatrixTranspose(m_view);
		m_transformData1.projection = XMMatrixTranspose(m_projection);

		m_transformData2.world = XMMatrixTranspose(m_world2);				
		m_transformData2.view = XMMatrixTranspose(m_view);
		m_transformData2.projection = XMMatrixTranspose(m_projection);

		m_transformData3.world = XMMatrixTranspose(m_world3);				
		m_transformData3.view = XMMatrixTranspose(m_view);
		m_transformData3.projection = XMMatrixTranspose(m_projection);
	}
}

void RenderCube::Render()
{
	RenderBegin();

	// IA - VS - RS - PS - OM
	{
		uint32 stride = sizeof(Vertex);
		uint32 offset = 0;

		// IA
		m_deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
		m_deviceContext->IASetInputLayout(m_inputLayout.Get());
		m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);	// INDEX���� ����

		// VS
		m_deviceContext->VSSetShader(m_vertexShader.Get(), nullptr, 0);
		m_deviceContext->VSSetConstantBuffers(0, 1, m_transformBuffer.GetAddressOf());

		// RS

		// PS
		m_deviceContext->PSSetShader(m_pixelShader.Get(), nullptr, 0);

		// OM
		// Render Sun, Earth, Moon
		m_deviceContext->UpdateSubresource(m_transformBuffer.Get(), 0, nullptr, &m_transformData1, 0, 0);
		m_deviceContext->DrawIndexed(m_indices.size(), 0, 0);

		m_deviceContext->UpdateSubresource(m_transformBuffer.Get(), 0, nullptr, &m_transformData2, 0, 0);
		m_deviceContext->DrawIndexed(m_indices.size(), 0, 0);

		m_deviceContext->UpdateSubresource(m_transformBuffer.Get(), 0, nullptr, &m_transformData3, 0, 0);
		m_deviceContext->DrawIndexed(m_indices.size(), 0, 0);
	}

	RenderImGUI();

	RenderEnd();
}

void RenderCube::CreateGeometry()
{
	// Vertex Data �غ�
	// Render()���� ���������ο� ���ε��� ���ؽ� ���� �غ�
	m_vertices.resize(8);

	m_vertices = 
	{
		{ Vector3(-1.0f, 1.0f, -1.0f),	Color(0.0f, 0.0f, 1.0f, 1.0f) },
		{ Vector3(1.0f, 1.0f, -1.0f),	Color(0.0f, 1.0f, 0.0f, 1.0f) },
		{ Vector3(1.0f, 1.0f, 1.0f),	Color(0.0f, 1.0f, 1.0f, 1.0f) },
		{ Vector3(-1.0f, 1.0f, 1.0f),	Color(1.0f, 0.0f, 0.0f, 1.0f) },
		{ Vector3(-1.0f, -1.0f, -1.0f), Color(1.0f, 0.0f, 1.0f, 1.0f) },
		{ Vector3(1.0f, -1.0f, -1.0f),	Color(1.0f, 1.0f, 0.0f, 1.0f) },
		{ Vector3(1.0f, -1.0f, 1.0f),	Color(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vector3(-1.0f, -1.0f, 1.0f),	Color(0.0f, 0.0f, 0.0f, 1.0f) },
	};

	// Index Data
	// Render()���� ���������ο� ���ε��� �ε��� ���� ����
	m_indices =
	{
		3,1,0, 2,1,3,
		0,5,4, 1,5,0,
		3,4,7, 0,4,3,
		1,6,5, 2,6,1,
		2,7,6, 3,7,2,
		6,4,5, 7,4,6,
	};

	// Vertex Buffer, Index Buffer ���� �غ�.
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

// Render() ���� ���������ο� ���ε��� InputLayout ���� 	
void RenderCube::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC layout[] =  // ��ǲ ���̾ƿ��� ���ؽ� ���̴��� �Է¹��� �������� ������ �����Ѵ�.
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	const int32 count = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);
	HR_T(m_device->CreateInputLayout(layout, count, m_vsBlob->GetBufferPointer(), m_vsBlob->GetBufferSize(), m_inputLayout.GetAddressOf()));
}

void RenderCube::CreateConstantBuffer()
{
	// ��� ���� ���� ����
	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(cbDesc));
	cbDesc.Usage = D3D11_USAGE_DEFAULT; // D3D11_USAGE_DYNAMIC;	// CPU_Write + GPU_Read �� �� �����ϰ� �������. �̰ɷ� �ٲٱ� .���� �ȳ���.
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// ��� ���� �뵵�� Ȱ��
	cbDesc.ByteWidth = sizeof(TransformData);
	cbDesc.CPUAccessFlags = 0;		// cpu �� ���ٰ����ϰ�.

	// ��� ���� ����
	HR_T(m_device->CreateBuffer(&cbDesc, nullptr, m_transformBuffer.GetAddressOf()));
}

void RenderCube::RenderImGUI()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// camera control window
	{
		ImGui::SetNextWindowPos(ImVec2(525, 10));
		ImGui::SetNextWindowSize(ImVec2(500, 200));		// �޴� â ũ�� ����
		ImGui::Begin("Camera Control Panel");

		ImGui::Text("Adjust camera position");
		ImGui::SliderFloat3("Camera (x, y, z)", m_cameraPos, -10.0f, 10.0f);
		ImGui::SliderFloat("FOV", &m_cameraFOV, 0.01f, 180.0f);
		ImGui::SliderFloat("Near", &m_cameraNear, 0.01f, 10.0f);
		ImGui::SliderFloat("Far", &m_cameraFar, 1.f, 100.0f);

		ImGui::End();
	}

	// cube control window
	{
		ImGui::SetNextWindowPos(ImVec2(10, 10));
		ImGui::SetNextWindowSize(ImVec2(500, 120));		// �޴� â ũ�� ����
		ImGui::Begin("Cube Control Panel");

		ImGui::Text("Adjust the planets positions");
		ImGui::SliderFloat3("Sun (x, y, z)", m_sunPos, -5.0f, 5.0f);
		ImGui::SliderFloat3("Earth (x, y, z)", m_earthPos, -5.0f, 5.0f);
		ImGui::SliderFloat3("Moon (x, y, z)", m_moonPos, -5.0f, 5.0f);

		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

