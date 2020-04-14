#pragma once
#include "CrySystem/ICryPlugin.h"

namespace Cry
{

	struct INoesis;

	struct INoesisPlugin : public IEnginePlugin
	{
		CRYINTERFACE_DECLARE_GUID(INoesisPlugin, "{5af1f056-8f10-42a0-ac0b-2385f506cf9d}"_cry_guid);

		virtual INoesis* GetNoesis() = 0;
	};

}