////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsApp/StoryboardCompletedTrigger.h>
#include <NsGui/Storyboard.h>
#include <NsGui/ResourceDictionary.h>
#include <NsCore/ReflectionImplementEmpty.h>


using namespace NoesisApp;


////////////////////////////////////////////////////////////////////////////////////////////////////
StoryboardCompletedTrigger::StoryboardCompletedTrigger()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
StoryboardCompletedTrigger::~StoryboardCompletedTrigger()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Noesis::Ptr<StoryboardCompletedTrigger> StoryboardCompletedTrigger::Clone() const
{
    return Noesis::StaticPtrCast<StoryboardCompletedTrigger>(Freezable::Clone());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Noesis::Ptr<StoryboardCompletedTrigger> StoryboardCompletedTrigger::CloneCurrentValue() const
{
    return Noesis::StaticPtrCast<StoryboardCompletedTrigger>(Freezable::CloneCurrentValue());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Noesis::Ptr<Noesis::Freezable> StoryboardCompletedTrigger::CreateInstanceCore() const
{
    return *new StoryboardCompletedTrigger();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void StoryboardCompletedTrigger::CloneCommonCore(const Freezable* source)
{
    ParentClass::CloneCommonCore(source);

    StoryboardCompletedTrigger* trigger = (StoryboardCompletedTrigger*)source;
    Noesis::Storyboard* storyboard = trigger->GetStoryboard();
    if (Noesis::DynamicCast<Noesis::ResourceDictionary*>(storyboard->GetNodeParent()) != 0)
    {
        // use original Storyboard resource, not a clone
        SetStoryboard(storyboard);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void StoryboardCompletedTrigger::OnDetaching()
{
    Noesis::Storyboard* storyboard = GetStoryboard();
    if (storyboard != 0)
    {
        storyboard->Completed() -= MakeDelegate(this,
            &StoryboardCompletedTrigger::OnStoryboardCompleted);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void StoryboardCompletedTrigger::OnStoryboardChanged(const Noesis::DependencyPropertyChangedEventArgs& e)
{
    Noesis::Storyboard* oldStoryboard = static_cast<const Noesis::Ptr<Noesis::Storyboard>*>(e.oldValue)->GetPtr();
    Noesis::Storyboard* newStoryboard = static_cast<const Noesis::Ptr<Noesis::Storyboard>*>(e.newValue)->GetPtr();

    if (oldStoryboard != 0)
    {
        oldStoryboard->Completed() -= MakeDelegate(this,
            &StoryboardCompletedTrigger::OnStoryboardCompleted);
    }
    if (newStoryboard != 0)
    {
        newStoryboard->Completed() += MakeDelegate(this,
            &StoryboardCompletedTrigger::OnStoryboardCompleted);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void StoryboardCompletedTrigger::OnStoryboardCompleted(BaseComponent*, const Noesis::TimelineEventArgs&)
{
    InvokeActions(0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
NS_BEGIN_COLD_REGION

NS_IMPLEMENT_REFLECTION_(StoryboardCompletedTrigger, "NoesisApp.StoryboardCompletedTrigger")
