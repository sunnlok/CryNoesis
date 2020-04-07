#include "StdAfx.h"
#include "ViewManager.h"
#include <NsGui/IView.h>
#include "Renderer/ViewRenderData.h"

using namespace Cry::Ns;


static ViewManager* g_pViewManager;

Cry::Ns::ViewManager* Cry::Ns::ViewManager::Get()
{
	return g_pViewManager;
}

Cry::Ns::ViewManager::ViewManager()
{
	g_pViewManager = this;
}

Cry::Ns::ViewManager::~ViewManager()
{
	g_pViewManager = nullptr;
}



void Cry::Ns::ViewManager::CreateViewFromXaml(const char* uri)
{
	auto pXaml = Noesis::GUI::LoadXaml<Noesis::FrameworkElement>(uri);
	if (!pXaml.GetPtr())
	{
		CryLogAlways("Failed to load xaml: %s", uri);
		return;
	}

	//Take the name of the root element as name of the view
	auto& view = CreateNewViewData();
	view.name = pXaml->ToString().Str();

	//TODO: Init to render width for now. Allow proper size assignement later
	view.viewWidth = (uint32)gEnv->pRenderer->GetWidth();
	view.viewHeight = (uint32)gEnv->pRenderer->GetHeight();

	view.pView = Noesis::GUI::CreateView(pXaml);
	view.pView->SetFlags(Noesis::RenderFlags::RenderFlags_PPAA);
	view.pView->SetSize(view.viewWidth, view.viewHeight);

	view.flags = ViewFlags::MainView | ViewFlags::ScaleWithView;

	auto pRenderData = CRenderDevice::Get()->InitializeRenderViewData(view);
	view.pViewRenderData = std::move(pRenderData);
	view.pView->Activate();
}

Cry::Ns::ViewData& Cry::Ns::ViewManager::CreateNewViewData()
{
	m_views.emplace_back();
	auto& newView = m_views.back();

	if (!m_freeIds.empty())
	{
		auto lastID = m_freeIds.end() - 1;
		newView.id = *lastID;
		m_freeIds.erase(lastID);
	}
	else
		newView.id = m_nextID++;

	return newView;
}

void Cry::Ns::ViewManager::RemoveView(uint16 viewID)
{
	auto view = std::find(m_views.begin(), m_views.end(), viewID);
	if (view == m_views.end())
		return;

	//We need to extend the lifetime of the render data and the noesis view.
	auto pViewRenderData = std::move(view->pViewRenderData);
	view->pView->Deactivate();
	CRenderDevice::Get()->DestroyView(std::move(pViewRenderData), view->pView);

	m_views.erase(view);
}

void Cry::Ns::ViewManager::NotifyRendererSizeChange()
{
	uint32 newWidth = (uint32)gEnv->pRenderer->GetWidth();
	uint32 newHeight = (uint32)gEnv->pRenderer->GetHeight();
	
	for (auto&  view : m_views)
	{
		if ((view.flags & ViewFlags::MainView) == 0 || (view.flags & ViewFlags::ScaleWithView) == 0)
			continue;

		if (view.viewWidth == newWidth || view.viewHeight == newHeight)
			continue;

		view.viewWidth = newWidth;
		view.viewHeight = newHeight;

		view.pView->SetSize(newWidth, newHeight);
		CRenderDevice::Get()->UpdateViewSize(&*view.pViewRenderData, newWidth, newHeight);
	}
}

void Cry::Ns::ViewManager::SetViewActivated(uint16 viewID, bool bActivated)
{
	if (!viewID)
		return;

	auto view = std::find(m_views.begin(), m_views.end(), viewID);
	if (view == m_views.end())
		return;

	if (bActivated)
		view->pView->Activate();
	else
		view->pView->Deactivate();
}

