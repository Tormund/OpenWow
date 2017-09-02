#pragma once

#include "quaternion.h"

#include <cmath>

// ---------------------------------------------------------------------------
template<typename T>
bool aiQuaterniont<T>::operator== (const aiQuaterniont& o) const {
	return x == o.x && y == o.y && z == o.z && w == o.w;
}

// ---------------------------------------------------------------------------
template<typename T>
bool aiQuaterniont<T>::operator!= (const aiQuaterniont& o) const {
	return !(*this == o);
}

// ---------------------------------------------------------------------------
template<typename T>
inline bool aiQuaterniont<T>::Equal(const aiQuaterniont& o, T epsilon) const {
	return
		std::abs(x - o.x) <= epsilon &&
		std::abs(y - o.y) <= epsilon &&
		std::abs(z - o.z) <= epsilon &&
		std::abs(w - o.w) <= epsilon;
}

// ---------------------------------------------------------------------------
// Constructs a quaternion from a rotation matrix
template<typename T>
inline aiQuaterniont<T>::aiQuaterniont(const Matrix3x3t<T> &pRotMatrix) {
	T t = pRotMatrix.a1 + pRotMatrix.b2 + pRotMatrix.c3;

	// large enough
	if(t > static_cast<T>(0)) {
		T s = std::sqrt(1 + t) * static_cast<T>(2.0);
		x = (pRotMatrix.c2 - pRotMatrix.b3) / s;
		y = (pRotMatrix.a3 - pRotMatrix.c1) / s;
		z = (pRotMatrix.b1 - pRotMatrix.a2) / s;
		w = static_cast<T>(0.25) * s;
	} // else we have to check several cases
	else if(pRotMatrix.a1 > pRotMatrix.b2 && pRotMatrix.a1 > pRotMatrix.c3) {
		// Column 0:
		T s = std::sqrt(static_cast<T>(1.0) + pRotMatrix.a1 - pRotMatrix.b2 - pRotMatrix.c3) * static_cast<T>(2.0);
		x = static_cast<T>(0.25) * s;
		y = (pRotMatrix.b1 + pRotMatrix.a2) / s;
		z = (pRotMatrix.a3 + pRotMatrix.c1) / s;
		w = (pRotMatrix.c2 - pRotMatrix.b3) / s;
	}
	else if(pRotMatrix.b2 > pRotMatrix.c3) {
		// Column 1:
		T s = std::sqrt(static_cast<T>(1.0) + pRotMatrix.b2 - pRotMatrix.a1 - pRotMatrix.c3) * static_cast<T>(2.0);
		x = (pRotMatrix.b1 + pRotMatrix.a2) / s;
		y = static_cast<T>(0.25) * s;
		z = (pRotMatrix.c2 + pRotMatrix.b3) / s;
		w = (pRotMatrix.a3 - pRotMatrix.c1) / s;
	}
	else {
		// Column 2:
		T s = std::sqrt(static_cast<T>(1.0) + pRotMatrix.c3 - pRotMatrix.a1 - pRotMatrix.b2) * static_cast<T>(2.0);
		x = (pRotMatrix.a3 + pRotMatrix.c1) / s;
		y = (pRotMatrix.c2 + pRotMatrix.b3) / s;
		z = static_cast<T>(0.25) * s;
		w = (pRotMatrix.b1 - pRotMatrix.a2) / s;
	}
}

// ---------------------------------------------------------------------------
// Construction from euler angles
template<typename T>
inline aiQuaterniont<T>::aiQuaterniont(T fPitch, T fYaw, T fRoll) {
	const T fSinPitch(std::sin(fPitch*static_cast<T>(0.5)));
	const T fCosPitch(std::cos(fPitch*static_cast<T>(0.5)));
	const T fSinYaw(std::sin(fYaw*static_cast<T>(0.5)));
	const T fCosYaw(std::cos(fYaw*static_cast<T>(0.5)));
	const T fSinRoll(std::sin(fRoll*static_cast<T>(0.5)));
	const T fCosRoll(std::cos(fRoll*static_cast<T>(0.5)));
	const T fCosPitchCosYaw(fCosPitch*fCosYaw);
	const T fSinPitchSinYaw(fSinPitch*fSinYaw);
	x = fSinRoll * fCosPitchCosYaw - fCosRoll * fSinPitchSinYaw;
	y = fCosRoll * fSinPitch * fCosYaw + fSinRoll * fCosPitch * fSinYaw;
	z = fCosRoll * fCosPitch * fSinYaw - fSinRoll * fSinPitch * fCosYaw;
	w = fCosRoll * fCosPitchCosYaw + fSinRoll * fSinPitchSinYaw;
}

