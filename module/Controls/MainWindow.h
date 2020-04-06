#pragma once

#include <NsCore/Noesis.h>
#include <NsGui/UserControl.h>


namespace Cry::Ns::Components
{
	class MainMenu final : public ::Noesis::UserControl
	{
	public:
		MainMenu();
	private:
		void InitializeComponent();
		void OnLoaded(Noesis::BaseComponent*, const Noesis::RoutedEventArgs&);

		NS_DECLARE_REFLECTION(MainMenu, Noesis::UserControl)
	};
}