#include "StdAfx.h"
#include "Plugin.h"

#include <CryCore/Platform/platform_impl.inl>
#include "Core/Implementation.h"
#include "CrySchematyc/Env/EnvPackage.h"
#include "CrySchematyc/ICore.h"
#include "CrySchematyc/Env/IEnvRegistry.h"

Cry::Ns::CPlugin::~CPlugin()
{
	gEnv->pSystem->GetISystemEventDispatcher()->RemoveListener(this);
	CImplementation::Destroy();
}

bool Cry::Ns::CPlugin::Initialize(SSystemGlobalEnvironment& env, const SSystemInitParams& initParams)
{
	if (!CImplementation::Instantiate())
		return false;

	gEnv->pSystem->GetISystemEventDispatcher()->RegisterListener(this, "Noesis Plugin");

	return true;
}

Cry::Ns::CImplementation* Cry::Ns::CPlugin::GetImpl()
{	
	return CImplementation::Get();
}

void Cry::Ns::CPlugin::MainUpdate(float delta)
{
	CImplementation::Get()->Update(delta);
}

void Cry::Ns::CPlugin::UpdateBeforeRender()
{

	CImplementation::Get()->UpdateBeforeRender();
}

void Cry::Ns::CPlugin::OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam)
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
	case ESYSTEM_EVENT_CRYSYSTEM_INIT_DONE:
	{
		CImplementation::Get()->Init();

		EnableUpdate(IEnginePlugin::EUpdateStep::BeforeRender, true);
		EnableUpdate(IEnginePlugin::EUpdateStep::MainUpdate, true);
	}
	break;
	case ESYSTEM_EVENT_DISPLAY_CHANGED:
	{
		GetImpl()->OnScreenSizeChanged();
	}
	break;
	}
}

Cry::INoesis* Cry::Ns::CPlugin::GetNoesis()
{
	return CImplementation::Get();
}

CRYREGISTER_SINGLETON_CLASS(Cry::Ns::CPlugin)