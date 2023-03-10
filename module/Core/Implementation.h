#pragma once

#include "Renderer/Renderer.h"
#include "Interfaces/INoesis.h"
#include "ViewManager.h"

class CInputHandler;

using TViewList = std::vector<Noesis::Ptr<Noesis::IView>>;

class CImplementation final : public INoesis
{
public:
	static CImplementation* Instantiate();
	static void Destroy();
	static CImplementation* Get();
public:
	CImplementation();
	~CImplementation();

	void Init();

	void Update(float delta);
	void UpdateBeforeRender();
	void OnScreenSizeChanged();

	void RegisterVariables();
	void LoadResources();

	bool CreateView(const char* xamlPath, Vec2i dimensions);

	virtual CViewManager* GetViewManager() const final;

	CRenderDevice* GetRenderDevice() const;

private:
	ICVar* m_pResourceDictVar;

	Noesis::Ptr<CRenderDevice> m_pRenderDevice;
	std::unique_ptr<CInputHandler> m_pInputHandler;
	std::unique_ptr<CViewManager> m_pViewManager;

	float m_lastFrameDelta = 0.f;
	CTimeValue m_startTime;
};