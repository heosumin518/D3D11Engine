#pragma once
///
/// ������ ������������ �����ϴ� Ŭ����
///	DeviceContext �� �ٽ� ���.
///


// ���������� ���ο��� '����'���� ����ϴ� ��ҵ�
// �Ʒ� ��ҵ��� ��� �����־�� �ϴ� ���� �ƴ϶� �̷��� ��ҵ��� �����Ͽ� ������������ �Ը´�� ������ �� �ִ�.
// Pipeline Ŭ������ '����' �� ģ�ٸ� PipelineInfo�� '��ź' �̶�� �� �� �ִ�.
// '����'�� '��ź'�� �Ը���� ���Ƴ����ִ� ��.
struct PipelineInfo
{
	shared_ptr<InputLayout> inputLayout;
	shared_ptr<VertexShader> vertexShader;
	shared_ptr<PixelShader> pixelShader;
	shared_ptr<RasterizerState> rasterizerState;
	shared_ptr<BlendState> blendState;
	D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	// constantBuffer �� ä���ִ� �κ��� Info �� ���� ������ ���������� ���� ������� ��Ȯ���� �ʱ� ��������.
	// �׷��� ������ �ʿ��ϸ� ��ü���� �����ϵ��� �Ѵ�. (SharedResource �� ��������.)
};

class Pipeline
{
public:
	Pipeline(ComPtr<ID3D11DeviceContext> deviceContext);
	~Pipeline();

	void UpdatePipeline(PipelineInfo info);

	// ���������� ���� �κе��� �ɼ��� �־ �׶��׶� ���Ƴ��� �� �ֵ��� �Լ��� �����д�.
	void SetVertexBuffer(shared_ptr<VertexBuffer> buffer);
	void SetIndexBuffer(shared_ptr<IndexBuffer> buffer);

	template <typename T>
	void SetConstantBuffer(uint32 slot, uint32 scope, shared_ptr<ConstantBuffer<T>> buffer)
	{
		// constantBuffer �� VS �ܰ迡���� ���̴� ���� �ƴ϶� PS �ܰ迡���� ���� �� �ֱ� ������ scope �� ����. bigflag ����� ����Ѵ�.
		if (scope & SS_VertexShader)
			_deviceContext->VSSetConstantBuffers(slot, 1, buffer->GetComPtr().GetAddressOf());

		if (scope & SS_PixelShader)
			_deviceContext->PSSetConstantBuffers(slot, 1, buffer->GetComPtr().GetAddressOf());
	}

	void SetTexture(uint32 slot, uint32 scope, shared_ptr<Texture> texture);
	void SetSamplerState(uint32 slot, uint32 scope, shared_ptr<SamplerState> samplerState);

	void Draw(uint32 vertexCount, uint32 startVertexLocation);
	void DrawIndexed(uint32 indexCount, uint32 startIndexLocation, uint32 baseVertexLocation);
private:
	ComPtr<ID3D11DeviceContext> _deviceContext;
};

inline void Pipeline::Draw(uint32 vertexCount, uint32 startVertexLocation)
{
	_deviceContext->Draw(vertexCount, startVertexLocation);
}

inline void Pipeline::DrawIndexed(uint32 indexCount, uint32 startIndexLocation, uint32 baseVertexLocation)
{
	_deviceContext->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);
}
