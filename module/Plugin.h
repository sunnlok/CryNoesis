// Copyright 2016-2018 Crytek GmbH / Crytek Group. All rights reserved.
#pragma once

#include <CrySystem/ICryPlugin.h>

#include <CryGame/IGameFramework.h>

#include <CryEntitySystem/IEntityClass.h>
#include "Interfaces/INoesisPlugin.h"

class CImplementation;

class CPlugin 
	: public INoesisPlugin
	, public ISystemEventListener
{
public:
	CRYINTERFACE_SIMPLE(Cry::IEnginePlugin)
	CRYGENERATE_SINGLETONCLASS_GUID(CPlugin, "MyPlugin", "{C08C1286-5304-45D7-A0F8-F90D01A758FA}"_cry_guid)

	virtual ~CPlugin();
	
	// Cry::IEnginePlugin
	virtual bool Initialize(SSystemGlobalEnvironment& env, const SSystemInitParams& initParams) override;
	// ~Cry::IEnginePlugin
	
	virtual void MainUpdate(float delta) override;
	virtual void UpdateBeforeRender() override;


	// ISystemEventListener
	virtual void OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam) override;
	// ~ISystemEventListener

	CImplementation* GetImpl();

	INoesis* GetNoesis() final;
};