// ---------------------------------------------------------------------------
// Returns a matrix representation of the quaternion
template<typename T>
inline Matrix3x3t<T> aiQuaterniont<T>::GetMatrix() const {
	Matrix3x3t<T> resMatrix;
	resMatrix.a1 = static_cast<T>(1.0) - static_cast<T>(2.0) * (y * y + z * z);
	resMatrix.a2 = static_cast<T>(2.0) * (x * y - z * w);
	resMatrix.a3 = static_cast<T>(2.0) * (x * z + y * w);
	resMatrix.b1 = static_cast<T>(2.0) * (x * y + z * w);
	resMatrix.b2 = static_cast<T>(1.0) - static_cast<T>(2.0) * (x * x + z * z);
	resMatrix.b3 = static_cast<T>(2.0) * (y * z - x * w);
	resMatrix.c1 = static_cast<T>(2.0) * (x * z - y * w);
	resMatrix.c2 = static_cast<T>(2.0) * (y * z + x * w);
	resMatrix.c3 = static_cast<T>(1.0) - static_cast<T>(2.0) * (x * x + y * y);

	return resMatrix;
}

// ---------------------------------------------------------------------------
// Construction from an axis-angle pair
template<typename T>
inline aiQuaterniont<T>::aiQuaterniont(Vector3t<T> axis, T angle) {
	axis.Normalize();

	const T sin_a = std::sin(angle / 2);
	const T cos_a = std::cos(angle / 2);
	x = axis.x * sin_a;
	y = axis.y * sin_a;
	z = axis.z * sin_a;
	w = cos_a;
}
// ---------------------------------------------------------------------------
// Construction from am existing, normalized quaternion
template<typename T>
inline aiQuaterniont<T>::aiQuaterniont(Vector3t<T> normalized) {
	x = normalized.x;
	y = normalized.y;
	z = normalized.z;

	const T t = static_cast<T>(1.0) - (x*x) - (y*y) - (z*z);

	if(t < static_cast<T>(0.0)) {
		w = static_cast<T>(0.0);
	}
	else w = std::sqrt(t);
}

// ---------------------------------------------------------------------------
// Performs a spherical interpolation between two quaternions
// Implementation adopted from the gmtl project. All others I found on the net fail in some cases.
// Congrats, gmtl!
template<typename T>
inline void aiQuaterniont<T>::Interpolate(aiQuaterniont& pOut, const aiQuaterniont& pStart, const aiQuaterniont& pEnd, T pFactor) {
	// calc cosine theta
	T cosom = pStart.x * pEnd.x + pStart.y * pEnd.y + pStart.z * pEnd.z + pStart.w * pEnd.w;

	// adjust signs (if necessary)
	aiQuaterniont end = pEnd;
	if(cosom < static_cast<T>(0.0)) {
		cosom = -cosom;
		end.x = -end.x;   // Reverse all signs
		end.y = -end.y;
		end.z = -end.z;
		end.w = -end.w;
	}

	// Calculate coefficients
	T sclp, sclq;
	if((static_cast<T>(1.0) - cosom) > static_cast<T>(0.0001)) // 0.0001 -> some epsillon
	{
		// Standard case (slerp)
		T omega, sinom;
		omega = std::acos(cosom); // extract theta from dot product's cos theta
		sinom = std::sin(omega);
		sclp = std::sin((static_cast<T>(1.0) - pFactor) * omega) / sinom;
		sclq = std::sin(pFactor * omega) / sinom;
	}
	else {
		// Very close, do linear interp (because it's faster)
		sclp = static_cast<T>(1.0) - pFactor;
		sclq = pFactor;
	}

	pOut.x = sclp * pStart.x + sclq * end.x;
	pOut.y = sclp * pStart.y + sclq * end.y;
	pOut.z = sclp * pStart.z + sclq * end.z;
	pOut.w = sclp * pStart.w + sclq * end.w;
}

// ---------------------------------------------------------------------------
template<typename T>
inline aiQuaterniont<T>& aiQuaterniont<T>::Normalize() {
	// compute the magnitude and divide through it
	const T mag = std::sqrt(x*x + y*y + z*z + w*w);
	if(mag) {
		const T invMag = static_cast<T>(1.0) / mag;
		x *= invMag;
		y *= invMag;
		z *= invMag;
		w *= invMag;
	}
	return *this;
}

// ---------------------------------------------------------------------------
template<typename T>
inline aiQuaterniont<T> aiQuaterniont<T>::operator* (const aiQuaterniont& t) const {
	return aiQuaterniont(w*t.w - x*t.x - y*t.y - z*t.z,
						 w*t.x + x*t.w + y*t.z - z*t.y,
						 w*t.y + y*t.w + z*t.x - x*t.z,
						 w*t.z + z*t.w + x*t.y - y*t.x);
}

// ---------------------------------------------------------------------------
template<typename T>
inline aiQuaterniont<T>& aiQuaterniont<T>::Conjugate() {
	x = -x;
	y = -y;
	z = -z;
	return *this;
}

// ---------------------------------------------------------------------------
template<typename T>
inline Vector3t<T> aiQuaterniont<T>::Rotate(const Vector3t<T>& v) {
	aiQuaterniont q2(0.f, v.x, v.y, v.z), q = *this, qinv = q;
	qinv.Conjugate();

	q = q*q2*qinv;
	return Vector3t<T>(q.x, q.y, q.z);
}