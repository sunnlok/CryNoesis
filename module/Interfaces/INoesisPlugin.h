////////////
//
// This has re-factored by Bismarck, original code was written by Sunnlok
// 
////////////
#pragma once

#include <CrySystem/ICryPlugin.h>

struct INoesis;

struct INoesisPlugin : public Cry::IEnginePlugin
{
	CRYINTERFACE_DECLARE_GUID(INoesisPlugin, "{101801A1-824B-4330-8EE4-3DB9BE7EAF55}"_cry_guid);

	virtual INoesis* GetNoesis() = 0;
};