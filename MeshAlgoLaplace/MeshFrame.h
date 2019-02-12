#pragma once
#include "MeshUtility.h"

class MeshFrame
{
public:
	vector<Vertex> VB;
	vector<int> boundaryBuffer;
	vector<int> freeBuffer;
	MeshFrame(vector<Vertex> _VB,vector<int> _boundaryBuffer);
};

