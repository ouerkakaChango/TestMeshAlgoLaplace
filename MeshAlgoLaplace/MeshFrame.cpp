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

void MeshFrame::SetBoundaryTransform(vector<TransformQR> _boundaryTransBuffer) {
	boundaryTransBuffer = _boundaryTransBuffer;
	//传播算法，先使用线性传播

	//本来为最近算法
	/*for (int i = 0; i < freeBuffer.size(); i++) {
		minBuffer.push_back( FindNearestBoundaryVertex(freeBuffer[i]) );
	}*/

	//线性传播
	//tf=sum(w*t[i])
	//w=1/dist(vf,vboundary[i])
	TransformQR tTrans;
	for (int i = 0; i < int(freeBuffer.size()); i++) {
		for (int j = 0; j < int(boundaryBuffer.size()); j++) {
			double tdis = (VB[boundaryBuffer[j]] - VB[freeBuffer[i]]).Size();
			tTrans += _boundaryTransBuffer[j] * (1 / tdis);
		}
		freeTransBuffer.push_back(tTrans);
		tTrans = TransformQR();
	}
}

int MeshFrame::FindNearestBoundaryVertex(int freeInx) {
	Vertex vFree = VB[freeInx];
	int minInx = -1;
	double minDis=1000000;
	double tDis;
	for (int i = 0; i < int(boundaryBuffer.size()); i++) {
		tDis = (VB[boundaryBuffer[i]] - vFree).Quadrance();
		if (tDis < minDis) {
			minDis = tDis;
			minInx = i;
		}
	}
	if (minInx == -1) {
		throw;
		return -1;
	}
	return boundaryBuffer[minInx];
}