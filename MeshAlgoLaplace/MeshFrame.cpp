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
	//�����㷨����ʹ�����Դ���

	//����Ϊ����㷨
	/*for (int i = 0; i < freeBuffer.size(); i++) {
		minBuffer.push_back( FindNearestBoundaryVertex(freeBuffer[i]) );
	}*/

	//���Դ���
	//tf=sum(w*t[i])
	//w=1/dist(vf,vboundary[i])
	TransformQR tTrans;
	vector<double> wVec;
	double allW;
	for (int i = 0; i < int(freeBuffer.size()); i++) {
		//���㣬Ϊ�´�׼��
		tTrans = TransformQR();
		wVec.clear();
		allW = 0;
		for (int j = 0; j < int(boundaryBuffer.size()); j++) {
			double tdis = (VB[boundaryBuffer[j]] - VB[freeBuffer[i]]).Size();
			//?? ���˵����Ըı࣬�������Ӱ��Ȩ�ز�����
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

	//��������free�����ܴ�����transQR�Ѽ�����ɣ�������freeTransBuffer
	
	//����һ�£��ı�VB�����һ�¶���
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