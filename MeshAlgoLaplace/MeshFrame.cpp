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
	vector<double> wVec;
	double allW;
	for (int i = 0; i < int(freeBuffer.size()); i++) {
		//清零，为下次准备
		tTrans = TransformQR();
		wVec.clear();
		allW = 0;
		for (int j = 0; j < int(boundaryBuffer.size()); j++) {
			double tdis = (VB[boundaryBuffer[j]] - VB[freeBuffer[i]]).Size();
			//?? 本人的灵性改编，否则距离影响权重不明显
			tdis = pow(tdis,50) ;
			double w = 1 / tdis;
			allW += w;
			wVec.push_back(w);
			//tTrans += _boundaryTransBuffer[j] * w;
		}
		for (int j = 0; j < int(boundaryBuffer.size()); j++) {
			tTrans += boundaryTransBuffer[j] * (wVec[j] / allW);
		}
		freeTransBuffer.push_back(tTrans);
	}

	//至此所有free顶点受传播的transQR已计算完成，保存在freeTransBuffer
	
	//先试一下，改变VB，输出一下顶点
	for (int i = 0; i < int(boundaryBuffer.size()); i++) {
		auto& tVertex = VB[boundaryBuffer[i]];
		tVertex.Set(Transform(boundaryTransBuffer[i], tVertex));
	}

	for (int i = 0; i < int(freeBuffer.size()); i++) {
		auto& tVertex = VB[freeBuffer[i]];
		tVertex.Set(Transform(freeTransBuffer[i], tVertex));
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