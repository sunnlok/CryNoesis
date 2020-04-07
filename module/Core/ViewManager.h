#pragma once
#include "NsCore/Ptr.h"
#include "CryCore/CryEnumMacro.h"

namespace Noesis
{
	struct IView;
}

namespace Cry::Ns
{
	struct ViewRenderData;

	enum class ViewFlags : uint8
	{
		ScaleWithView = 1,
		MainView = 2,

	};
	CRY_CREATE_ENUM_FLAG_OPERATORS(ViewFlags);

	struct ViewData
	{
		uint16 id;
		string name;
		
		uint32 viewWidth;
		uint32 viewHeight;

		ViewFlags flags;

		::Noesis::Ptr<::Noesis::IView> pView;

		std::unique_ptr<ViewRenderData> pViewRenderData;

		bool operator==(const ViewData& other) { return other.id == id; }
		bool operator==(const uint16& otherID) { return otherID == id; }
		bool operator==(const string& otherName) { return otherName == name; }
	};

	class ViewManager
	{
	public:
		static ViewManager* Get();

		ViewManager();
		~ViewManager();

		void CreateViewFromXaml(const char* uri);

		void RemoveView(uint16 viewID);

		void NotifyRendererSizeChange();

		void SetViewActivated(uint16 viewID, bool bActivated);

		const std::vector<ViewData>& GetViews() { return m_views; }
	protected:
		ViewData& CreateNewViewData();
		

		std::vector<ViewData> m_views;


		std::vector<uint16> m_freeIds;
		uint16 m_nextID = 1;
	};
}