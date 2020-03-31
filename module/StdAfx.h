#pragma once

#include <CryCore/Project/CryModuleDefs.h>
#define eCryModule eCryM_EnginePlugin
#define GAME_API   DLL_EXPORT

#include <CryCore/Platform/platform.h>
#include <CrySystem/ISystem.h>
#include <CryNetwork/ISerialize.h>

#pragma warning( push )
#pragma warning( disable : 4244)
#include <CrySystem/Profilers/ICryProfilingSystem.h>
#include <Cry3DEngine/I3DEngine.h>
#pragma  pop

#define CHAR8_T_DEFINED 1
using char8_t = char;