// MeshAlgoLaplace.cpp: 定义控制台应用程序的入口点。
//参考 https://www.docin.com/p-976236736.html
//https://www.ljll.math.upmc.fr/frey/papers/meshing/Yu%20Y.,%20Mesh%20editing%20with%20Poisson%20based%20gradient%20field%20manipulation.pdf

#include "stdafx.h"
#include "windows.h"
#include <iostream>
#include <fstream>
#include <string>
#include "MeshUtility.h"
#include "MeshFrame.h"

using std::cout;
using std::endl;
using std::ofstream;
using std::string;

extern vector<Vertex> VB;

void PrintVertexBuffer(const vector<Vertex>& _arr) {
	int i = 1;
	for (auto& iter : _arr)
	{
		cout <<i<<" "<< iter.x << " " << iter.y << " " << iter.z << endl;
		i++;
	}
}

void SaveVertexBuffer(const vector<Vertex>& _arr,string filepath) {
	ofstream f1(filepath);
	for (int i = 0; i < _arr.size(); i++) {
		f1 << _arr[i].x << " " << _arr[i].y << " " << _arr[i].z;
		if (i != _arr.size() - 1) {
			f1 << endl;
		}
	}
}

Vertex Tri(int _inx) {
	if (_inx == 0) {
		return Vertex(0, 0, 0);
	}
	else if (_inx == 1) {
		return Vertex(0, 1, 0);
	}
	else if (_inx == 2) {
		return Vertex(1, 1, 0);
	}
	else if (_inx == 3) {
		return Vertex(1, 0, 0);
	}
	else {
		throw;
		return Vertex(0, 0, 0);
	}
}

vector<Vec3> GetLaplaceBufferOnMesh(vector<Vertex>& _VB);

vector<Vec3> GetDivOfGradBufferOnMesh(vector<Vertex>& _VB);

int main()
{
	vector<Triangle> TB;

	//uv空间思考
	//构建点，里横外竖
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			VB.push_back(Tri(j) * 10 + Vertex(0, 0, 10)*i);
		}
	}

	//构建三角形，并将vertex中的临近顶点和包含三角形都录入
	//里横外竖,不包含最上面一排顶点 
	//3 2
	//0 1
	//三角形023,012
	int i0, i1, i2, i3;
	int tTriInx;
	tTriInx = -1;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			i0 = j + 4 * i;
			i1 = (j + 1) % 4 + 4 * i;
			i2 = (j + 1) % 4 + 4 * (i + 1);
			i3 = j + 4 * (i + 1);
			VB[i0].neighbors.push_back(i1);
			VB[i0].neighbors.push_back(i2);
			VB[i0].neighbors.push_back(i3);
			VB[i1].neighbors.push_back(i0);
			VB[i1].neighbors.push_back(i2);
			VB[i1].neighbors.push_back(i3);
			VB[i2].neighbors.push_back(i1);
			VB[i2].neighbors.push_back(i0);
			VB[i2].neighbors.push_back(i3);
			VB[i3].neighbors.push_back(i1);
			VB[i3].neighbors.push_back(i2);
			VB[i3].neighbors.push_back(i0);
			TB.push_back(Triangle(i0, i2, i3));
			tTriInx += 1;
			VB[i0].neighborTris.push_back(TB[tTriInx]);
			VB[i2].neighborTris.push_back(TB[tTriInx]);
			VB[i3].neighborTris.push_back(TB[tTriInx]);
			TB.push_back(Triangle(i0, i1, i2));
			tTriInx += 1;
			VB[i0].neighborTris.push_back(TB[tTriInx]);
			VB[i1].neighborTris.push_back(TB[tTriInx]);
			VB[i2].neighborTris.push_back(TB[tTriInx]);
		}
	}

	SaveVertexBuffer(VB, "d:\\pcl0.txt");

	vector<Vec3> LapBuffer = GetLaplaceBufferOnMesh(VB);

	vector<Vec3> DivOfGradBuffer = GetDivOfGradBufferOnMesh(VB);
	/////////////////////////////////////
	Vec3 a(1, 0, 0), b(1, 1, 1),c;

	c = Transform(TransformQR(a, b),a);
	/////////////////////////////////////
	vector<int> boundaryBuffer;
	int endInx = VB.size() - 1;
	boundaryBuffer.push_back(endInx - 3);
	boundaryBuffer.push_back(endInx - 2);
	boundaryBuffer.push_back(endInx - 1);
	boundaryBuffer.push_back(endInx);

	//逆时针旋转最上层90度
	vector<TransformQR> boundaryTransBuffer;
	boundaryTransBuffer.push_back(TransformQR(VB[endInx - 3], Vec3(0, 10, 50)));
	boundaryTransBuffer.push_back(TransformQR(VB[endInx - 2], Vec3(10, 10, 50)));
	boundaryTransBuffer.push_back(TransformQR(VB[endInx - 1], Vec3(10, 0, 50)));
	boundaryTransBuffer.push_back(TransformQR(VB[endInx], Vec3(0, 0, 50)));

	Vec3 a1 = VB[endInx],b1= Vec3(0, 0, 50),c1;
	c1 = Transform(TransformQR(a1, b1), a1);

	MeshFrame frame(VB, boundaryBuffer);
	MeshFrame frame2(VB, boundaryBuffer);
	//传播算法在此
	frame2.SetBoundaryTransform(boundaryTransBuffer);
	PrintVertexBuffer(frame2.VB);
	SaveVertexBuffer(frame2.VB,"d:\\pcl1.txt");
	//目前达成:LapBuffer==DivOfGradBuffer
	//变形后的顶点?
	//正在实现:变形后三角形重构
	system("pause");
	return 0;
}

