#pragma once
#include "MeshUtility.h"

class MeshFrame
{
public:
	vector<Vertex> VB;
	vector<int> boundaryBuffer, freeBuffer,minBuffer;
	vector<TransformQR> boundaryTransBuffer,freeTransBuffer;

	MeshFrame(vector<Vertex> _VB,vector<int> _boundaryBuffer);
	void SetBoundaryTransform(vector<TransformQR> _boundaryTransBuffer);
	int FindNearestBoundaryVertex(int freeInx);
};

