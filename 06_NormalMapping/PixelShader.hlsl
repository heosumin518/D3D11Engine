#include "Shared.fxh"

/**
 * \brief Pixel Shader
 * \param input 
 * \return 
 */

Texture2D diffuse0 : register(t0);
Texture2D normal0 : register(t1);
Texture2D specular0 : register(t2);
SamplerState sampler0 : register(s0);

float4 ps_main(VS_OUTPUT input) : SV_Target
{
    float3 worldNormal = normalize(input.N);
    float3 worldTangent = normalize(input.T);
    float3 worldBiTangent = cross(worldNormal, worldTangent);

    if (m_useNormalMap)
    {
	    // �븻 �� �ؽ�ó�κ��� �븻 �о���� (����: 0 ~ 1)
        float3 tangentNormal = normal0.Sample(sampler0, input.m_uv).rgb;
	    // ���� �븻 ������ ������ -1 ~ 1�� Ȯ���ϱ�
        tangentNormal = normalize(tangentNormal * 2.f - 1.f);

        // �븻 ���͸� ����������� ��ȯ�ϱ� ���� ������ȯ ��� �����
        float3x3 TBN = float3x3(worldTangent, worldBiTangent, worldNormal); // ���� ������ ���͸� ź��Ʈ �������� ��ȯ�ϴ� ���
        //TBN = transpose(TBN); // ���� �������� ��ȯ�ؾ��ϹǷ� ����� ���ϱ�
        //worldNormal = mul(TBN, tangentNormal); // ź��Ʈ ������ �븻 ���͸� ���� �������� ��ȯ

        worldNormal = mul(tangentNormal, TBN);

        // -> ������� ������ �ʴ´ٸ� ���ϴ� ������ �ٲپ� �־�� �Ѵ�. = mul(tangentNormal, TBN)
    }

    // ���� ���
    // 1. ���ݻ籤
    float3 lightDir = normalize(m_lightDir);
    float3 diffuse = saturate(dot(worldNormal, -lightDir)); // ���ݻ籤�� ��
    float4 albedo = diffuse0.Sample(sampler0, input.m_uv); // diffuse ��
    diffuse = m_lightDiffuse * m_materialDiffuse * albedo * diffuse; // ���� ���͸����� ����� �ؽ�ó�� ���� �׸��� ���ݻ籤�� ���� ���Ѵ�.

    // 2. ���ݻ籤
    float3 specular = 0;
    if (diffuse.x > 0)
    {
        float3 viewDir = normalize(input.m_viewDir);

        // ����ŧ�� ��
        float4 specularIntensity =  specular0.Sample(sampler0, input.m_uv);

        // �� ��
        float3 halfVector = normalize(lightDir + viewDir);
        specular = saturate(dot(halfVector, -worldNormal));
        specular = pow(specular, m_specularPower) * m_lightSpecular * m_materialSpecular;

        if (m_useSpecularMap)
            specular *= specularIntensity.rgb;
    }

    //float3 ambient = float3(0.1f, 0.1f, 0.1f);
    float3 ambient = m_lightAmbient * m_materialAmbient * albedo.rgb;

    // ���� ���� = �ֺ��� + ���ݻ籤 + ���ݻ籤
    float4 finalColor = float4(ambient + diffuse + specular, 1);

    return finalColor;
}
