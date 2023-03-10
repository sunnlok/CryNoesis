#pragma once

#include <NsCore/Noesis.h>
#include <NsGui/UserControl.h>


class ViewContainer final : public ::Noesis::UserControl
{
public:
	ViewContainer();
private:
	void InitializeComponent();
	void OnLoaded(Noesis::BaseComponent*, const Noesis::RoutedEventArgs&);

	NS_DECLARE_REFLECTION(ViewContainer, Noesis::UserControl)
};