vector<Vec3> GetLaplaceBufferOnMesh(vector<Vertex>& _VB) {
	vector<Vec3> LapBuffer;
	Vec3 tlaplace, dLap;
	double tA;
	double cot1, cot2;
	for (int vertexInx = 0; vertexInx<int(_VB.size()); vertexInx++)
	{
		tlaplace = Vec3(0, 0, 0);
		tA = 0;
		for (auto& neighborTri : _VB[vertexInx].neighborTris)
		{
			dLap = Vec3(0, 0, 0);
			if (vertexInx == neighborTri.x)
			{
				cot1 = neighborTri.GetYCot();
				cot2 = neighborTri.GetZCot();
				dLap += (_VB[neighborTri.y] - _VB[vertexInx])*cot1;
				dLap += (_VB[neighborTri.z] - _VB[vertexInx])*cot2;
			}
			else if (vertexInx == neighborTri.y)
			{
				cot1 = neighborTri.GetZCot();
				cot2 = neighborTri.GetXCot();
				dLap += (_VB[neighborTri.z] - _VB[vertexInx])*cot1;
				dLap += (_VB[neighborTri.x] - _VB[vertexInx])*cot2;
			}
			else if (vertexInx == neighborTri.z)
			{
				cot1 = neighborTri.GetXCot();
				cot2 = neighborTri.GetYCot();
				dLap += (_VB[neighborTri.x] - _VB[vertexInx])*cot1;
				dLap += (_VB[neighborTri.y] - _VB[vertexInx])*cot2;
			}
			tlaplace += dLap;
			tA += neighborTri.Size();
		}
		tlaplace /= 2.0*tA;
		LapBuffer.push_back(tlaplace);
	}
	return LapBuffer;
}

vector<Vec3> GetDivOfGradBufferOnMesh(vector<Vertex>& _VB) {
	vector<Vec3> re;
	MeshGradient tGrad;
	Vec3 v1, v2;
	Vec3 tdiv;
	Vec3 tK;
	double tA;
	double tAllA;

	for (int vertexInx = 0; vertexInx<int(_VB.size()); vertexInx++)
	{
		tdiv = Vec3(0, 0, 0);
		tAllA = 0;
		for (auto& neighborTri : _VB[vertexInx].neighborTris)
		{
			tA = neighborTri.Size();
			tAllA += tA;
			if (vertexInx == neighborTri.x)
			{
				v1 = neighborTri.GetVerticalVecToZ();
				v2 = neighborTri.GetVerticalVecToY();
				v1.MakeNormal();
				v1 = v1 * (VB[neighborTri.x] - VB[neighborTri.y]).Size();
				v2.MakeNormal();
				v2 = v2 * (VB[neighborTri.x] - VB[neighborTri.z]).Size();
				tGrad.xGrad = v1 * (VB[neighborTri.y] - VB[vertexInx]).x + v2 * (VB[neighborTri.z] - VB[vertexInx]).x;
				tGrad.yGrad = v1 * (VB[neighborTri.y] - VB[vertexInx]).y + v2 * (VB[neighborTri.z] - VB[vertexInx]).y;
				tGrad.zGrad = v1 * (VB[neighborTri.y] - VB[vertexInx]).z + v2 * (VB[neighborTri.z] - VB[vertexInx]).z;
				tK = -neighborTri.GetVerticalVecToX();
				tK /= tK.Quadrance();
			}
			else if (vertexInx == neighborTri.y)
			{
				v1 = neighborTri.GetVerticalVecToX();
				v2 = neighborTri.GetVerticalVecToZ();
				v1.MakeNormal();
				v1 = v1 * (VB[neighborTri.y] - VB[neighborTri.z]).Size();
				v2.MakeNormal();
				v2 = v2 * (VB[neighborTri.x] - VB[neighborTri.y]).Size();
				tGrad.xGrad = v1 * (VB[neighborTri.z] - VB[vertexInx]).x + v2 * (VB[neighborTri.x] - VB[vertexInx]).x;
				tGrad.yGrad = v1 * (VB[neighborTri.z] - VB[vertexInx]).y + v2 * (VB[neighborTri.x] - VB[vertexInx]).y;
				tGrad.zGrad = v1 * (VB[neighborTri.z] - VB[vertexInx]).z + v2 * (VB[neighborTri.x] - VB[vertexInx]).z;
				tK = -neighborTri.GetVerticalVecToY();
				tK /= tK.Quadrance();
			}
			else if (vertexInx == neighborTri.z)
			{
				v1 = neighborTri.GetVerticalVecToY();
				v2 = neighborTri.GetVerticalVecToX();
				v1.MakeNormal();
				v1 = v1 * (VB[neighborTri.x] - VB[neighborTri.z]).Size();
				v2.MakeNormal();
				v2 = v2 * (VB[neighborTri.y] - VB[neighborTri.z]).Size();
				tGrad.xGrad = v1 * (VB[neighborTri.x] - VB[vertexInx]).x + v2 * (VB[neighborTri.y] - VB[vertexInx]).x;
				tGrad.yGrad = v1 * (VB[neighborTri.x] - VB[vertexInx]).y + v2 * (VB[neighborTri.y] - VB[vertexInx]).y;
				tGrad.zGrad = v1 * (VB[neighborTri.x] - VB[vertexInx]).z + v2 * (VB[neighborTri.y] - VB[vertexInx]).z;
				tK = -neighborTri.GetVerticalVecToZ();
				tK /= tK.Quadrance();
			}
			tdiv.x += Dot(tK, tGrad.xGrad);
			tdiv.y += Dot(tK, tGrad.yGrad);
			tdiv.z += Dot(tK, tGrad.zGrad);
		}
		tdiv /= 2.0*tAllA;
		re.push_back(tdiv);
	}
	return re;
}