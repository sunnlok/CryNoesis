#pragma once

#include <NsCore/Noesis.h>
#include <NsGui/UserControl.h>


namespace Cry::Ns::Components
{
	class ViewContainer final : public ::Noesis::UserControl
	{
	public:
		ViewContainer();
	private:
		void InitializeComponent();
		void OnLoaded(Noesis::BaseComponent*, const Noesis::RoutedEventArgs&);

		NS_DECLARE_REFLECTION(ViewContainer, Noesis::UserControl)
	};
}