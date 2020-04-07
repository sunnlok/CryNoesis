#include "StdAfx.h"
#include "ViewContainer.h"

#include <NsGui/IntegrationAPI.h>
#include <NsCore/ReflectionImplementEmpty.h>


Cry::Ns::Components::ViewContainer::ViewContainer()
{
	InitializeComponent();
}

void Cry::Ns::Components::ViewContainer::InitializeComponent()
{
	
}

void Cry::Ns::Components::ViewContainer::OnLoaded(Noesis::BaseComponent*, const Noesis::RoutedEventArgs&)
{
	
}

NS_IMPLEMENT_REFLECTION_(Cry::Ns::Components::ViewContainer, "ViewContainer")