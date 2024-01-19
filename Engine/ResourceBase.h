#pragma once
#include "Types.h"

namespace Engine
{
	enum class ResourceType : uint8
	{
		None = -1,
		StaticMesh,
		SkeletalMesh,
		Texture,
		Material,
		Animation,

		End
	};

	enum
	{
		RESOURCE_TYPE_COUNT = static_cast<uint8>(ResourceType::End)
	};

	class ResourceBase
	{
	public:
		ResourceBase(ResourceType type);
		virtual ~ResourceBase();

		ResourceType GetType() { return m_type; }

		const string& GetName() { return m_name; }

	protected:
		virtual void Load(const wstring& path) { }
		//virtual void Save(const wstring& path) { }

	protected:
		ResourceType m_type;
		string m_name;
	};
}
