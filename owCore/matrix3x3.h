#pragma once

#include "pushpack.h"

template <typename T>
class Matrix4x4t;

template <typename T>
class Vector2t;

// ---------------------------------------------------------------------------
/** @brief Represents a row-major 3x3 matrix
 *
 *  There's much confusion about matrix layouts (column vs. row order).
 *  This is *always* a row-major matrix. Not even with the
 *  #aiProcess_ConvertToLeftHanded flag, which absolutely does not affect
 *  matrix order - it just affects the handedness of the coordinate system
 *  defined thereby.
 */
template <typename T>
class Matrix3x3t {
public:

	Matrix3x3t() :
		a1(static_cast<T>(1.0f)), a2(), a3(),
		b1(), b2(static_cast<T>(1.0f)), b3(),
		c1(), c2(), c3(static_cast<T>(1.0f)) { }

	Matrix3x3t(T _a1, T _a2, T _a3,
			   T _b1, T _b2, T _b3,
			   T _c1, T _c2, T _c3) :
		a1(_a1), a2(_a2), a3(_a3),
		b1(_b1), b2(_b2), b3(_b3),
		c1(_c1), c2(_c2), c3(_c3) { }

public:

	// matrix multiplication.
	Matrix3x3t& operator *= (const Matrix3x3t& m);
	Matrix3x3t  operator  * (const Matrix3x3t& m) const;

	// array access operators
	T* operator[]       (unsigned int p_iIndex);
	const T* operator[] (unsigned int p_iIndex) const;

	// comparison operators
	bool operator== (const Matrix4x4t<T>& m) const;
	bool operator!= (const Matrix4x4t<T>& m) const;

	bool Equal(const Matrix4x4t<T>& m, T epsilon = 1e-6) const;

	template <typename T2>
	operator Matrix3x3t<T2>() const;

public:

	// -------------------------------------------------------------------
	/** @brief Construction from a 4x4 matrix. The remaining parts
	 *  of the matrix are ignored.
	 */
	explicit Matrix3x3t(const Matrix4x4t<T>& pMatrix);

	// -------------------------------------------------------------------
	/** @brief Transpose the matrix
	 */
	Matrix3x3t& Transpose();

	// -------------------------------------------------------------------
	/** @brief Invert the matrix.
	 *  If the matrix is not invertible all elements are set to qnan.
	 *  Beware, use (f != f) to check whether a T f is qnan.
	 */
	Matrix3x3t& Inverse();
	T Determinant() const;

public:
	// -------------------------------------------------------------------
	/** @brief Returns a rotation matrix for a rotation around z
	 *  @param a Rotation angle, in radians
	 *  @param out Receives the output matrix
	 *  @return Reference to the output matrix
	 */
	static Matrix3x3t& RotationZ(T a, Matrix3x3t& out);

	// -------------------------------------------------------------------
	/** @brief Returns a rotation matrix for a rotation around
	 *    an arbitrary axis.
	 *
	 *  @param a Rotation angle, in radians
	 *  @param axis Axis to rotate around
	 *  @param out To be filled
	 */
	static Matrix3x3t& Rotation(T a, const Vector3t<T>& axis, Matrix3x3t& out);

	// -------------------------------------------------------------------
	/** @brief Returns a translation matrix
	 *  @param v Translation vector
	 *  @param out Receives the output matrix
	 *  @return Reference to the output matrix
	 */
	static Matrix3x3t& Translation(const Vector2t<T>& v, Matrix3x3t& out);

	// -------------------------------------------------------------------
	/** @brief A function for creating a rotation matrix that rotates a
	 *  vector called "from" into another vector called "to".
	 * Input : from[3], to[3] which both must be *normalized* non-zero vectors
	 * Output: mtx[3][3] -- a 3x3 matrix in colum-major form
	 * Authors: Tomas Miller, John Hughes
	 *          "Efficiently Building a Matrix to Rotate One Vector to Another"
	 *          Journal of Graphics Tools, 4(4):1-4, 1999
	 */
	static Matrix3x3t& FromToMatrix(const Vector3t<T>& from, const Vector3t<T>& to, Matrix3x3t& out);

public:
	T a1, a2, a3;
	T b1, b2, b3;
	T c1, c2, c3;
};

typedef Matrix3x3t<float> Matrix3x3;

#include "poppack.h"

