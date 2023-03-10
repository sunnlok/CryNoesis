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

	auto& view = CreateNewViewData(pXaml->GetName());
	view.name = pXaml->GetName();

	view.viewHeight = (uint32)gEnv->pRenderer->GetHeight();
	view.viewWidth = (uint32)gEnv->pRenderer->GetWidth();

	view.pView = Noesis::GUI::CreateView(pXaml);
	view.pView->SetFlags(Noesis::RenderFlags::RenderFlags_PPAA | Noesis::RenderFlags::RenderFlags_LCD);
	view.pView->SetSize(view.viewWidth, view.viewHeight);
	view.flags = ViewFlags::MainView | ViewFlags::ScaleWithView;


}

void CViewManager::RemoveView(uint16 viewID)
{
}

void CViewManager::NotifyRendererSizeChange()
{
}

const ViewDataBase* CViewManager::GetViewData(uint16 viewId)
{
	return nullptr;
}

const ViewDataBase* CViewManager::FindViewData(const char* name)
{
	return nullptr;
}

void CViewManager::ActivateView(uint16 viewID, bool bActivated)
{
}

void CViewManager::ActivateView(const ViewDataBase& view, bool bActivate)
{
}

ViewData& CViewManager::CreateNewViewData(const char* name)
{
	if (m_nextID != 0)
	{
		auto newId = m_nextID++;
		m_views[name] = ViewData{};

		ViewData& view = m_views[name];
		view.id = newId;
		return view;
	}

	ViewData& view = m_views[name];
	view.id = m_nextID++;

	return view;
}
