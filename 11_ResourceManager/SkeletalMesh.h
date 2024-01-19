#pragma once
#include "Mesh.h"
#include "ResourceBase.h"

class SkeletalMesh : public Mesh, public ResourceBase
{
	using Super = ResourceBase;

public:
	SkeletalMesh();
	~SkeletalMesh();


private:

};

