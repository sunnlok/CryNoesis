////////////
//
// This has re-factored by Bismarck, original code was written by Sunnlok
// 
////////////
#pragma once

#include <CryCore/Base64.h>
#include <CryString/CryString.h>
#include <CryCore/CryEnumMacro.h>

enum class ViewFlags : uint8
{
	ScaleWithView = 1, 
	MainView = 2
};


CRY_CREATE_ENUM_FLAG_OPERATORS(ViewFlags)

struct ViewDataBase
{
	uint16 id;
	string name;
	ViewFlags flags;

	uint32 viewWidth;
	uint32 viewHeight;

	bool operator==(const ViewDataBase& other) { return other.id == id; }
	bool operator==(const uint16& otherId) { return otherId == id; }
	bool operator==(const string& otherName) { return otherName == name; }
};


struct IViewManager
{
	virtual const ViewDataBase* GetViewData(uint16 viewId) = 0;
	virtual const ViewDataBase* FindViewData(const char* name) = 0;
	virtual void ActivateView(const ViewDataBase& view, bool bActivate = true) = 0;
};
