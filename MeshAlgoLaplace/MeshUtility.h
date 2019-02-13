#pragma once
#include<vector>
#include "MeshTransform.h"

using std::vector;
using std::pair;
using std::make_pair;

class MeshUtility
{
public:
};

class Vertex
{
public:
	double x, y, z;
	vector<int> neighbors;
	vector<Triangle> neighborTris;
	Vertex() :x(0), y(0), z(0) {

	}
	Vertex(double _x, double _y, double _z) :x(_x), y(_y), z(_z) {

	}
	Vertex operator*(const Vertex& _v) {
		Vertex re(0, 0, 0);
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

	void operator*=(double n) {
		x = x * n;
		y = y * n;
		z = z * n;
	}

	Vertex operator-(const Vertex& _v) {
		Vertex re(0, 0, 0);
		re.x = x - _v.x;
		re.y = y - _v.y;
		re.z = z - _v.z;
		return re;
	}

	Vertex operator-() {
		Vertex re(0, 0, 0);
		re.x = -x;
		re.y = -y;
		re.z = -z;
		return re;
	}

	double Size() const {
		return sqrt(x*x + y * y + z * z);
	}

	double Quadrance() const {
		return x * x + y * y + z * z;
	}

	void MakeNormal() {
		double r = Size();
		if (!Near0(r))
		{
			x = x / r;
			y = y / r;
			z = z / r;
		}
	}

	void Set(double _x, double _y, double _z);
	void Set(Vec3 v);
};

double Dot(const Vec3& a, const Vec3& b);

Vec3 Cross(const Vec3& a, const Vec3& b);

Vec3 Normal(const Vec3& v);

double Rad(const Vec3& a, const Vec3& b);

class Triangle
{
public:
	int x, y, z;
	Triangle(int _x, int _y, int _z) :x(_x), y(_y), z(_z) {

	}
	double GetXCot();
	double GetYCot();
	double GetZCot();

	double Size();

	Vec3 GetFootOfX();
	Vec3 GetFootOfY();
	Vec3 GetFootOfZ();

	Vec3 GetVerticalVecToX();

	Vec3 GetVerticalVecToY();

	Vec3 GetVerticalVecToZ();
};

class MeshGradient
{
public:
	Vec3 xGrad, yGrad, zGrad;
};

Quaternion GetQua(const Vec3& v1,const Vec3& v2);
Vec3 Transform(TransformQR transQR, const Vec3& v);