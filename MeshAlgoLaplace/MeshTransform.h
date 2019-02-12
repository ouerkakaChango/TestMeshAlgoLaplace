#pragma once

class Triangle;
class Vertex;
#define Vec3 Vertex
#define Near0(x) (abs(x)<0.0001)

class MeshTransform
{
public:
	MeshTransform();
	~MeshTransform();
};

//q=w+xi+yj+zk
class Quaternion{
public:
	double w, x, y, z;
	Quaternion() :w(0.0), x(0.0), y(0.0), z(0.0) {}
	Quaternion(double _w, double _x, double _y, double _z) :w(_w), x(_x), y(_y), z(_z) {}
	Quaternion operator*(const Quaternion& q);
	Quaternion operator*(const Vec3& v);
	Quaternion operator*(double n);
	Quaternion operator/(double n);
	void operator*=(double n);
	void operator/=(double n);
	void operator+=(const Quaternion& q);
	Quaternion Conjugate();
	Quaternion Inverse();
	double Size() const;
	double Quadrance() const;
};

Quaternion Normal(const Quaternion& q);


//使用四元数(Quaternion)和位矢大小比例(Ratio)表示的Transform
class TransformQR{
public:
	Quaternion q;
	double r;
	TransformQR() :q(Quaternion()), r(0.0) {}
	TransformQR(Quaternion _q, double _r) :q(Normal(_q)), r(_r) {}
	TransformQR(Vec3 v1, Vec3 v2);
	TransformQR operator*(double n);
	void operator/=(double n);
	void operator+=(const TransformQR& transQR);
};

