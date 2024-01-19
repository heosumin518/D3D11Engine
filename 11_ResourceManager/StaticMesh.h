#pragma once
#include "Mesh.h"
#include "ResourceBase.h"

class StaticMesh : public Mesh, public ResourceBase
{
	using Super = ResourceBase;

public:
	StaticMesh();
	~StaticMesh();

private:

};

