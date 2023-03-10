// Copyright 2016-2017 Crytek GmbH / Crytek Group. All rights reserved.
#include "StdAfx.h"
#include "Plugin.h"

#include "Core/Handlers.h"
#include "Core/Implementation.h"

#include <CrySchematyc/Env/IEnvRegistry.h>
#include <CrySchematyc/Env/EnvPackage.h>
#include <CrySchematyc/Utils/SharedString.h>

// Included only once per DLL module.
#include <CryCore/Platform/platform_impl.inl>

#include <NsGui/IntegrationAPI.h>


CPlugin::~CPlugin()
{
	gEnv->pSystem->GetISystemEventDispatcher()->RemoveListener(this);

	if (gEnv->pSchematyc)
	{
		gEnv->pSchematyc->GetEnvRegistry().DeregisterPackage(CPlugin::GetCID());
	}
	CImplementation::Destroy();
}

bool CPlugin::Initialize(SSystemGlobalEnvironment& env, const SSystemInitParams& initParams)
{
	gEnv->pSystem->GetISystemEventDispatcher()->RegisterListener(this,"CPlugin");

	if (!CImplementation::Instantiate())
		return false;

	EnableUpdate(IEnginePlugin::EUpdateStep::BeforeRender, true);
	EnableUpdate(IEnginePlugin::EUpdateStep::MainUpdate, true);

	return true;
}

void CPlugin::MainUpdate(float delta)
{
	CImplementation::Get()->Update(delta);
}

void CPlugin::UpdateBeforeRender()
{
	CImplementation::Get()->UpdateBeforeRender();
}

void CPlugin::OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam)
{
	switch (event)
	{
	case ESYSTEM_EVENT_REGISTER_SCHEMATYC_ENV:
	{
		// Register all components that belong to this plug-in
		auto staticAutoRegisterLambda = [](Schematyc::IEnvRegistrar& registrar)
		{
			// Call all static callback registered with the CRY_STATIC_AUTO_REGISTER_WITH_PARAM
			Detail::CStaticAutoRegistrar<Schematyc::IEnvRegistrar&>::InvokeStaticCallbacks(registrar);
		};

		if (gEnv->pSchematyc)
		{
			gEnv->pSchematyc->GetEnvRegistry().RegisterPackage(
				stl::make_unique<Schematyc::CEnvPackage>(
					CPlugin::GetCID(),
					"CryNoesis",
					"",
					"UI",
					staticAutoRegisterLambda
					)
			);
		}
	}
	break;
	case ESYSTEM_EVENT_CRYSYSTEM_INIT_DONE:
	{
		CImplementation::Get()->Init();
	}
	break;
	case ESYSTEM_EVENT_DISPLAY_CHANGED:
	{
		GetImpl()->OnScreenSizeChanged();
	}
	break;
	}
}

CImplementation* CPlugin::GetImpl()
{
	return CImplementation::Get();
}

INoesis* CPlugin::GetNoesis()
{
	return CImplementation::Get();
}

CRYREGISTER_SINGLETON_CLASS(CPlugin)