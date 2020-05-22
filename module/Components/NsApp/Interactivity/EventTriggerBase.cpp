#include "StdAfx.h" 
////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsApp/EventTriggerBase.h>
#include <NsGui/UIElementData.h>
#include <NsGui/FrameworkElement.h>
#include <NsCore/ReflectionImplement.h>
#include <NsCore/String.h>


using namespace NoesisApp;


////////////////////////////////////////////////////////////////////////////////////////////////////
EventTriggerBase::EventTriggerBase(const Noesis::TypeClass* sourceType):
    TriggerBase(Noesis::TypeOf<DependencyObject>()), mSourceType(sourceType), mSource(0)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
EventTriggerBase::~EventTriggerBase()
{
    if (GetAssociatedObject() != 0)
    {
        UnregisterEvent();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Noesis::BaseComponent* EventTriggerBase::GetSourceObject() const
{
    return GetValue<Noesis::Ptr<BaseComponent>>(SourceObjectProperty);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void EventTriggerBase::SetSourceObject(BaseComponent* source)
{
    SetValue<Noesis::Ptr<BaseComponent>>(SourceObjectProperty, source);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
const char* EventTriggerBase::GetSourceName() const
{
    return GetValue<Noesis::String>(SourceNameProperty).Str();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void EventTriggerBase::SetSourceName(const char* name)
{
    SetValue<Noesis::String>(SourceNameProperty, name);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Noesis::Ptr<EventTriggerBase> EventTriggerBase::Clone() const
{
    return Noesis::StaticPtrCast<EventTriggerBase>(Freezable::Clone());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Noesis::Ptr<EventTriggerBase> EventTriggerBase::CloneCurrentValue() const
{
    return Noesis::StaticPtrCast<EventTriggerBase>(Freezable::CloneCurrentValue());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Noesis::BaseComponent* EventTriggerBase::GetSource() const
{
    return mSource;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void EventTriggerBase::OnSourceChangedImpl(BaseComponent*, BaseComponent* newSource)
{
    UnregisterEvent();
    RegisterEvent(newSource, GetEventName());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void EventTriggerBase::OnEventNameChanged(const char*, const char* newName)
{
    UnregisterEvent();
    RegisterEvent(mSource, newName);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void EventTriggerBase::OnEvent()
{
    InvokeActions(0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void EventTriggerBase::OnAttached()
{
    ParentClass::OnAttached();

    UpdateSource(GetAssociatedObject());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void EventTriggerBase::OnDetaching()
{
    UpdateSource(0);

    ParentClass::OnDetaching();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void EventTriggerBase::UpdateSource(DependencyObject* associatedObject)
{
    BaseComponent* oldSource = mSource;
    BaseComponent* newSource = associatedObject;

    if (associatedObject != 0)
    {
        BaseComponent* sourceObject = GetSourceObject();
        const char* sourceName = GetSourceName();

        if (sourceObject != 0)
        {
            newSource = sourceObject;
        }
        else if (!Noesis::StrIsNullOrEmpty(sourceName))
        {
            Noesis::FrameworkElement* element = Noesis::DynamicCast<Noesis::FrameworkElement*>(associatedObject);
            BaseComponent* found = element != 0 ? element->FindName(sourceName) : 0;
            if (found != 0)
            {
                newSource = found;
            }
        }
    }

    if (oldSource != newSource)
    {
        if (newSource == 0 || mSourceType->IsAssignableFrom(newSource->GetClassType()))
        {
            mSource = newSource;

            if (GetAssociatedObject() != 0)
            {
                OnSourceChangedImpl(oldSource, newSource);
            }
        }
        else
        {
            NS_ERROR("%s invalid source type '%s'", GetClassType()->GetName(),
                newSource->GetClassType()->GetName());
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void EventTriggerBase::RegisterEvent(BaseComponent* source, const char* eventName)
{
    if (source != 0 && !Noesis::StrIsNullOrEmpty(eventName))
    {
        Noesis::Symbol eventId = Noesis::Symbol(eventName);
        const Noesis::TypeClass* type = source->GetClassType();

        Noesis::UIElement* element = Noesis::DynamicCast<Noesis::UIElement*>(source);
        if (element != 0)
        {
            const Noesis::RoutedEvent* routedEvent = FindRoutedEvent(eventId, type);
            if (routedEvent != 0)
            {
                element->AddHandler(routedEvent, MakeDelegate(this,
                    &EventTriggerBase::OnRoutedEvent));
                element->Destroyed() += MakeDelegate(this,
                    &EventTriggerBase::OnSourceDestroyed);

                mUnregisterEvent = [this, element, routedEvent]()
                {
                    element->RemoveHandler(routedEvent, MakeDelegate(this,
                        &EventTriggerBase::OnRoutedEvent));
                    element->Destroyed() -= MakeDelegate(this,
                        &EventTriggerBase::OnSourceDestroyed);
                };

                return;
            }
        }

        const Noesis::TypeProperty* event = FindEvent(eventId, type);
        if (event != 0)
        {
            Noesis::EventHandler& handler = *(Noesis::EventHandler*)event->GetContent(source);
            handler += MakeDelegate(this, &EventTriggerBase::OnDelegateEvent);

            DependencyObject* dob = Noesis::DynamicCast<DependencyObject*>(source);
            if (dob != 0)
            {
                dob->Destroyed() += MakeDelegate(this, &EventTriggerBase::OnSourceDestroyed);
            }
            else
            {
                source->AddReference();
            }

            mUnregisterEvent = [this, source, &handler]()
            {
                handler -= MakeDelegate(this, &EventTriggerBase::OnDelegateEvent);

                DependencyObject* d = Noesis::DynamicCast<DependencyObject*>(source);
                if (d != 0)
                {
                    d->Destroyed() -= MakeDelegate(this, &EventTriggerBase::OnSourceDestroyed);
                }
                else
                {
                    source->Release();
                }
            };
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void EventTriggerBase::UnregisterEvent()
{
    mUnregisterEvent();
    mUnregisterEvent = UnregisterEventDelegate();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void EventTriggerBase::OnSourceDestroyed(DependencyObject*)
{
    UnregisterEvent();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
const Noesis::RoutedEvent* EventTriggerBase::FindRoutedEvent(Noesis::Symbol eventName,
    const Noesis::TypeClass* type)
{
    const Noesis::TypeClass* elementType = Noesis::TypeOf<Noesis::UIElement>();
    while (type != 0)
    {
        const Noesis::UIElementData* metadata = type->GetMetaData().Find<Noesis::UIElementData>();

        if (metadata != 0)
        {
            const Noesis::RoutedEvent* event = metadata->FindEvent(eventName);
            if (event != 0)
            {
                return event;
            }
        }

        if (type == elementType)
        {
            break;
        }

        type = type->GetBase();
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void EventTriggerBase::OnRoutedEvent(BaseComponent*, const Noesis::RoutedEventArgs&)
{
    OnEvent();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
const Noesis::TypeProperty* EventTriggerBase::FindEvent(Noesis::Symbol eventName,
    const Noesis::TypeClass* type)
{
    while (type != 0)
    {
        const Noesis::TypeProperty* event = type->FindEvent(eventName);
        if (event != 0)
        {
            return event;
        }

        type = type->GetBase();
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void EventTriggerBase::OnDelegateEvent(BaseComponent*, const Noesis::EventArgs&)
{
    OnEvent();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void EventTriggerBase::OnSourceObjectChanged(DependencyObject* d,
    const Noesis::DependencyPropertyChangedEventArgs&)
{
    EventTriggerBase* trigger = static_cast<EventTriggerBase*>(d);
    trigger->UpdateSource(trigger->GetAssociatedObject());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void EventTriggerBase::OnSourceNameChanged(DependencyObject* d,
    const Noesis::DependencyPropertyChangedEventArgs&)
{
    EventTriggerBase* trigger = static_cast<EventTriggerBase*>(d);
    trigger->UpdateSource(trigger->GetAssociatedObject());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
NS_BEGIN_COLD_REGION

NS_IMPLEMENT_REFLECTION(EventTriggerBase, "NoesisApp.EventTriggerBase")
{
    Noesis::DependencyData* data = NsMeta<Noesis::DependencyData>(Noesis::TypeOf<SelfClass>());
    data->RegisterProperty<Noesis::Ptr<BaseComponent>>(SourceObjectProperty, "SourceObject",
        Noesis::PropertyMetadata::Create(Noesis::Ptr<BaseComponent>(),
            Noesis::PropertyChangedCallback(OnSourceObjectChanged)));
    data->RegisterProperty<Noesis::String>(SourceNameProperty, "SourceName",
        Noesis::PropertyMetadata::Create(Noesis::String(),
            Noesis::PropertyChangedCallback(OnSourceNameChanged)));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
const Noesis::DependencyProperty* EventTriggerBase::SourceObjectProperty;
const Noesis::DependencyProperty* EventTriggerBase::SourceNameProperty;

