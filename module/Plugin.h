#pragma once

#include <CrySystem/ICryPlugin.h>
#include "CrySystem/ISystem.h"
#include "INoesisPlugin.h"

namespace Cry
{
namespace Ns
{
	class CImplementation;
	class CPlugin final : public Cry::INoesisPlugin, public ISystemEventListener
	{
	public:

		CRYINTERFACE_BEGIN()
			CRYINTERFACE_ADD(Cry::INoesisPlugin)
			CRYINTERFACE_ADD(Cry::IEnginePlugin)
		CRYINTERFACE_END()

		CRYGENERATE_SINGLETONCLASS_GUID(CPlugin, "CryNoesis", "{f458414f-ce8c-40f3-bc0a-e0f69cc3f55c}"_cry_guid)

		PLUGIN_FLOWNODE_REGISTER
		PLUGIN_FLOWNODE_UNREGISTER;

		virtual ~CPlugin();

		//! Retrieve name of plugin.
		virtual const char* GetName() const override { return "CryNoesis"; }

		//! Retrieve category for the plugin.
		virtual const char* GetCategory() const override { return "UI"; }

		//! This is called to initialize the new plugin.
		virtual bool Initialize(SSystemGlobalEnvironment& env, const SSystemInitParams& initParams) override;

		virtual void MainUpdate(float delta) override;
		virtual void UpdateBeforeRender() override;

		CImplementation* GetImpl();

		INoesis* GetNoesis() final;

		virtual void OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam) override;

	};

}
}