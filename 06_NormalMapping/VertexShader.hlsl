#include "Shared.fxh"

/**
 * \brief Vertex Shader
 * \param input 
 * \return 
 */

VS_OUTPUT vs_main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;

    // ���� ��ġ�� ���� �������� ��ȯ
    output.m_position = mul(input.m_position, m_world);
    output.m_uv = input.m_uv; // �ؽ�ó ��ǥ

    // ī�޶� ���� ���ϱ�
	float3 viewDir = normalize(output.m_position.xyz - m_eyePos.xyz); // �ü� ����(ī�޶� ����) // �ü� ��ġ(ī�޶� ��ġ)�κ��� ������ġ������ ���⺤��
    output.m_viewDir = viewDir;

    // ���� ��ġ�� ��, �������� �������� ��ȯ
    output.m_position = mul(output.m_position, m_view);
    output.m_position = mul(output.m_position, m_projection);

    // �븻, ź��Ʈ ���͸� ����������� ��ȯ�Ͽ� �ȼ����̴��� ���ϱ�
    float3 worldNormal = mul(input.m_normal, (float3x3) m_world); // ���� ���� // �Է����� ���� ���� ������ �ִ� ������ ���� �������� ��ȯ�Ѵ�.
    output.N = normalize(worldNormal);

    float3 worldTangent = mul(input.m_tangent, (float3x3) m_world);
    output.T = normalize(worldTangent);

    return output;
}
/*
 * �� �ڵ忡�� ��������� float3x3 �� ĳ������ �Ͽ��µ�,
 * 4x4��Ŀ��� 4��° �� �Ǵ� ���� �����̵� ���̹Ƿ� ���⺤�Ϳ��� �ƹ��� ���⵵ ��ġ�� �ʴ´�.
 */

/*
 * ������ ����� ��� �ʿ����� �� �� �ִٸ� �ȼ����̴� ���� �������̴����� �ϴ� ���� ���ɻ� �����ϴ�.
 * 3D ������ �� ���� ��� �������� �����ϴ� ������ ��ġ�ؾ� �Ѵ�.
*/

/*
 * BiTangent�� CrossProduct ��� ������ ���������ϴ�.
*/