#include "atkmath/quaternion.h"
#include "atkmath/matrix3.h"
#include "atkmath/vector3.h"

namespace atkmath
{

	Quaternion Quaternion::Slerp(const Quaternion &q0, const Quaternion &q1, double t)
	{
		float theta = acos(q0[0] * q1[0] + q0[1] * q1[1] + q0[2] * q1[2] + q0[3] * q1[3]);
		atkmath::Quaternion qnew = sin((1.0f - t) * theta) * q0 / sin(theta) + sin(t * theta) * q1 / sin(theta);
		return qnew;
	}

	void Quaternion::toAxisAngle(Vector3 &axis, double &angleRad) const
	{
		angleRad = 2.0f * acos(mW);
		if (sin(angleRad / 2.0f) == 0)
		{
			axis = Vector3(1, 0, 0);
		}
		else
		{
			axis[0] = mX / sin(angleRad / 2.0f);
			axis[1] = mY / sin(angleRad / 2.0f);
			axis[2] = mZ / sin(angleRad / 2.0f);
		}
	}

	void Quaternion::fromAxisAngle(const Vector3 &axis, double angleRad)
	{
		mW = cos(angleRad / 2.0f);
		double anglePart = sin(angleRad / 2.0f);
		mX = axis[0] * anglePart;
		mY = axis[1] * anglePart;
		mZ = axis[2] * anglePart;
		normalize();
	}

	Matrix3 Quaternion::toMatrix() const
	{
		Matrix3 m = Matrix3(1.0f - 2.0f * (mY * mY + mZ * mZ), 2.0f * (mX * mY - mW * mZ), 2.0f * (mX * mZ + mY * mW),
							2.0f * (mX * mY + mW * mZ), 1.0f - 2.0f * (mX * mX + mZ * mZ), 2.0f * (mY * mZ - mW * mX),
							2.0f * (mX * mZ - mY * mW), 2.0f * (mY * mZ - mW * mX), 1.0f - 2.0f * (mX * mX + mY * mY));

		return m;
	}

	void Quaternion::fromMatrix(const Matrix3 &rot)
	{
		/*
		float xs = 0.25f * (1.0f + rot.m11 - rot.m22 - rot.m33);
		float ys = 0.25f * (1.0f - rot.m11 + rot.m22 - rot.m33);
		float zs = 0.25f * (1.0f - rot.m22 - rot.m22 + rot.m33);
		float largest = (ws > xs) ? ws : xs;
		largest = (largest > ys) ? largest : ys;
		largest = (largest > zs) ? largest : zs;
		*/

		mW = sqrt(0.25f * (1.0f + rot[0][0] + rot[1][1] + rot[2][2]));
		mX = (rot[2][1] - rot[1][2]) / (4.0f * mW);
		mY = (rot[0][2] - rot[2][0]) / (4.0f * mW);
		mZ = (rot[1][0] - rot[0][1]) / (4.0f * mW);
	}

}