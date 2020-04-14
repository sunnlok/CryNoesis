#pragma once
#include "NsCore/Ptr.h"
#include "IViewManager.h"

namespace Noesis
{
	struct IView;
}

namespace Cry::Ns
{
	struct ViewRenderData;

	struct ViewData : ViewDataBase
	{
		::Noesis::Ptr<::Noesis::IView> pView;

		std::unique_ptr<ViewRenderData> pViewRenderData;
	};

	class ViewManager final : public IViewManager
	{
	public:
		static ViewManager* Get();

		ViewManager();
		~ViewManager();

		void CreateViewFromXaml(const char* uri);

		void RemoveView(uint16 viewID);

		void NotifyRendererSizeChange();

		void SetViewActivated(uint16 viewID, bool bActivated);

		void SetViewActivated(const ViewDataBase& view, bool bActivated) final;

		const ViewData* GetViewData(uint16 viewID) final;
		const ViewData* FindViewData(const char* name) final;

		const std::vector<ViewData>& GetViews() { return m_views; }
	protected:
		ViewData& CreateNewViewData();
		

		std::vector<ViewData> m_views;


		std::vector<uint16> m_freeIds;
		uint16 m_nextID = 1;
	};
}