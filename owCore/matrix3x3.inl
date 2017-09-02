#pragma once

#include "matrix3x3.h"

#include "matrix4x4.h"
#include <algorithm>
#include <cmath>
#include <limits>

// ------------------------------------------------------------------------------------------------
// Construction from a 4x4 matrix. The remaining parts of the matrix are ignored.
template <typename T>
inline Matrix3x3t<T>::Matrix3x3t(const Matrix4x4t<T>& pMatrix) {
	a1 = pMatrix.a1; a2 = pMatrix.a2; a3 = pMatrix.a3;
	b1 = pMatrix.b1; b2 = pMatrix.b2; b3 = pMatrix.b3;
	c1 = pMatrix.c1; c2 = pMatrix.c2; c3 = pMatrix.c3;
}

// ------------------------------------------------------------------------------------------------
template <typename T>
inline Matrix3x3t<T>& Matrix3x3t<T>::operator *= (const Matrix3x3t<T>& m) {
	*this = Matrix3x3t<T>(m.a1 * a1 + m.b1 * a2 + m.c1 * a3,
						  m.a2 * a1 + m.b2 * a2 + m.c2 * a3,
						  m.a3 * a1 + m.b3 * a2 + m.c3 * a3,
						  m.a1 * b1 + m.b1 * b2 + m.c1 * b3,
						  m.a2 * b1 + m.b2 * b2 + m.c2 * b3,
						  m.a3 * b1 + m.b3 * b2 + m.c3 * b3,
						  m.a1 * c1 + m.b1 * c2 + m.c1 * c3,
						  m.a2 * c1 + m.b2 * c2 + m.c2 * c3,
						  m.a3 * c1 + m.b3 * c2 + m.c3 * c3);
	return *this;
}

// ------------------------------------------------------------------------------------------------
template <typename T>
template <typename TOther>
Matrix3x3t<T>::operator Matrix3x3t<TOther>() const {
	return Matrix3x3t<TOther>(static_cast<TOther>(a1), static_cast<TOther>(a2), static_cast<TOther>(a3),
							  static_cast<TOther>(b1), static_cast<TOther>(b2), static_cast<TOther>(b3),
							  static_cast<TOther>(c1), static_cast<TOther>(c2), static_cast<TOther>(c3));
}

// ------------------------------------------------------------------------------------------------
template <typename T>
inline Matrix3x3t<T> Matrix3x3t<T>::operator* (const Matrix3x3t<T>& m) const {
	Matrix3x3t<T> temp(*this);
	temp *= m;
	return temp;
}

// ------------------------------------------------------------------------------------------------
template <typename T>
inline T* Matrix3x3t<T>::operator[] (unsigned int p_iIndex) {
	return &this->a1 + p_iIndex * 3;
}

// ------------------------------------------------------------------------------------------------
template <typename T>
inline const T* Matrix3x3t<T>::operator[] (unsigned int p_iIndex) const {
	return &this->a1 + p_iIndex * 3;
}

// ------------------------------------------------------------------------------------------------
template <typename T>
inline bool Matrix3x3t<T>::operator== (const Matrix4x4t<T>& m) const {
	return a1 == m.a1 && a2 == m.a2 && a3 == m.a3 &&
		b1 == m.b1 && b2 == m.b2 && b3 == m.b3 &&
		c1 == m.c1 && c2 == m.c2 && c3 == m.c3;
}

// ------------------------------------------------------------------------------------------------
template <typename T>
inline bool Matrix3x3t<T>::operator!= (const Matrix4x4t<T>& m) const {
	return !(*this == m);
}

