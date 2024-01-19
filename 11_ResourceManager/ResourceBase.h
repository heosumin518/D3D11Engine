#pragma once
#include "pch.h"

enum class ResourceType : uint8_t
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
	RESOURCE_TYPE_COUNT = static_cast<uint8_t>(ResourceType::End)
};

class ResourceBase
{
public:
	ResourceBase(ResourceType type);
	virtual ~ResourceBase();

	ResourceType GetType() { return m_type; }

	//void SetName(const wstring& name) { m_name = name; }
	//const wstring& GetName() { return m_name; }
	uint32_t GetID() const { return m_id; }

protected:
	virtual void Load(const wstring& path) {}
	//virtual void Save(const wstring& path) {}

protected:
	ResourceType m_type = ResourceType::None;
	//wstring m_name;
	wstring m_path;
	uint32_t m_id = 0;
};

