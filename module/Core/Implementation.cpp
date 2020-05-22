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
#include <NsCore/RegisterComponent.h>
#include "Controls/ViewContainer.h"
#include "InputHandler.h"
#include "ComponentRegistration.h"
#include "ViewManager.h"


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

static void PlaySound(void* user, const char* filename, float volume)
{
	auto actualFilename = PathUtil::GetFileName(filename);
	auto triggerID = CryAudio::StringToId(actualFilename);

	gEnv->pAudioSystem->ExecuteTrigger(triggerID);
}

Cry::Ns::CImplementation::CImplementation()
{
	RegisterVariables();
	
	m_pViewManager = std::make_unique<ViewManager>();

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
	const char* fonts[] = { "Segoe UI Emoji" };
	//GUI::LoadApplicationResources("MenuResources.xaml");
	Noesis::GUI::SetFontFallbacks( fonts, 1 );
	Noesis::GUI::SetFontDefaultProperties(15, FontWeight_Normal, FontStretch_Normal, FontStyle_Normal);

	Noesis::RegisterComponent<Cry::Ns::Components::ViewContainer>();
	Cry::Ns::Registration::RegisterInteractivityComponents();
	Noesis::GUI::SetPlayAudioCallback(nullptr, PlaySound);

	LoadResources();
}

Cry::Ns::CImplementation::~CImplementation()
{
	m_pViewManager.reset();

	Noesis::GUI::Shutdown();
}

void Cry::Ns::CImplementation::RegisterVariables()
{
	ConsoleRegistrationHelper::AddCommand("Noesis.LoadXaml", &LoadXamlCmd);


	m_pResourceDictVar = ConsoleRegistrationHelper::RegisterString("Noesis.ResourceDictionary", "", 0, "Resource dictionary to load at startup.");

}

void Cry::Ns::CImplementation::LoadResources()
{
	//Make this able to be compiled in
	string resources = m_pResourceDictVar->GetString();

	Noesis::GUI::LoadApplicationResources(resources);
}

void Cry::Ns::CImplementation::Init()
{
	
	m_startTime = gEnv->pTimer->GetFrameStartTime();
	m_pInputHandler = std::make_unique<CInputHandler>(this);

	m_pRenderDevice.Reset(new Ns::CRenderDevice());
}

void Cry::Ns::CImplementation::Update(float delta)
{
	
}

void Cry::Ns::CImplementation::UpdateBeforeRender()
{
	auto curTime = gEnv->pTimer->GetFrameStartTime() - m_startTime;


	for (auto& view : m_pViewManager->GetViews())
	{
		view.pView->Update(curTime.GetSeconds());
	}
		
}

void Cry::Ns::CImplementation::OnScreenSizeChanged()
{
	m_pViewManager->NotifyRendererSizeChange();
		
}

bool Cry::Ns::CImplementation::CreateView(const char* xamlPath, Vec2i dimensions)
{
	m_pViewManager->CreateViewFromXaml(xamlPath);

	return true;
}

Cry::Ns::ViewManager* Cry::Ns::CImplementation::GetViewManager() const
{
	return &*m_pViewManager;
}

