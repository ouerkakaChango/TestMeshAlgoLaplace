// MeshAlgoLaplace.cpp: 定义控制台应用程序的入口点。
//参考 https://www.docin.com/p-976236736.html

#include "stdafx.h"
#include "windows.h"
#include<iostream>
#include<vector>

using std::cout;
using std::endl;
using std::vector;
using std::pair;
using std::make_pair;


class Triangle;
class Vertex;
#define Vec3 Vertex
#define Near0(x) abs(x)<0.0001
class Vertex
{
	public:
		double x, y, z;
		vector<int> neighbors;
		vector<Triangle> neighborTris;
		Vertex() :x(0), y(0), z(0) {

		}
		Vertex(double _x, double _y, double _z):x(_x),y(_y),z(_z){

		}
		Vertex operator*(const Vertex& _v) {
			Vertex re(0,0,0);
			re.x = x * _v.x;
			re.y = y * _v.y;
			re.z = z * _v.z;
			return re;
		}

		Vertex operator*(const double& i) {
			Vertex re(0, 0, 0);
			re.x = x * i;
			re.y = y * i;
			re.z = z * i;
			return re;
		}

		Vertex operator+(const Vertex& _v) {
			Vertex re(0, 0, 0);
			re.x = x + _v.x;
			re.y = y + _v.y;
			re.z = z + _v.z;
			return re;
		}

		Vertex operator/(const double& i) {
			Vertex re(0, 0, 0);
			if (Near0(i)) {
				throw;
			}
			else
			{
				re.x = x / i;
				re.y = y / i;
				re.z = z / i;
			}
			return re;
		}

		void operator+=(const Vertex& _v) {
			x = x + _v.x;
			y = y + _v.y;
			z = z + _v.z;
		}

		void operator/=(double n) {
			if (Near0(n)) {
				throw;
			}
			else {
				x = x / n;
				y = y / n;
				z = z / n;
			}
		}

		Vertex operator-(const Vertex& _v) {
			Vertex re(0, 0, 0);
			re.x = x - _v.x;
			re.y = y - _v.y;
			re.z = z - _v.z;
			return re;
		}

		Vertex& operator-() {
			x = -x;
			y = -y;
			z = -z;
			return *this;
		}

		double Size() const {
			return sqrt(x*x+y*y+z*z);
		}

		double Quadrance () const {
			return x * x + y * y + z * z;
		}
};

vector<Vertex> VB;