// ---------------------------------------------------------------------------
template<typename T>
inline bool Matrix3x3t<T>::Equal(const Matrix4x4t<T>& m, T epsilon) const {
	return
		std::abs(a1 - m.a1) <= epsilon &&
		std::abs(a2 - m.a2) <= epsilon &&
		std::abs(a3 - m.a3) <= epsilon &&
		std::abs(b1 - m.b1) <= epsilon &&
		std::abs(b2 - m.b2) <= epsilon &&
		std::abs(b3 - m.b3) <= epsilon &&
		std::abs(c1 - m.c1) <= epsilon &&
		std::abs(c2 - m.c2) <= epsilon &&
		std::abs(c3 - m.c3) <= epsilon;
}

// ------------------------------------------------------------------------------------------------
template <typename T>
inline Matrix3x3t<T>& Matrix3x3t<T>::Transpose() {
	// (T&) don't remove, GCC complains cause of packed fields
	std::swap((T&)a2, (T&)b1);
	std::swap((T&)a3, (T&)c1);
	std::swap((T&)b3, (T&)c2);
	return *this;
}

// ----------------------------------------------------------------------------------------
template <typename T>
inline T Matrix3x3t<T>::Determinant() const {
	return a1*b2*c3 - a1*b3*c2 + a2*b3*c1 - a2*b1*c3 + a3*b1*c2 - a3*b2*c1;
}

// ----------------------------------------------------------------------------------------
template <typename T>
inline Matrix3x3t<T>& Matrix3x3t<T>::Inverse() {
	// Compute the reciprocal determinant
	T det = Determinant();
	if(det == static_cast<T>(0.0)) {
		// Matrix not invertible. Setting all elements to nan is not really
		// correct in a mathematical sense; but at least qnans are easy to
		// spot. XXX we might throw an exception instead, which would
		// be even much better to spot :/.
		const T nan = std::numeric_limits<T>::quiet_NaN();
		*this = Matrix3x3t<T>(nan, nan, nan, nan, nan, nan, nan, nan, nan);

		return *this;
	}

	T invdet = static_cast<T>(1.0) / det;

	Matrix3x3t<T> res;
	res.a1 = invdet  * (b2 * c3 - b3 * c2);
	res.a2 = -invdet * (a2 * c3 - a3 * c2);
	res.a3 = invdet  * (a2 * b3 - a3 * b2);
	res.b1 = -invdet * (b1 * c3 - b3 * c1);
	res.b2 = invdet  * (a1 * c3 - a3 * c1);
	res.b3 = -invdet * (a1 * b3 - a3 * b1);
	res.c1 = invdet  * (b1 * c2 - b2 * c1);
	res.c2 = -invdet * (a1 * c2 - a2 * c1);
	res.c3 = invdet  * (a1 * b2 - a2 * b1);
	*this = res;

	return *this;
}

// ------------------------------------------------------------------------------------------------
template <typename T>
inline Matrix3x3t<T>& Matrix3x3t<T>::RotationZ(T a, Matrix3x3t<T>& out) {
	out.a1 = out.b2 = std::cos(a);
	out.b1 = std::sin(a);
	out.a2 = -out.b1;

	out.a3 = out.b3 = out.c1 = out.c2 = 0.f;
	out.c3 = 1.f;

	return out;
}

// ------------------------------------------------------------------------------------------------
// Returns a rotation matrix for a rotation around an arbitrary axis.
template <typename T>
inline Matrix3x3t<T>& Matrix3x3t<T>::Rotation(T a, const Vector3t<T>& axis, Matrix3x3t<T>& out) {
	T c = std::cos(a), s = std::sin(a), t = 1 - c;
	T x = axis.x, y = axis.y, z = axis.z;

	// Many thanks to MathWorld and Wikipedia
	out.a1 = t*x*x + c;   out.a2 = t*x*y - s*z; out.a3 = t*x*z + s*y;
	out.b1 = t*x*y + s*z; out.b2 = t*y*y + c;   out.b3 = t*y*z - s*x;
	out.c1 = t*x*z - s*y; out.c2 = t*y*z + s*x; out.c3 = t*z*z + c;

	return out;
}

