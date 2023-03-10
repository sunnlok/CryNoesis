#include "StdAfx.h"

#include "Implementation.h"

#include "Handlers.h"
#include "Providers/XamlProvider.h"
#include "Providers/TextureProvider.h"
#include "Providers/FontProvider.h"
#include "InputHandler.h"
#include "Controls/ViewContainer.h"

#include <CrySystem/ConsoleRegistration.h>

static std::unique_ptr<CImplementation> g_pImplementation;


CImplementation* CImplementation::Instantiate()
{
	if (g_pImplementation)
		return g_pImplementation.get();

	g_pImplementation = std::make_unique<CImplementation>();
	return g_pImplementation.get();
}

void CImplementation::Destroy()
{
	if (g_pImplementation)
		g_pImplementation.release();
}

CImplementation* CImplementation::Get()
{
	return g_pImplementation.get();
}

static void LoadXamlCmd(IConsoleCmdArgs* pArgs)
{
	if (!g_pImplementation && pArgs->GetArgCount() < 2)
		return;

	g_pImplementation->CreateView(pArgs->GetArg(1), { gEnv->pRenderer->GetWidth(), gEnv->pRenderer->GetHeight() });
}

static void PlaySound(void* user, const Noesis::Uri& filename, float volume)
{
	auto actualFilename = PathUtil::GetFileName(filename.Str());
	auto triggerID = CryAudio::StringToId(actualFilename);

	gEnv->pAudioSystem->ExecuteTrigger(triggerID);
}

static Noesis::MemoryCallbacks g_memCallbacks = {
	nullptr,
	&Allocator::Alloc,
	&Allocator::ReAlloc,
	&Allocator::DeAlloc,
	&Allocator::AllocSize
};

CImplementation::CImplementation()
	: m_pResourceDictVar(nullptr)
{
	RegisterVariables();

	m_pViewManager = std::make_unique<CViewManager>();

	Noesis::SetMemoryCallbacks(g_memCallbacks);
	Noesis::SetErrorHandler(&ErrorHandler);
	Noesis::SetLogHandler(&LogHandler);
	Noesis::SetAssertHandler(&AssertHandler);

	Noesis::SetLicense(NS_LICENSE_NAME, NS_LICENSE_KEY);

	Noesis::GUI::Init();

	Noesis::GUI::SetXamlProvider(Noesis::MakePtr<CXamlProvider>());
	Noesis::GUI::SetFontProvider(Noesis::MakePtr<CFontProvider>());
	Noesis::GUI::SetTextureProvider(Noesis::MakePtr<CTextureProvider>());

	const char* fonts[] = { "Segoe UI Emoji" };
	//GUI::LoadApplicationResources("MenuResources.xaml");
	Noesis::GUI::SetFontFallbacks(fonts, 1);
	Noesis::GUI::SetFontDefaultProperties(15, Noesis::FontWeight_Normal, Noesis::FontStretch_Normal, Noesis::FontStyle_Normal);

	Noesis::RegisterComponent<ViewContainer>();

	Noesis::GUI::SetPlayAudioCallback(nullptr, PlaySound);
	LoadResources();
}

CImplementation::~CImplementation()
{
	g_pImplementation.reset();
	Noesis::GUI::Shutdown();
}

void CImplementation::Init()
{
	m_startTime = gEnv->pTimer->GetFrameStartTime();
	m_pInputHandler = std::make_unique<CInputHandler>(this);

	m_pRenderDevice.Reset(new CRenderDevice());

	m_pRenderDevice->StartRenderer();
}

void CImplementation::Update(float delta)
{
}

void CImplementation::UpdateBeforeRender()
{
	auto curTime = gEnv->pTimer->GetFrameStartTime() - m_startTime;

	if (!m_pViewManager)
		return;

	for (auto& view : m_pViewManager->GetViews())
	{
		view.second.pView->Update(curTime.GetSeconds());
	}
}

void CImplementation::OnScreenSizeChanged()
{
	m_pViewManager->NotifyRendererSizeChange();
}

void CImplementation::RegisterVariables()
{
	ConsoleRegistrationHelper::AddCommand("Noesis.LoadXaml", &LoadXamlCmd);
	m_pResourceDictVar = ConsoleRegistrationHelper::RegisterString("Noesis.ResourceDictionary", "", 0, "Resource dictionary to load at startup.");
}

void CImplementation::LoadResources()
{
	string resources = m_pResourceDictVar->GetString();
	Noesis::GUI::LoadApplicationResources(resources.c_str());
}

bool CImplementation::CreateView(const char* xamlPath, Vec2i dimensions)
{
	m_pViewManager->CreateViewFromXaml(xamlPath);
	return true;
}

CViewManager* CImplementation::GetViewManager() const
{
	return &*m_pViewManager;
}

CRenderDevice* CImplementation::GetRenderDevice() const
{
	return &*m_pRenderDevice;
}
