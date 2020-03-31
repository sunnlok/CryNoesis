#pragma once
#include "Renderer/Renderer.h"


namespace Cry
{
namespace Ns
{
	class CImplementation
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

		bool CreateView(const char* xamlPath, Vec2i dimensions);
	private:
		float m_lastFrameDelta = 0;


		std::vector<Noesis::Ptr<Noesis::IView>> m_views;

		Noesis::Ptr<CRenderDevice> m_pRenderDevice;
	};
}
}