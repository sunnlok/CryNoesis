#include "StdAfx.h"
#include "Interfaces/NoesisSDK.h"

#include "ViewContainer.h"

#include "StdAfx.h"
#include "ViewContainer.h"

ViewContainer::ViewContainer()
{
	InitializeComponent();
}

void ViewContainer::InitializeComponent()
{
}

void ViewContainer::OnLoaded(Noesis::BaseComponent*, const Noesis::RoutedEventArgs&)
{
}

NS_IMPLEMENT_REFLECTION_(ViewContainer, "ViewContainer")