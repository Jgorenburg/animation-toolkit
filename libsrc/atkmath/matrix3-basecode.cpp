#include "atkmath/matrix3.h"
#include "atkmath/quaternion.h"

namespace atkmath
{

   Vector3 Matrix3::toEulerAnglesXYZ() const
   {
      double x, y, z;
      y = asin(this->m13);

      if (fabs(y) != 1.0f)
      {
         x = atan2(-1 * this->m23, this->m33);
         z = atan2(-1 * this->m12, this->m11);
      }
      else
      {
         x = 0;
         z = atan2(this->m21, this->m22);
      }
      return Vector3(x, y, z);
   }

   Vector3 Matrix3::toEulerAnglesXZY() const
   {
      double x, y, z;
      z = asin(-1 * this->m12);

      if (fabs(z) != 1.0f)
      {
         x = atan2(this->m32, this->m22);
         y = atan2(this->m13, this->m11);
      }
      else
      {
         x = 0;
         y = atan2(this->m23, this->m33);
      }
      return Vector3(x, y, z);
   }

   Vector3 Matrix3::toEulerAnglesYXZ() const
   {
      double x, y, z;
      x = asin(-1 * this->m23);

      if (fabs(x) != 1.0f)
      {
         z = atan2(this->m21, this->m22);
         y = atan2(this->m13, this->m33);
      }
      else
      {
         z = 0;
         y = atan2(this->m12, this->m11);
      }
      return Vector3(x, y, z);
   }

   Vector3 Matrix3::toEulerAnglesYZX() const
   {
      double x, y, z;
      z = asin(this->m21);

      if (fabs(z) != 1.0f)
      {
         x = atan2(-1 * this->m23, this->m22);
         y = atan2(-1 * this->m31, this->m11);
      }
      else
      {
         x = 0;
         y = atan2(this->m13, this->m33);
      }

      return Vector3(x, y, z);
   }

   Vector3 Matrix3::toEulerAnglesZXY() const
   {
      double x, y, z;
      x = asin(this->m32);

      if (fabs(x) != 1.0f)
      {
         z = atan2(-1 * this->m12, this->m22);
         y = atan2(-1 * this->m31, this->m33);
      }
      else
      {
         z = 0;
         y = atan2(this->m13, this->m11);
      }
      return Vector3(x, y, z);
   }

   Vector3 Matrix3::toEulerAnglesZYX() const
   {
      double x, y, z;
      y = asin(-1 * this->m31);

      if (fabs(y) != 1.0f)
      {
         z = atan2(this->m21, this->m11);
         x = atan2(this->m32, this->m33);
      }
      else
      {
         z = 0;
         x = atan2(this->m12, this->m22);
      }
      return Vector3(x, y, z);
   }

   void Matrix3::fromEulerAnglesXYZ(const Vector3 &angleRad)
   {
      double z = angleRad[2];
      double y = angleRad[1];
      double x = angleRad[0];
      Matrix3 Zmat = Matrix3(cos(z), -sin(z), 0,
                             sin(z), cos(z), 0,
                             0, 0, 1);
      Matrix3 Ymat = Matrix3(cos(y), 0, sin(y),
                             0, 1, 0,
                             -sin(y), 0, cos(y));
      Matrix3 Xmat = Matrix3(1, 0, 0,
                             0, cos(x), -sin(x),
                             0, sin(x), cos(x));
      *this = Xmat * Ymat * Zmat;
   }

   void Matrix3::fromEulerAnglesXZY(const Vector3 &angleRad)
   {
      double z = angleRad[2];
      double y = angleRad[1];
      double x = angleRad[0];
      Matrix3 Zmat = Matrix3(cos(z), -sin(z), 0,
                             sin(z), cos(z), 0,
                             0, 0, 1);
      Matrix3 Ymat = Matrix3(cos(y), 0, sin(y),
                             0, 1, 0,
                             -sin(y), 0, cos(y));
      Matrix3 Xmat = Matrix3(1, 0, 0,
                             0, cos(x), -sin(x),
                             0, sin(x), cos(x));
      *this = Xmat * Zmat * Ymat;
   }

   void Matrix3::fromEulerAnglesYXZ(const Vector3 &angleRad)
   {
      double z = angleRad[2];
      double y = angleRad[1];
      double x = angleRad[0];
      Matrix3 Zmat = Matrix3(cos(z), -sin(z), 0,
                             sin(z), cos(z), 0,
                             0, 0, 1);
      Matrix3 Ymat = Matrix3(cos(y), 0, sin(y),
                             0, 1, 0,
                             -sin(y), 0, cos(y));
      Matrix3 Xmat = Matrix3(1, 0, 0,
                             0, cos(x), -sin(x),
                             0, sin(x), cos(x));
      *this = Ymat * Xmat * Zmat;
   }

   void Matrix3::fromEulerAnglesYZX(const Vector3 &angleRad)
   {
      double z = angleRad[2];
      double y = angleRad[1];
      double x = angleRad[0];
      Matrix3 Zmat = Matrix3(cos(z), -sin(z), 0,
                             sin(z), cos(z), 0,
                             0, 0, 1);
      Matrix3 Ymat = Matrix3(cos(y), 0, sin(y),
                             0, 1, 0,
                             -sin(y), 0, cos(y));
      Matrix3 Xmat = Matrix3(1, 0, 0,
                             0, cos(x), -sin(x),
                             0, sin(x), cos(x));
      *this = Ymat * Zmat * Xmat;
   }

   void Matrix3::fromEulerAnglesZXY(const Vector3 &angleRad)
   {
      double z = angleRad[2];
      double y = angleRad[1];
      double x = angleRad[0];
      Matrix3 Zmat = Matrix3(cos(z), -sin(z), 0,
                             sin(z), cos(z), 0,
                             0, 0, 1);
      Matrix3 Ymat = Matrix3(cos(y), 0, sin(y),
                             0, 1, 0,
                             -sin(y), 0, cos(y));
      Matrix3 Xmat = Matrix3(1, 0, 0,
                             0, cos(x), -sin(x),
                             0, sin(x), cos(x));
      *this = Zmat * Xmat * Ymat;
   }

   void Matrix3::fromEulerAnglesZYX(const Vector3 &angleRad)
   {
      double z = angleRad[2];
      double y = angleRad[1];
      double x = angleRad[0];
      Matrix3 Zmat = Matrix3(cos(z), -sin(z), 0,
                             sin(z), cos(z), 0,
                             0, 0, 1);
      Matrix3 Ymat = Matrix3(cos(y), 0, sin(y),
                             0, 1, 0,
                             -sin(y), 0, cos(y));
      Matrix3 Xmat = Matrix3(1, 0, 0,
                             0, cos(x), -sin(x),
                             0, sin(x), cos(x));
      *this = Zmat * Ymat * Xmat;
   }

   void Matrix3::toAxisAngle(Vector3 &axis, double &angleRad) const
   {
      // TODO
   }

   void Matrix3::fromAxisAngle(const Vector3 &axis, double angleRad)
   {
      // TODO
      *this = Identity;
   }

}