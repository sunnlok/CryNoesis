////////////
//
// This has re-factored by Bismarck, original code was written by Sunnlok
// 
////////////
#pragma once

struct IViewManager;

struct INoesis
{
	virtual IViewManager* GetViewManager() const = 0;
};
