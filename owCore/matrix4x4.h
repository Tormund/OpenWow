#pragma once

#include "vector3.h"
#include "pushpack.h"

template<typename T> class Matrix3x3t;
template<typename T> class aiQuaterniont;

// ---------------------------------------------------------------------------
/** @brief Represents a row-major 4x4 matrix, use this for homogeneous
 *   coordinates.
 *
 *  There's much confusion about matrix layouts (column vs. row order).
 *  This is *always* a row-major matrix. Not even with the
 *  #aiProcess_ConvertToLeftHanded flag, which absolutely does not affect
 *  matrix order - it just affects the handedness of the coordinate system
 *  defined thereby.
 */
template<typename T>
class Matrix4x4t {
public:

	/** set to identity */
	Matrix4x4t();

	/** construction from single values */
	Matrix4x4t(T _a1, T _a2, T _a3, T _a4,
				 T _b1, T _b2, T _b3, T _b4,
				 T _c1, T _c2, T _c3, T _c4,
				 T _d1, T _d2, T _d3, T _d4);


	/** construction from 3x3 matrix, remaining elements are set to identity */
	explicit Matrix4x4t(const Matrix3x3t<T>& m);

	/** construction from position, rotation and scaling components
	 * @param scaling The scaling for the x,y,z axes
	 * @param rotation The rotation as a hamilton quaternion
	 * @param position The position for the x,y,z axes
	 */
	Matrix4x4t(const Vector3t<T>& scaling, const aiQuaterniont<T>& rotation, const Vector3t<T>& position);

public:

	// array access operators
	T* operator[]       (unsigned int p_iIndex);
	const T* operator[] (unsigned int p_iIndex) const;

	// comparison operators
	bool operator== (const Matrix4x4t& m) const;
	bool operator!= (const Matrix4x4t& m) const;

	bool Equal(const Matrix4x4t& m, T epsilon = 1e-6) const;

	// matrix multiplication.
	Matrix4x4t& operator *= (const Matrix4x4t& m);
	Matrix4x4t  operator *  (const Matrix4x4t& m) const;

	template <typename TOther>
	operator Matrix4x4t<TOther>() const;

public:

	// -------------------------------------------------------------------
	/** @brief Transpose the matrix */
	Matrix4x4t& Transpose();

	// -------------------------------------------------------------------
	/** @brief Invert the matrix.
	 *  If the matrix is not invertible all elements are set to qnan.
	 *  Beware, use (f != f) to check whether a T f is qnan.
	 */
	Matrix4x4t& Inverse();
	T Determinant() const;


	// -------------------------------------------------------------------
	/** @brief Returns true of the matrix is the identity matrix.
	 *  The check is performed against a not so small epsilon.
	 */
	inline bool IsIdentity() const;

	// -------------------------------------------------------------------
	/** @brief Decompose a trafo matrix into its original components
	 *  @param scaling Receives the output scaling for the x,y,z axes
	 *  @param rotation Receives the output rotation as a hamilton
	 *   quaternion
	 *  @param position Receives the output position for the x,y,z axes
	 */
	void Decompose(Vector3t<T>& scaling, aiQuaterniont<T>& rotation, Vector3t<T>& position) const;

	// -------------------------------------------------------------------
	/** @brief Decompose a trafo matrix with no scaling into its
	 *    original components
	 *  @param rotation Receives the output rotation as a hamilton
	 *    quaternion
	 *  @param position Receives the output position for the x,y,z axes
	 */
	void DecomposeNoScaling(aiQuaterniont<T>& rotation, Vector3t<T>& position) const;


	// -------------------------------------------------------------------
	/** @brief Creates a trafo matrix from a set of euler angles
	 *  @param x Rotation angle for the x-axis, in radians
	 *  @param y Rotation angle for the y-axis, in radians
	 *  @param z Rotation angle for the z-axis, in radians
	 */
	Matrix4x4t& FromEulerAnglesXYZ(T x, T y, T z);
	Matrix4x4t& FromEulerAnglesXYZ(const Vector3t<T>& blubb);

public:
	// -------------------------------------------------------------------
	/** @brief Returns a rotation matrix for a rotation around the x axis
	 *  @param a Rotation angle, in radians
	 *  @param out Receives the output matrix
	 *  @return Reference to the output matrix
	 */
	static Matrix4x4t& RotationX(T a, Matrix4x4t& out);

	// -------------------------------------------------------------------
	/** @brief Returns a rotation matrix for a rotation around the y axis
	 *  @param a Rotation angle, in radians
	 *  @param out Receives the output matrix
	 *  @return Reference to the output matrix
	 */
	static Matrix4x4t& RotationY(T a, Matrix4x4t& out);

	// -------------------------------------------------------------------
	/** @brief Returns a rotation matrix for a rotation around the z axis
	 *  @param a Rotation angle, in radians
	 *  @param out Receives the output matrix
	 *  @return Reference to the output matrix
	 */
	static Matrix4x4t& RotationZ(T a, Matrix4x4t& out);

	// -------------------------------------------------------------------
	/** Returns a rotation matrix for a rotation around an arbitrary axis.
	 *  @param a Rotation angle, in radians
	 *  @param axis Rotation axis, should be a normalized vector.
	 *  @param out Receives the output matrix
	 *  @return Reference to the output matrix
	 */
	static Matrix4x4t& Rotation(T a, const Vector3t<T>& axis,
								  Matrix4x4t& out);

	// -------------------------------------------------------------------
	/** @brief Returns a translation matrix
	 *  @param v Translation vector
	 *  @param out Receives the output matrix
	 *  @return Reference to the output matrix
	 */
	static Matrix4x4t& Translation(const Vector3t<T>& v, Matrix4x4t& out);

	// -------------------------------------------------------------------
	/** @brief Returns a scaling matrix
	 *  @param v Scaling vector
	 *  @param out Receives the output matrix
	 *  @return Reference to the output matrix
	 */
	static Matrix4x4t& Scaling(const Vector3t<T>& v, Matrix4x4t& out);

	// -------------------------------------------------------------------
	/** @brief A function for creating a rotation matrix that rotates a
	 *  vector called "from" into another vector called "to".
	 * Input : from[3], to[3] which both must be *normalized* non-zero vectors
	 * Output: mtx[3][3] -- a 3x3 matrix in column-major form
	 * Authors: Tomas Mueller, John Hughes
	 *          "Efficiently Building a Matrix to Rotate One Vector to Another"
	 *          Journal of Graphics Tools, 4(4):1-4, 1999
	 */
	static Matrix4x4t& FromToMatrix(const Vector3t<T>& from, const Vector3t<T>& to, Matrix4x4t& out);

public:
	T a1, a2, a3, a4;
	T b1, b2, b3, b4;
	T c1, c2, c3, c4;
	T d1, d2, d3, d4;
};

typedef Matrix4x4t<float> Matrix4x4;

#include "poppack.h"

