#include "StdAfx.h"
#include "Implementation.h"
#include <NsCore/Noesis.h>
#include <NsGui/IntegrationAPI.h>
#include <NsCore/Memory.h>

#include "Handlers.h"
#include "Providers/XamlProvider.h"
#include "Providers/TextureProvider.h"
#include "Providers/FontProvider.h"
#include <NsGui/IView.h>
#include <NsGui/IRenderer.h>
#include "NsGui/FrameworkElement.h"
#include "CrySystem/ConsoleRegistration.h"
#include <NsGui/ResourceDictionary.h>
#include "CryRenderer/IRenderAuxGeom.h"

static std::unique_ptr<Cry::Ns::CImplementation> g_pImplementation;

Cry::Ns::CImplementation* Cry::Ns::CImplementation::Instantiate()
{
	if (g_pImplementation)
		return g_pImplementation.get();

	g_pImplementation = std::make_unique<Cry::Ns::CImplementation>();
	return g_pImplementation.get();
}

void Cry::Ns::CImplementation::Destroy()
{
	if (g_pImplementation)
		g_pImplementation.release();
}

Cry::Ns::CImplementation* Cry::Ns::CImplementation::Get()
{
	return g_pImplementation.get();
}

//////////////////////////////////////////////////////////////////////////

using namespace Cry;

static Noesis::MemoryCallbacks g_memCallbacks = {
	nullptr,
	&Ns::Detail::Allocator::Alloc,
	&Ns::Detail::Allocator::ReAlloc,
	&Ns::Detail::Allocator::DeAlloc,
	&Ns::Detail::Allocator::AllocSize
};


static void LoadXamlCmd(IConsoleCmdArgs* pArgs)
{
	if (!g_pImplementation && pArgs->GetArgCount() < 2)
		return;

	g_pImplementation->CreateView(pArgs->GetArg(1), { gEnv->pRenderer->GetWidth(), gEnv->pRenderer->GetHeight() });
}


Cry::Ns::CImplementation::CImplementation()
{
	ConsoleRegistrationHelper::AddCommand("Noesis.LoadXaml", &LoadXamlCmd);

	using namespace Noesis;

	Noesis::SetMemoryCallbacks(g_memCallbacks);

	Noesis::SetErrorHandler(&Ns::Detail::ErrorHandler);


	Noesis::SetLogHandler(&Ns::Detail::LogHandler);
	Noesis::SetAssertHandler(&Ns::Detail::AssertHandler);

	GUI::Init(NS_LICENSE_NAME, NS_LICENSE_KEY);
	{
		using namespace Noesis::GUI;

		SetXamlProvider(MakePtr<Ns::CXamlProvider>());
		SetFontProvider(MakePtr<Ns::CFontProvider>());
		SetTextureProvider(MakePtr<Ns::CTextureProvider>());
	}	
	const char* fonts[] = { "./#PT Root UI", "Arial", "Segoe UI Emoji" };
	GUI::LoadApplicationResources("NocturnalStyle.xaml");
	Noesis::GUI::SetFontFallbacks( fonts, 3 );
	Noesis::GUI::SetFontDefaultProperties(15, FontWeight_Normal, FontStretch_Normal, FontStyle_Normal);
}

Cry::Ns::CImplementation::~CImplementation()
{
	m_views.clear();
	Noesis::GUI::Shutdown();
}

void Cry::Ns::CImplementation::Init()
{
	
	m_startTime = gEnv->pTimer->GetFrameStartTime();
	m_pRenderDevice = Noesis::MakePtr<Ns::CRenderDevice>();
}

void Cry::Ns::CImplementation::Update(float delta)
{

}

void Cry::Ns::CImplementation::UpdateBeforeRender()
{
	auto curTime = gEnv->pTimer->GetFrameStartTime() - m_startTime;


	for (auto& view : m_views)
		view->Update(curTime.GetSeconds());
}

bool Cry::Ns::CImplementation::CreateView(const char* xamlPath, Vec2i dimensions)
{
	Noesis::Ptr<Noesis::FrameworkElement> xaml = Noesis::GUI::LoadXaml<Noesis::FrameworkElement>(xamlPath);


	auto pView = Noesis::GUI::CreateView(xaml);
	if (!pView.GetPtr())
		return false;

	pView->SetSize(dimensions.x, dimensions.y);

	m_pRenderDevice->AddView({ pView, dimensions.x, dimensions.y });


	m_views.emplace_back(std::move(pView));

	return true;
}

