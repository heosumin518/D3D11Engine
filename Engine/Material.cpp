#include "pch.h"
#include "Material.h"

Engine::Material::Material() : Super(ResourceType::Material)
{
}

Engine::Material::~Material()
{
}

void Engine::Material::Create(const aiMaterial* material)
{
	aiString texturePath;
	wstring basePath = L"../Resources/";
	filesystem::path path;
	wstring finalPath;

	m_name = material->GetName().C_Str();

	if (AI_SUCCESS == material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath))
	{
		path = ToWString(string(texturePath.C_Str()));
		finalPath = basePath + path.filename().wstring();
		m_baseColorMap = RESOURCES->Load<Texture>(finalPath);
	}
}
