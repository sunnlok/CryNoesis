#pragma once

namespace Cry
{
	namespace Ns
	{
		struct IViewManager;
	}

	struct INoesis
	{
		virtual Ns::IViewManager* GetViewManager() const = 0;
	};

}