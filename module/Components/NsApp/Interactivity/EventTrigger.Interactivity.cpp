////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsApp/EventTrigger.h>
#include <NsGui/UIElementData.h>
#include <NsGui/FrameworkElement.h>
#include <NsCore/ReflectionImplement.h>
#include <NsCore/String.h>


using namespace NoesisApp;


////////////////////////////////////////////////////////////////////////////////////////////////////
NoesisApp::EventTrigger::EventTrigger()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
NoesisApp::EventTrigger::EventTrigger(const char* eventName)
{
    ForceCreateDependencyProperties();
    SetEventName(eventName);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
NoesisApp::EventTrigger::~EventTrigger()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
const char* NoesisApp::EventTrigger::GetEventName() const
{
    return GetValue<Noesis::String>(EventNameProperty).Str();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void NoesisApp::EventTrigger::SetEventName(const char* name)
{
    SetValue<Noesis::String>(EventNameProperty, name);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Noesis::Ptr<NoesisApp::EventTrigger> NoesisApp::EventTrigger::Clone() const
{
    return Noesis::StaticPtrCast<EventTrigger>(Freezable::Clone());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Noesis::Ptr<NoesisApp::EventTrigger> NoesisApp::EventTrigger::CloneCurrentValue() const
{
    return Noesis::StaticPtrCast<EventTrigger>(Freezable::CloneCurrentValue());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Noesis::Ptr<Noesis::Freezable> NoesisApp::EventTrigger::CreateInstanceCore() const
{
    return *new EventTrigger();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void NoesisApp::EventTrigger::StaticOnEventNameChanged(DependencyObject* d,
    const Noesis::DependencyPropertyChangedEventArgs& e)
{
    const Noesis::String& oldName = *static_cast<const Noesis::String*>(e.oldValue);
    const Noesis::String& newName = *static_cast<const Noesis::String*>(e.newValue);

    EventTrigger* trigger = static_cast<EventTrigger*>(d);
    trigger->OnEventNameChanged(oldName.Str(), newName.Str());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
NS_BEGIN_COLD_REGION

NS_IMPLEMENT_REFLECTION(NoesisApp::EventTrigger, "NoesisApp.EventTrigger")
{
    Noesis::DependencyData* data = NsMeta<Noesis::DependencyData>(Noesis::TypeOf<SelfClass>());
    data->RegisterProperty<Noesis::String>(EventNameProperty, "EventName",
        Noesis::PropertyMetadata::Create(Noesis::String("Loaded"),
            Noesis::PropertyChangedCallback(StaticOnEventNameChanged)));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
const Noesis::DependencyProperty* NoesisApp::EventTrigger::EventNameProperty;
