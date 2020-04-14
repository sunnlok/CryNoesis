#pragma once
#include "CryCore/CryEnumMacro.h"

namespace Cry
{
	namespace Ns
	{
		enum class ViewFlags : uint8
		{
			ScaleWithView = 1,
			MainView = 2,

		};
		CRY_CREATE_ENUM_FLAG_OPERATORS(ViewFlags);

		struct ViewDataBase
		{
			uint16 id;
			string name;
			ViewFlags flags;

			uint32 viewWidth;
			uint32 viewHeight;

			bool operator==(const ViewDataBase& other) { return other.id == id; }
			bool operator==(const uint16& otherID) { return otherID == id; }
			bool operator==(const string& otherName) { return otherName == name; }
		};

		struct IViewManager
		{
			virtual const ViewDataBase* GetViewData(uint16 viewID) = 0;
			virtual const ViewDataBase* FindViewData(const char* name) = 0;
			virtual void  SetViewActivated(const ViewDataBase& view, bool bActivated) = 0;
		};
	}
}