////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __APP_STYLEINTERACTION_H__
#define __APP_STYLEINTERACTION_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsApp/InteractivityApi.h>
#include <NsApp/Behavior.h>
#include <NsApp/TriggerBase.h>
#include <NsGui/FreezableCollection.h>


namespace Noesis
{
class DependencyObject;
class DependencyProperty;
struct DependencyPropertyChangedEventArgs;
}

namespace NoesisApp
{

////////////////////////////////////////////////////////////////////////////////////////////////////
class StyleBehaviorCollection: public Noesis::FreezableCollection<Behavior>
{
    NS_DECLARE_REFLECTION(StyleBehaviorCollection, Noesis::BaseFreezableCollection)
};

////////////////////////////////////////////////////////////////////////////////////////////////////
class StyleTriggerCollection: public Noesis::FreezableCollection<TriggerBase>
{
    NS_DECLARE_REFLECTION(StyleTriggerCollection, Noesis::BaseFreezableCollection)
};

NS_WARNING_PUSH
NS_CLANG_WARNING_DISABLE("-Wdocumentation")

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Allows setting a collection of *Interactivity* behaviors and triggers in a Style, so they are
/// applied to all instances of the styled control.
///
/// .. code-block:: xml
///
///    <Grid
///      xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
///      xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
///      xmlns:i="http://schemas.microsoft.com/expression/2010/interactivity"
///      xmlns:ei="http://schemas.microsoft.com/expression/2010/interactions"
///      xmlns:noesis="clr-namespace:NoesisGUIExtensions;assembly=Noesis.GUI.Extensions">
///      <Grid.Resources>
///        <Style TargetType="Button" BasedOn="{StaticResource {x:Type Button}}">
///          <Setter Property="noesis:StyleInteraction.Triggers">
///            <Setter.Value>
///              <noesis:StyleTriggerCollection>
///                <i:EventTrigger EventName="Click">
///                  <ei:PlaySoundAction Source="Sounds/ButtonClick.wav" />
///                </i:EventTrigger>
///              </noesis:StyleTriggerCollection>
///            </Setter.Value>
///          </Setter>
///        </Style>
///      </Grid.Resources>
///      ...
///    </Grid>
////////////////////////////////////////////////////////////////////////////////////////////////////
struct NS_APP_INTERACTIVITY_API StyleInteraction
{
    static const Noesis::DependencyProperty* BehaviorsProperty;
    static const Noesis::DependencyProperty* TriggersProperty;

private:
    static void OnBehaviorsChanged(Noesis::DependencyObject* d,
        const Noesis::DependencyPropertyChangedEventArgs& e);
    static void OnTriggersChanged(Noesis::DependencyObject* d,
        const Noesis::DependencyPropertyChangedEventArgs& e);

    NS_DECLARE_REFLECTION(StyleInteraction, Noesis::NoParent);
};

NS_WARNING_POP

}


#endif
