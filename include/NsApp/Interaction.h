////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __APP_INTERACTION_H__
#define __APP_INTERACTION_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsApp/InteractivityApi.h>


namespace Noesis
{
class DependencyObject;
class DependencyProperty;
struct DependencyPropertyChangedEventArgs;
}

namespace NoesisApp
{

class BehaviorCollection;
class TriggerCollection;

NS_WARNING_PUSH
NS_CLANG_WARNING_DISABLE("-Wdocumentation")

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Manages a collection of behaviors and triggers that expand the object functionality from XAML.
///
/// .. code-block:: xml
///
///    <UserControl
///      xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
///      xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
///      xmlns:i="http://schemas.microsoft.com/expression/2010/interactivity"
///      xmlns:ei="http://schemas.microsoft.com/expression/2010/interactions">
///      <i:Interaction.Behaviors>
///        <ei:MouseDragElementBehavior />
///      <i:Interaction.Behaviors>
///      <i:Interaction.Triggers>
///        <i:EventTrigger EventName="Loaded">
///          <ei:GoToStateAction StateName="Init" />
///        </i:EventTrigger>
///      <i:Interaction.Triggers>
///    </UserControl>
////////////////////////////////////////////////////////////////////////////////////////////////////
struct NS_APP_INTERACTIVITY_API Interaction
{
    /// Gets the value of the Behaviors attached property
    static BehaviorCollection* GetBehaviors(const Noesis::DependencyObject* obj);

    /// Gets the value of the Triggers attached property
    static TriggerCollection* GetTriggers(const Noesis::DependencyObject* obj);

private:
    static void OnBehaviorsChanged(Noesis::DependencyObject* d,
        const Noesis::DependencyPropertyChangedEventArgs& e);
    static void OnTriggersChanged(Noesis::DependencyObject* d,
        const Noesis::DependencyPropertyChangedEventArgs& e);

    static bool CoerceBehaviors(const Noesis::DependencyObject* d, const void* value,
        void* coercedValue);
    static bool CoerceTriggers(const Noesis::DependencyObject* d, const void* value,
        void* coercedValue);

private:
    static const Noesis::DependencyProperty* BehaviorsProperty; // Attached
    static const Noesis::DependencyProperty* TriggersProperty; // Attached

    NS_DECLARE_REFLECTION(Interaction, Noesis::NoParent)
};

NS_WARNING_POP

}


#endif
