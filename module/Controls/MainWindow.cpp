#include "StdAfx.h"
#include "MainWindow.h"

#include <NsGui/IntegrationAPI.h>
#include <NsCore/ReflectionImplementEmpty.h>


Cry::Ns::Components::MainMenu::MainMenu()
{
	Loaded() += Noesis::MakeDelegate(this, &MainMenu::OnLoaded);
	InitializeComponent();
}

void Cry::Ns::Components::MainMenu::InitializeComponent()
{
	Noesis::GUI::LoadComponent(this, "UI/MainMenu.xaml");
}

void Cry::Ns::Components::MainMenu::OnLoaded(Noesis::BaseComponent*, const Noesis::RoutedEventArgs&)
{
	CryLogAlways("Blirblub");
}

NS_IMPLEMENT_REFLECTION_(Cry::Ns::Components::MainMenu, "Cry.MainMenu")