#include "Shared.fxh"

/**
 * \brief Vertex Shader
 * \param input 
 * \return 
 */

VS_OUTPUT vs_main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.m_position = mul(input.m_position, m_world);

    // �Ի籤 ���Ϳ� ī�޶� ���� ���ϱ�
    float3 lightDir = normalize(m_lightDir);    // �Ի籤 ���� // ������ ���̸� 1�� ����� -> ����ȭ(normalize)
    float3 viewDir = normalize(output.m_position.xyz - m_eyePos.xyz);    // �ü� ���� // �ü� ��ġ�κ��� ������ġ������ ���⺤��
    output.m_viewDir = viewDir;

    output.m_position = mul(output.m_position, m_view);
    output.m_position = mul(output.m_position, m_projection);

    float3 worldNormal = mul(input.m_normal, (float3x3) m_world);   // ���� ���� // �Է����� ���� ���� ������ �ִ� ������ ���� �������� ��ȯ�Ѵ�.
    worldNormal = normalize(worldNormal);
    output.m_worldNormal = worldNormal; // ����..
	// ��������� float3x3 �� ĳ������ �Ͽ��µ�, 4x4��Ŀ��� 4��° �� �Ǵ� ���� �����̵� ���̹Ƿ� ���⺤�Ϳ��� �ƹ��� ���⵵ ��ġ�� �ʴ´�.

    output.m_diffuse = dot(-lightDir, worldNormal); // �� ������ ������ ���� �� ȭ��ǥ�� �ص��� ���� ������ ���� lightDir �� ���������� �ξ���.

    if (m_useBlinnPhong == true)
    {
		float3 halfDir = normalize(lightDir + viewDir);     // ī�޶� ���Ϳ� �Ի籤������ �߰��� �ǹ��ϴ� ���� ����
        output.m_reflection = halfDir;
    }
    else
    {
		float3 reflectionDir = reflect(lightDir, worldNormal);  // ���ݻ籤�� ���⺤��
        output.m_reflection = reflectionDir;
    }

    output.m_uv = input.m_uv;   // �ؽ�ó

    return output;
}

/*
 * ������ ����� ��� �ʿ����� �� �� �ִٸ� �ȼ����̴� ���� �������̴����� �ϴ� ���� ���ɻ� �����ϴ�.
 * 3D ������ �� ���� ��� �������� �����ϴ� ������ ��ġ�ؾ� �Ѵ�.
*/