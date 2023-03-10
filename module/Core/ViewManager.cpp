#include "StdAfx.h"

#include "ViewManager.h"

#include <CrySystem/ConsoleRegistration.h>

static CViewManager* g_pViewManager;

static void ActivateViewCMD(IConsoleCmdArgs* pArgs)
{
	if (pArgs->GetArgCount() != 2)
		return;

	int id = std::atoi(pArgs->GetArg(1));

	if (id)
	{
		auto view = g_pViewManager->GetViewData((uint16)id);
		if (view)
			g_pViewManager->ActivateView(*view);
	}
	else
	{
		auto view = g_pViewManager->FindViewData(pArgs->GetArg(1));
		if (view)
			g_pViewManager->ActivateView(*view);
	}


}

static void DeactivateViewCMD(IConsoleCmdArgs* pArgs)
{
	if (pArgs->GetArgCount() != 2)
		return;

	int id = std::atoi(pArgs->GetArg(1));

	if (id)
	{
		auto view = g_pViewManager->GetViewData((uint16)id);
		if (view)
			g_pViewManager->ActivateView(*view, false);
	}
	else
	{
		auto view = g_pViewManager->FindViewData(pArgs->GetArg(1));
		if (view)
			g_pViewManager->ActivateView(*view, false);
	}
}

CViewManager::CViewManager()
{
	g_pViewManager = this;
	ConsoleRegistrationHelper::AddCommand("Noesis.Views.Activate", ActivateViewCMD, 0, "Activate a noesis view either by name or id");
	ConsoleRegistrationHelper::AddCommand("Noesis.Views.Deactivate", DeactivateViewCMD, 0, "Deactivates a noesis view either by name or id");
}


CViewManager::~CViewManager()
{
	g_pViewManager = nullptr;
}

CViewManager* CViewManager::Get()
{
	return g_pViewManager;
}

void CViewManager::CreateViewFromXaml(const char* uri)
{
	auto pXaml = Noesis::GUI::LoadXaml<Noesis::FrameworkElement>(uri);
	if (!pXaml.GetPtr())
	{
		CryLogAlways("Failed to load xaml: %s", uri);
		return;
	}

	auto& view = CreateNewViewData(pXaml->ToString().Str());
	view.name = pXaml->ToString().Str();

	view.viewHeight = (uint32)gEnv->pRenderer->GetHeight();
	view.viewWidth = (uint32)gEnv->pRenderer->GetWidth();

	view.pView = Noesis::GUI::CreateView(pXaml);
	view.pView->SetFlags(Noesis::RenderFlags::RenderFlags_PPAA | Noesis::RenderFlags::RenderFlags_LCD);
	view.pView->SetSize(view.viewWidth, view.viewHeight);
	view.flags = ViewFlags::MainView | ViewFlags::ScaleWithView;

	auto pRenderData = CRenderDevice::Get()->InitializeRenderViewData(view);
	view.pViewRenderData = std::move(pRenderData);
	view.pView->Activate();
}

void CViewManager::RemoveView(uint16 viewID)
{
	for (auto& it : m_views)
	{
		if (it.second.id == viewID)
		{
			auto pViewRenderData = std::move(it.second.pViewRenderData);
			it.second.pView->Deactivate();
			CRenderDevice::Get()->DestroyView(std::move(pViewRenderData), it.second.pView);

			m_views.erase(it.first);
		}
	}
}

void CViewManager::NotifyRendererSizeChange()
{
	uint32 newWidth = (uint32)gEnv->pRenderer->GetWidth();
	uint32 newHeight = (uint32)gEnv->pRenderer->GetHeight();

	for (auto& view : m_views)
	{
		if ((view.second.flags & ViewFlags::MainView) == 0 || (view.second.flags & ViewFlags::ScaleWithView) == 0)
			continue;
		
		if (view.second.viewWidth == newWidth || view.second.viewHeight == newHeight)
			continue;

		view.second.viewWidth = newWidth;
		view.second.viewHeight = newHeight;

		view.second.pView->SetSize(newWidth, newHeight);
		CRenderDevice::Get()->UpdateViewSize(&*view.second.pViewRenderData, newWidth, newHeight);
	}
}

const ViewDataBase* CViewManager::GetViewData(uint16 viewId)
{
	for (auto& it : m_views)
	{
		if (it.second.id == viewId)
		{
			return &it.second;
		}
	}
	return nullptr;
}

const ViewDataBase* CViewManager::FindViewData(const char* name)
{

	for (auto& it : m_views)
	{
		if (it.first == name)
		{
			return &it.second;
		}
	}
	return nullptr;
}

void CViewManager::ActivateView(uint16 viewID, bool bActivated)
{
	if (!viewID)
		return;
	
	for (auto& it : m_views)
	{
		if (it.second.id == viewID)
		{
			return ActivateView(it.second, bActivated);
		}
	}

}

void CViewManager::ActivateView(const ViewDataBase& viewBase, bool bActivate)
{
	auto& view = static_cast<const ViewData&>(viewBase);

	if (!view.pView.GetPtr())
		return;

	if (bActivate)
		view.pView->Activate();
	else
		view.pView->Deactivate();
}

ViewData& CViewManager::CreateNewViewData(const char* name)
{
	if (m_nextID != 0)
	{
		ViewData& view = m_views[name];
		view.id = m_nextID++;

		return view;
	}

	auto newId = m_nextID++;
	m_views[name] = ViewData{};

	ViewData& view = m_views[name];
	view.id = newId;
	return view;
}
