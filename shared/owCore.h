#pragma once

#define OW_CORE_DLL_API __declspec(dllimport)

#include "../owCore/common.h"

// Maths
#include "../owCore/MathUtils.h"

// Ref manager
#include "../owCore/RefItem.h"
#include "../owCore/RefItemNamed.h"
#include "../owCore/RefManager.h"
#include "../owCore/RefSimpleManagers.h"

// Ref manager 1 Dim
#include "../owCore/RefManager1Dim.h"

// Debug outputs
#include "../owCore/DebugOutput.h"
#include "../owCore/DebugOutput_ConsoleWindows.h"
#include "../owCore/DebugOutput_Log.h"

#include "../owCore/ModulesMgr.h"

#include "../owCore/Console.h"

// Files
#include "../owCore/File.h"
#include "../owCore/FileSystem.h"
#include "../owCore/INIFile.h"
#include "../owCore/XMLFile.h"
#include "../owCore/WowChunkFile.h"

// Input
#include "../owCore/Input.h"
#include "../owCore/InputListener.h"