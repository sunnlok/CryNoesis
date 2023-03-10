// Copyright 2016-2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <CryCore/Project/CryModuleDefs.h>
#define eCryModule eCryM_EnginePlugin
#define GAME_API   DLL_EXPORT

#include <CryCore/Platform/platform.h>
#include <CrySystem/ISystem.h>
#include <Cry3DEngine/I3DEngine.h>
#include <CryNetwork/ISerialize.h>

#pragma warning( push )
#pragma warning( disable : 4244)
#pragma warning( disable : 4266)
#include <CrySystem/Profilers/ICryProfilingSystem.h>
#pragma  pop

#include <NsApp/InteractivityApi.h>
#include <NsApp/MediaElementApi.h>

#include <NoesisPCH.h>

#define NS_APP_INTERACTIVITY_API NS_DLL_IMPORT
#define NS_APP_MEDIAELEMENT_API NS_DLL_IMPORT