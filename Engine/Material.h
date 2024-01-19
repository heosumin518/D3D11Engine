#pragma once
#include "ResourceBase.h"
#include "Texture.h"

namespace Engine
{
	enum MaterialMapFlags
	{
		BASECOLOR = 1 << 0,
		NORMAL = 1 << 1,
		SPECULAR = 1 << 2,
		EMISSIVE = 1 << 3,
		OPACITY = 1 << 4,
		METALNESS = 1 << 5,
		ROUGHNESS = 1 << 6
	};

	enum TEXTURE_TYPE_COUNT
	{
		
	};

	class Material : public ResourceBase
	{
		using Super = ResourceBase;

	public:
		Material();
		~Material();

		void Create(const aiMaterial* material);

		shared_ptr<Texture> m_baseColorMap;
		shared_ptr<Texture> m_normalMap;
		shared_ptr<Texture> m_specularMap;
		shared_ptr<Texture> m_emissiveMap;
		shared_ptr<Texture> m_opacityMap;
		shared_ptr<Texture> m_matelnessMap;
		shared_ptr<Texture> m_roughnessMap;

		UINT m_materialMapFlags = 0;
	};
}
