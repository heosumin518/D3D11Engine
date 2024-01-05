#include "Shared.hlsli"

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

static const float PI = 3.141592;
static const float Epsilon = 0.00001;

// Constant normal incidence Fresnel factor for all dielectrics.
static const float3 Fdielectric = 0.04;

// GGX/Towbridge-Reitz normal distribution function.
// Uses Disney's reparametrization of alpha = roughness^2.
float ndfGGX(float cosLh, float roughness)
{
    float alpha = roughness * roughness;
    float alphaSq = alpha * alpha;

    float denom = (cosLh * cosLh) * (alphaSq - 1.0) + 1.0;
    return alphaSq / (PI * denom * denom);
}

// Single term for separable Schlick-GGX below.
float gaSchlickG1(float cosTheta, float k)
{
    return cosTheta / (cosTheta * (1.0 - k) + k);
}

// Schlick-GGX approximation of geometric attenuation function using Smith's method.
float gaSchlickGGX(float cosLi, float cosLo, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0; // Epic suggests using this roughness remapping for analytic lights.
    return gaSchlickG1(cosLi, k) * gaSchlickG1(cosLo, k);
}

// Shlick's approximation of the Fresnel factor.
float3 fresnelSchlick(float3 F0, float cosTheta)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float4 ps_main(PS_INPUT input) : SV_Target
{
    float3 vNormal = normalize(input.NormalWorld);
    float3 vTangent = normalize(input.TangentWorld);
    float3 vBiTanget = cross(vNormal, vTangent);
    
    if (UseNormalMap)
    {
        // ��� ���� ���ø��Ͽ� ��� ���͸� �����´�.
        float3 vNormalTangentSpace = txNormal.Sample(samLinear, input.TexCoord).rgb * 2.0f - 1.0f;

        // ��� �ʿ��� ������ ���͸� ��� �� ��ǥ�迡�� ���� ��ǥ��� ��ȯ�Ѵ�.
        float3x3 WorldTransform = float3x3(vTangent, vBiTanget, vNormal);
        vNormal = mul(vNormalTangentSpace, WorldTransform);

        // ��� ���� ����ȭ
        vNormal = normalize(vNormal);
    }

    // Sample input textures to get shading model params.
    float4 albedo = txDiffuse.Sample(samLinear, input.TexCoord);
    float metalness = 0.0f;
    if (UseMetalnessMap)
        metalness = txMatalness.Sample(samLinear, input.TexCoord).r;

    float roughness = 0.0f;
    if (UseRoughnessMap)
        roughness = txRoughness.Sample(samLinear, input.TexCoord).r;

    float3 emissive = 0.0f;
    if (UseEmissiveMap)
        emissive = txEmissive.Sample(samLinear, input.TexCoord).rgb;

    float Opacity = 1.0f;
    if (UseOpacityMap)
        Opacity = txOpacity.Sample(samLinear, input.TexCoord).a;

	// Outgoing light direction (vector from world-space fragment position to the "eye").
    float3 Lo = normalize(EyePosition - input.PositionWorld);

	// Get current fragment's normal and transform to world space.
    float3 N = vNormal;

	// Angle between surface normal and outgoing light direction.
    float cosLo = max(0.0, dot(N, Lo));

	// Specular reflection vector.
    float3 Lr = 2.0 * cosLo * N - Lo;

	// Fresnel reflectance at normal incidence (for metals use albedo color).
    float3 F0 = lerp(Fdielectric, albedo, metalness);
	// Direct lighting calculation for analytical lights.
    float3 directLighting = 0.0;

    float3 Li = -LightDirection;
    float3 Lradiance = LightRadiance;

    // Half-vector between Li and Lo.
    float3 Lh = normalize(Li + Lo);

    // Calculate angles between surface normal and various light vectors.
    float cosLi = max(0.0, dot(N, Li));
    float cosLh = max(0.0, dot(N, Lh));

    // Calculate Fresnel term for direct lighting.
    float3 F = fresnelSchlick(F0, max(0.0, dot(Lh, Lo)));   // �ּҰ� F0, �ִ밪�� 1.0, 1.0, 1.0

    // Calculate normal distribution for specular BRDF.
    float D = ndfGGX(cosLh, max(0.01, roughness));  // �����Ͻ��� 0�� �Ǹ� ���� 0�� �ǹǷ� 0�̸� �ּҰ��� ���
    // Calculate geometric attenuation for specular BRDF.
    float G = gaSchlickGGX(cosLi, cosLo, roughness);

    // �ݼӼ��� 1�̸� ħ���� �����Ƿ� ǥ������ �Ͼ�� �ʾ� ǥ������ ������ 0, 0, 0, ���� ó���Ѵ�.
    // �ݼӼ��� 0�̸� �ݻ��� ������ ������ŭ ǥ������ ���� ǥ���� �����ϴ�.
    // �ݼӼ��� 1�ȼ��� �ش�Ǵ� �̼� ǥ���� ������ �ǹ��ϹǷ� ������ ����Ͽ� �߰����� ó���Ѵ�.
    float3 kd = lerp(float3(1, 1, 1) - F, float3(0, 0, 0), metalness);

    // Lambert diffuse BRDF.
    // We don't scale by 1/PI for lighting & material units to be more convenient.
	// See: https://seblagarde.wordpress.com/2012/01/08/pi-or-not-to-pi-in-game-lighting-equation/
    float3 diffuseBRDF = kd * albedo / PI;

    // Cook-Torrance specular microfacet BRDF.
    float3 specularBRDF = (F * D * G) / max(Epsilon, 4.0 * cosLi * cosLo);

    // Total contribution for this light.
    directLighting += (diffuseBRDF + specularBRDF) * Lradiance * cosLi;

    float3 ambientLighting = 0;
    // IBL �ڵ� �ۼ�.. ambient..

    float3 final = directLighting + ambientLighting + emissive;
    float3 GammaCorrect = pow(final, float(1.0 / 2.2).rrr);
    float3 output = GammaCorrect;

    return float4(output, Opacity);
}
