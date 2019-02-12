#include "stdafx.h"
#include <algorithm>
#include "MeshFrame.h"

MeshFrame::MeshFrame(vector<Vertex> _VB, vector<int> _boundaryBuffer) :VB(_VB), boundaryBuffer(_boundaryBuffer) {
	for (int i = 0; i < int(VB.size()); i++) {
		if (std::find(boundaryBuffer.begin(), boundaryBuffer.end(), i) == boundaryBuffer.end())
		{
			freeBuffer.push_back(i);
		}
	}
};