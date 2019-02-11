#include "stdafx.h"
#include "MeshUtility.h"

vector<Vertex> VB;

MeshUtility::MeshUtility()
{
}


MeshUtility::~MeshUtility()
{
}

double Triangle::GetXCot() {
	Vec3 xy, xz;
	double tcos;
	xy = VB[y] - VB[x];
	xz = VB[z] - VB[x];
	tcos = Dot(xy, xz) / (xy.Size()*xz.Size());
	return tcos / sqrt(1 - tcos * tcos);
}

double Triangle::GetYCot() {
	Vec3 yx, yz;
	double tcos;
	yx = VB[x] - VB[y];
	yz = VB[z] - VB[y];
	tcos = Dot(yx, yz) / (yx.Size()*yz.Size());
	return tcos / sqrt(1 - tcos * tcos);
}

double Triangle::GetZCot() {
	Vec3 zx, zy;
	double tcos;
	zx = VB[x] - VB[z];
	zy = VB[y] - VB[z];
	tcos = Dot(zx, zy) / (zx.Size()*zy.Size());
	return tcos / sqrt(1 - tcos * tcos);
}

double Triangle::Size() {
	//xy,xz向量叉积，取模再/2得到三角形面积
	Vec3 xy, xz, tcross;
	xy = VB[y] - VB[x];
	xz = VB[z] - VB[x];
	tcross = Cross(xy, xz);
	return tcross.Size() / 2;
}

Vec3 Triangle::GetFootOfX() {
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

Vec3 Triangle::GetFootOfY() {
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

Vec3 Triangle::GetFootOfZ() {
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

Vec3 Triangle::GetVerticalVecToX() {
	Vec3 foot = GetFootOfX();
	return VB[x] - foot;
}

Vec3 Triangle::GetVerticalVecToY() {
	Vec3 foot = GetFootOfY();
	return VB[y] - foot;
}

Vec3 Triangle::GetVerticalVecToZ() {
	Vec3 foot = GetFootOfZ();
	return VB[z] - foot;
}

Vec3 Cross(const Vec3& a, const Vec3& b) {
	Vec3 re(0, 0, 0);
	re.x = a.y*b.z - a.z*b.y;
	re.y = a.z*b.x - a.x*b.z;
	re.z = a.x*b.y - a.y*b.x;
	return re;
}



double Dot(const Vec3& a, const Vec3& b) {
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

Vec3 Normal(const Vec3& v) {
	Vec3 re;
	double r = v.Size();
	re.x = v.x / r;
	re.y = v.y / r;
	re.z = v.z / r;
	return re;
}

double Rad(const Vec3& a, const Vec3& b) {
	if (Near0(a.Size()) || Near0(b.Size()))
	{
		throw;
		return 0;
	}
	return acos(Dot(a, b) / a.Size() / b.Size());
}

Quaternion GetQua(const Vec3& v1, const Vec3& v2) {
	Vec3 n = Normal(Cross(v1, v2));
	double theta = Rad(v1, v2);
	return Quaternion(cos(theta/2.0),n.x*sin(theta/2.0), n.y*sin(theta / 2.0), n.z*sin(theta / 2.0));
}

Vec3 Transform(TransformQR transQR, const Vec3& v) {
	Quaternion qre;
	Vec3 re;
	qre = (transQR.q*v)*transQR.q.Inverse();
	re = Vec3(qre.x,qre.y,qre.z);
	re *= transQR.r;
	return re;
}