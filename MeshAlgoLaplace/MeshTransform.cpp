#include "stdafx.h"
#include "MeshTransform.h"
#include "MeshUtility.h"

MeshTransform::MeshTransform()
{
}


MeshTransform::~MeshTransform()
{
}

Quaternion Quaternion::operator*(const Quaternion& q) {
	Quaternion re;
	re.w = w * q.w - x * q.x - y * q.y - z * q.z;
	re.x = w * q.x + x * q.w + y * q.z - z * q.y;
	re.y = w * q.y - x * q.z + y * q.w + z * q.x;
	re.z = w * q.z + x * q.y - y * q.x + z * q.w;
	return re;
}

Quaternion Quaternion::operator*(const Vec3& v) {
	return (*this)*Quaternion(0.0, v.x, v.y, v.z);
}

Quaternion Quaternion::operator/(double n) {
	Quaternion re;
	if (Near0(n)) {
		throw;
		return Quaternion();
	}
	re.w = w/n;
	re.x = x/n;
	re.y = y/n;
	re.z = z/n;
	return re;
}

Quaternion Quaternion::Conjugate(){
	Quaternion re;
	re.w = w;
	re.x = -x;
	re.y = -y;
	re.z = -z;
	return re;
}

Quaternion Quaternion::Inverse() {
	return Conjugate() / Quadrance();
}

double Quaternion::Size() const{
	return sqrt(x * x + y * y + z * z + w * w);
}

double Quaternion::Quadrance() const {
	return x * x + y * y + z * z + w * w;
}

Quaternion Normal(const Quaternion& q) {
	Quaternion re;
	double len = q.Size();
	re.w = q.w / len;
	re.x = q.x / len;
	re.y = q.y / len;
	re.z = q.z / len;
	return re;
}

TransformQR::TransformQR(Vec3 v1, Vec3 v2) {
	r = v2.Size() / v1.Size();
	//GetQua的函数内保证了返回值已经单位化
	q = GetQua(v1, v2);
}