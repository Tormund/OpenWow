#pragma once

// GLM Types
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

typedef glm::vec2 vec2;
typedef const glm::vec2& cvec2;

typedef glm::vec3 vec3;
typedef const glm::vec3& cvec3;

typedef glm::vec4 vec4;
typedef const glm::vec4& cvec4;

typedef glm::mat4x4 mat4;
typedef const glm::mat4x4& cmat4;

typedef glm::quat Quaternion;
typedef const glm::quat& cQuaternion;

#include "Matrix.h"
typedef const Matrix& cMatrix;

inline glm::vec3 operator*(cvec3 v, const glm::mat4x4& mat)
{
	vec3 o;
	o.x = mat[0][0] * v.x + mat[0][1] * v.y + mat[0][2] * v.z + mat[0][3];
	o.y = mat[1][0] * v.x + mat[1][1] * v.y + mat[1][2] * v.z + mat[1][3];
	o.z = mat[2][0] * v.x + mat[2][1] * v.y + mat[2][2] * v.z + mat[2][3];
	return o;
}

inline glm::vec3 operator*(const glm::mat4x4& mat, cvec3 v)
{
	vec3 o;
	o.x = mat[0][0] * v.x + mat[0][1] * v.y + mat[0][2] * v.z + mat[0][3];
	o.y = mat[1][0] * v.x + mat[1][1] * v.y + mat[1][2] * v.z + mat[1][3];
	o.z = mat[2][0] * v.x + mat[2][1] * v.y + mat[2][2] * v.z + mat[2][3];
	return o;
}

typedef glm::vec2 C2Vector;
typedef glm::vec3 C3Vector;

// Own types
#include "BoundingBox.h"
#include "Color.h"
#include "Rect.h"

// STL Types
#include <map>
#include <set>
#include <list>
#include <vector>
#include <string>
#include <sstream>
#include <queue>

typedef const std::string& cstring;