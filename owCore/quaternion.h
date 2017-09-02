#pragma once

template <typename T> 
class Vector3t;

template <typename T> 
class Matrix3x3t;

template <typename T>
class aiQuaterniont {
public:
	aiQuaterniont() : w(1.0), x(), y(), z() { }
	aiQuaterniont(T pw, T px, T py, T pz) : w(pw), x(px), y(py), z(pz) { }

	/** Construct from rotation matrix. Result is undefined if the matrix is not orthonormal. */
	explicit aiQuaterniont(const Matrix3x3t<T>& pRotMatrix);

	/** Construct from euler angles */
	aiQuaterniont(T rotx, T roty, T rotz);

	/** Construct from an axis-angle pair */
	aiQuaterniont(Vector3t<T> axis, T angle);

	/** Construct from a normalized quaternion stored in a vec3 */
	explicit aiQuaterniont(Vector3t<T> normalized);

	/** Returns a matrix representation of the quaternion */
	Matrix3x3t<T> GetMatrix() const;

public:

	bool operator== (const aiQuaterniont& o) const;
	bool operator!= (const aiQuaterniont& o) const;

	bool Equal(const aiQuaterniont& o, T epsilon = 1e-6) const;

public:

	/** Normalize the quaternion */
	aiQuaterniont& Normalize();

	/** Compute quaternion conjugate */
	aiQuaterniont& Conjugate();

	/** Rotate a point by this quaternion */
	Vector3t<T> Rotate(const Vector3t<T>& in);

	/** Multiply two quaternions */
	aiQuaterniont operator* (const aiQuaterniont& two) const;

public:

	/** Performs a spherical interpolation between two quaternions and writes the result into the third.
	 * @param pOut Target object to received the interpolated rotation.
	 * @param pStart Start rotation of the interpolation at factor == 0.
	 * @param pEnd End rotation, factor == 1.
	 * @param pFactor Interpolation factor between 0 and 1. Values outside of this range yield undefined results.
	 */
	static void Interpolate(aiQuaterniont& pOut, const aiQuaterniont& pStart, const aiQuaterniont& pEnd, T pFactor);

public:

	//! w,x,y,z components of the quaternion
	T w, x, y, z;
};

typedef aiQuaterniont<float> aiQuaternion;
