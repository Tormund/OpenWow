#pragma once

#include "Platform.h"
#include "Math.h"

typedef Vec2f vec2;
typedef const Vec2f& cvec2;

typedef Vec3f vec3;
typedef const Vec3f& cvec3;

typedef Vec4f vec4;
typedef const Vec4f& cvec4;

typedef Matrix4f mat4;
typedef const Matrix4f& cmat4;

typedef Quaternion quat;
typedef const Quaternion& cquat;

// Own types
#include "BoundingBox.h"
#include "Color.h"
#include "Primitives.h"
#include "Rect.h"
#include "Timer.h"

// STL Types
#include <map>
#include <set>
#include <list>
#include <vector>
#include <string>
#include <sstream>
#include <queue>

typedef const std::string& cstring;