double Dot(const Vec3& a, const Vec3& b){
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

Vec3 Cross(const Vec3& a, const Vec3& b) {
	Vec3 re(0, 0, 0);
	re.x = a.y*b.z - a.z*b.y;
	re.y = a.z*b.x - a.x*b.z;
	re.z = a.x*b.y - a.y*b.x;
	return re;
}

Vec3 Normal(const Vec3& v) {
	Vec3 re;
	double r = v.Size();
	re.x = v.x / r;
	re.y = v.y / r;
	re.z = v.z / r;
	return re;
}

class Triangle
{
public:
	int x, y, z;
	Triangle(int _x, int _y, int _z) :x(_x), y(_y), z(_z) {

	}
	double GetXCot() {
		Vec3 xy, xz;
		double tcos;
		xy = VB[y] - VB[x];
		xz = VB[z] - VB[x];
		tcos = Dot(xy, xz) / (xy.Size()*xz.Size());
		return tcos / sqrt(1 - tcos * tcos);
	}
	double GetYCot() {
		Vec3 yx, yz;
		double tcos;
		yx = VB[x] - VB[y];
		yz = VB[z] - VB[y];
		tcos = Dot(yx, yz) / (yx.Size()*yz.Size());
		return tcos / sqrt(1 - tcos * tcos);
	}
	double GetZCot() {
		Vec3 zx, zy;
		double tcos;
		zx = VB[x] - VB[z];
		zy = VB[y] - VB[z];
		tcos = Dot(zx, zy) / (zx.Size()*zy.Size());
		return tcos / sqrt(1 - tcos * tcos);
	}
	double Size() {
		//xy,xz向量叉积，取模再/2得到三角形面积
		Vec3 xy, xz, tcross;
		xy = VB[y] - VB[x];
		xz = VB[z] - VB[x];
		tcross = Cross(xy, xz);
		return tcross.Size()/2;
	}

	Vec3 GetFootOfX() {
		//https://blog.csdn.net/zhouschina/article/details/14647587
		//y-z x
		Vec3 re;
		double k;
		//分子
		k = Dot(VB[y] - VB[x], VB[z] - VB[y]);
		//分母
		k /= (VB[z] - VB[y]).Quadrance();
		k = -k;
		re = (VB[z] - VB[y])*k + VB[y];
		return re;
	}

	Vec3 GetFootOfY() {
		//https://blog.csdn.net/zhouschina/article/details/14647587
		//z-x y
		Vec3 re;
		double k;
		//分子
		k = Dot(VB[z] - VB[y], VB[x] - VB[z]);
		//分母
		k /= (VB[x] - VB[z]).Quadrance();
		k = -k;
		re = (VB[x] - VB[z])*k + VB[z];
		return re;
	}

	Vec3 GetFootOfZ() {
		//https://blog.csdn.net/zhouschina/article/details/14647587
		//x-y z
		Vec3 re;
		double k;
		//分子
		k = Dot(VB[x] - VB[z], VB[y] - VB[x]);
		//分母
		k /= (VB[y] - VB[x]).Quadrance();
		k = -k;
		re = (VB[y] - VB[x])*k + VB[x];
		return re;
	}

	Vec3 GetVerticalVecToX() {
		Vec3 foot = GetFootOfX();
		return VB[x] - foot;
	}

	Vec3 GetVerticalVecToY() {
		Vec3 foot = GetFootOfY();
		return VB[y] - foot;
	}

	Vec3 GetVerticalVecToZ() {
		Vec3 foot = GetFootOfZ();
		return VB[z] - foot;
	}
};

class MeshGradient
{
public:
	Vec3 xGrad, yGrad, zGrad;
};

void PrintVertexBuffer(const vector<Vertex>& _arr){
	for (auto& iter : _arr)
	{
		cout << iter.x << " " << iter.y <<" "<<iter.z<< endl;
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
			VB.push_back( Tri(j) * 10 + Vertex(0, 0, 10)*i );
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
			TB.push_back(Triangle(i0,i2,i3));
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

	vector<Vec3> LapBuffer = GetLaplaceBufferOnMesh(VB);

	vector<Vec3> DivOfGradBuffer = GetDivOfGradBufferOnMesh(VB);

	//目前问题，lap不等于梯度的散度
	system("pause");
    return 0;
}

vector<Vec3> GetLaplaceBufferOnMesh(vector<Vertex>& _VB){
	vector<Vec3> LapBuffer;
	Vec3 tlaplace,dLap;
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
				v1 = neighborTri.GetVerticalVecToY();
				v2 = neighborTri.GetVerticalVecToZ();
				//...以后改
				v1 = Normal(v1);
				v1 = v1 * (VB[neighborTri.x] - VB[neighborTri.z]).Size();
				v2 = Normal(v2);
				v2 = v2 * (VB[neighborTri.x] - VB[neighborTri.y]).Size();
				//...以后改
				tGrad.xGrad = v1 *(VB[neighborTri.y] - VB[vertexInx]).x + v2 * (VB[neighborTri.z] - VB[vertexInx]).x;
				tGrad.yGrad = v1 *(VB[neighborTri.y] - VB[vertexInx]).y + v2 * (VB[neighborTri.z] - VB[vertexInx]).y;
				tGrad.zGrad = v1 *(VB[neighborTri.y] - VB[vertexInx]).z + v2 * (VB[neighborTri.z] - VB[vertexInx]).z;
				tK = -neighborTri.GetVerticalVecToX();
				tK /= tK.Quadrance();
			}
			else if (vertexInx == neighborTri.y)
			{
				v1 = neighborTri.GetVerticalVecToZ();
				v2 = neighborTri.GetVerticalVecToX();
				//...以后改
				v1 = Normal(v1);
				v1 = v1 * (VB[neighborTri.x] - VB[neighborTri.y]).Size();
				v2 = Normal(v2);
				v2 = v2 * (VB[neighborTri.y] - VB[neighborTri.z]).Size();
				//...以后改
				tGrad.xGrad = v1 * (VB[neighborTri.z] - VB[vertexInx]).x + v2 * (VB[neighborTri.x] - VB[vertexInx]).x;
				tGrad.yGrad = v1 * (VB[neighborTri.z] - VB[vertexInx]).y + v2 * (VB[neighborTri.x] - VB[vertexInx]).y;
				tGrad.zGrad = v1 * (VB[neighborTri.z] - VB[vertexInx]).z + v2 * (VB[neighborTri.x] - VB[vertexInx]).z;
				tK = -neighborTri.GetVerticalVecToY();
				tK /= tK.Quadrance();
			}
			else if (vertexInx == neighborTri.z)
			{
				v1 = neighborTri.GetVerticalVecToX();
				v2 = neighborTri.GetVerticalVecToY();
				//...以后改
				v1 = Normal(v1);
				v1 = v1 * (VB[neighborTri.y] - VB[neighborTri.z]).Size();
				v2 = Normal(v2);
				v2 = v2 * (VB[neighborTri.x] - VB[neighborTri.z]).Size();
				//...以后改
				tGrad.xGrad = v1 * (VB[neighborTri.x] - VB[vertexInx]).x + v2 * (VB[neighborTri.y] - VB[vertexInx]).x;
				tGrad.yGrad = v1 * (VB[neighborTri.x] - VB[vertexInx]).y + v2 * (VB[neighborTri.y] - VB[vertexInx]).y;
				tGrad.zGrad = v1 * (VB[neighborTri.x] - VB[vertexInx]).z + v2 * (VB[neighborTri.y] - VB[vertexInx]).z;
				tK = -neighborTri.GetVerticalVecToZ();
				tK /= tK.Quadrance();
			}
			tdiv.x += Dot(tK, tGrad.xGrad) ;
			tdiv.y += Dot(tK, tGrad.yGrad) ;
			tdiv.z += Dot(tK, tGrad.zGrad) ;
		}
		tdiv /= 2.0*tAllA;
		re.push_back(tdiv);
	}
	return re;
}