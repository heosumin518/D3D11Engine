#include "Shared.fxh"

/**
 * \brief Pixel Shader
 * \param input 
 * \return 
 */

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

float4 ps_main(VS_OUTPUT input) : SV_Target
{
    float4 textureDiffuse = texture0.Sample(sampler0, input.m_uv);

    float3 diffuse = saturate(input.m_diffuse) * textureDiffuse * m_lightDiffuse;

    float3 reflection = normalize(input.m_reflection);
    float3 viewDir = normalize(input.m_viewDir);
    // VS ���� �̹� �������ͷ� ������� ���͵��� �ٽ� ����ȭ ���ִ� ������
    // �����⸦ ��ġ�� ���� �� ���� ��Ʈ���� �� �ֱ� �����̴�.

    float3 specular = 0;

    if (diffuse.x > 0)  // ���ݻ籤 ���� 0% �̻��� ������ ���ݻ籤�� ����Ѵ�.
    {                   // ���ݻ籤�� �������� �ʴ� ǥ�鿡�� �̹� ���� ���� �����Ƿ� ���ݻ籤�� ������ �� ����.
        if (m_useBlinnPhong == true)
        {
    		specular = saturate(dot(reflection, -input.m_worldNormal));
        }
        else
        {
    		specular = saturate(dot(reflection, -viewDir));
        }
        specular = pow(specular, m_lightSpecularPower) * m_lightSpecular;
    }

    //float3 ambient = float3(0.1f, 0.1f, 0.1f);
    float3 ambient = m_lightAmbient;

    // ���� ���� = �ֺ��� + ���ݻ籤 + ���ݻ籤
    float4 finalColor = float4(ambient + diffuse + specular, 1);

    return finalColor;
}