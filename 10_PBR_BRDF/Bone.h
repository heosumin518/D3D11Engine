#pragma once

class Node;
struct Bone
{
	string name;
	Matrix offsetMatrix;
	unsigned int numWeights;
	shared_ptr<Node> owner;
};
