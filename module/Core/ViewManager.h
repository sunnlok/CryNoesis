#pragma once

#include "Interfaces/IViewManager.h"
#include "Renderer/ViewRenderData.h"

#include <NsCore/Ptr.h>
#include <NsGui/IView.h>

struct ViewRenderData;

struct ViewData : ViewDataBase
{
	Noesis::Ptr<::Noesis::IView> pView;

	std::unique_ptr<ViewRenderData> pViewRenderData;
};

class CViewManager final : public IViewManager
{
public:
	CViewManager();
	~CViewManager();

	static CViewManager* Get();

	void CreateViewFromXaml(const char* uri);
	void RemoveView(uint16 viewID);
	void NotifyRendererSizeChange();

	virtual const ViewDataBase* GetViewData(uint16 viewId) override;
	virtual const ViewDataBase* FindViewData(const char* name) override;

	void ActivateView(uint16 viewID, bool bActivate = true);
	virtual void ActivateView(const ViewDataBase& view, bool bActivate = true) override;

	const std::map<const char*, ViewData>& GetViews() { return m_views; }
private:
	uint16 m_nextID = 1;

	ViewData& CreateNewViewData(const char* name);

	std::map<const char*, ViewData> m_views;


};