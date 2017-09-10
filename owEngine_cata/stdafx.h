#pragma once

#define NOMINMAX
#include <winerror.h>

// INCLUDES
#include <GL/glew.h>

#define GLFW_DLL
#include <GLFW/glfw3.h>

// ASSIMP
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/DefaultLogger.hpp>

#include <owCore.h>
#pragma comment(lib, "owCore.lib")

#include <owRender.h>
#pragma comment(lib, "owRender.lib")

#include <owGUI.h>
#pragma comment(lib, "owGUI.lib")

#include <owGame.h>
#pragma comment(lib, "owGame.lib")

#include <owDBC.h>
#pragma comment(lib, "owDBC.lib")