// ------------------------------------------------------------------------------------------------
template <typename T>
inline Matrix3x3t<T>& Matrix3x3t<T>::Translation(const Vector2t<T>& v, Matrix3x3t<T>& out) {
	out = Matrix3x3t<T>();
	out.a3 = v.x;
	out.b3 = v.y;
	return out;
}

// ----------------------------------------------------------------------------------------
/** A function for creating a rotation matrix that rotates a vector called
 * "from" into another vector called "to".
 * Input : from[3], to[3] which both must be *normalized* non-zero vectors
 * Output: mtx[3][3] -- a 3x3 matrix in colum-major form
 * Authors: Tomas M�ller, John Hughes
 *          "Efficiently Building a Matrix to Rotate One Vector to Another"
 *          Journal of Graphics Tools, 4(4):1-4, 1999
 */
 // ----------------------------------------------------------------------------------------
template <typename T>
inline Matrix3x3t<T>& Matrix3x3t<T>::FromToMatrix(const Vector3t<T>& from, const Vector3t<T>& to, Matrix3x3t<T>& mtx) {
	const T e = from * to;
	const T f = (e < 0) ? -e : e;

	if(f > static_cast<T>(1.0) - static_cast<T>(0.00001))     /* "from" and "to"-vector almost parallel */
	{
		Vector3D u, v;     /* temporary storage vectors */
		Vector3D x;       /* vector most nearly orthogonal to "from" */

		x.x = (from.x > 0.0) ? from.x : -from.x;
		x.y = (from.y > 0.0) ? from.y : -from.y;
		x.z = (from.z > 0.0) ? from.z : -from.z;

		if(x.x < x.y) {
			if(x.x < x.z) {
				x.x = static_cast<T>(1.0);
				x.y = x.z = static_cast<T>(0.0);
			}
			else {
				x.z = static_cast<T>(1.0);
				x.x = x.y = static_cast<T>(0.0);
			}
		}
		else {
			if(x.y < x.z) {
				x.y = static_cast<T>(1.0);
				x.x = x.z = static_cast<T>(0.0);
			}
			else {
				x.z = static_cast<T>(1.0);
				x.x = x.y = static_cast<T>(0.0);
			}
		}

		u.x = x.x - from.x; u.y = x.y - from.y; u.z = x.z - from.z;
		v.x = x.x - to.x;   v.y = x.y - to.y;   v.z = x.z - to.z;

		const T c1 = static_cast<T>(2.0) / (u * u);
		const T c2 = static_cast<T>(2.0) / (v * v);
		const T c3 = c1 * c2  * (u * v);

		for(unsigned int i = 0; i < 3; i++) {
			for(unsigned int j = 0; j < 3; j++) {
				mtx[i][j] = -c1 * u[i] * u[j] - c2 * v[i] * v[j]
					+ c3 * v[i] * u[j];
			}
			mtx[i][i] += static_cast<T>(1.0);
		}
	}
	else  /* the most common case, unless "from"="to", or "from"=-"to" */
	{
		const Vector3D v = from ^ to;
		/* ... use this hand optimized version (9 mults less) */
		const T h = static_cast<T>(1.0) / (static_cast<T>(1.0) + e);      /* optimization by Gottfried Chen */
		const T hvx = h * v.x;
		const T hvz = h * v.z;
		const T hvxy = hvx * v.y;
		const T hvxz = hvx * v.z;
		const T hvyz = hvz * v.y;
		mtx[0][0] = e + hvx * v.x;
		mtx[0][1] = hvxy - v.z;
		mtx[0][2] = hvxz + v.y;

		mtx[1][0] = hvxy + v.z;
		mtx[1][1] = e + h * v.y * v.y;
		mtx[1][2] = hvyz - v.x;

		mtx[2][0] = hvxz - v.y;
		mtx[2][1] = hvyz + v.x;
		mtx[2][2] = e + hvz * v.z;
	}
	return mtx;
}
