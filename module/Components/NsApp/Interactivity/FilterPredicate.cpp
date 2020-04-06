////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsApp/FilterPredicate.h>
#include <NsGui/INotifyCollectionChanged.h>
#include <NsGui/INotifyPropertyChanged.h>
#include <NsGui/IList.h>
#include <NsCore/ReflectionImplement.h>


using namespace NoesisApp;


////////////////////////////////////////////////////////////////////////////////////////////////////
FilterPredicate::FilterPredicate() : mUpdatingSource(false), mSourceChanged(false)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
FilterPredicate::~FilterPredicate()
{
    UnregisterItemsSource(mItemsSource);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void FilterPredicate::Refresh()
{
    mUpdatingSource = false;
    mSourceChanged = false;

    mFilterRequired();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void FilterPredicate::BeginUpdate()
{
    mUpdatingSource = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void FilterPredicate::EndUpdate()
{
    mUpdatingSource = false;

    if (mSourceChanged)
    {
        Refresh();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Noesis::BaseComponent* FilterPredicate::GetItemsSource() const
{
    return mItemsSource;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void FilterPredicate::SetItemsSource(Noesis::BaseComponent* value)
{
    UnregisterItemsSource(mItemsSource);
    mItemsSource.Reset(value);
    RegisterItemsSource(mItemsSource);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Noesis::DelegateEvent_<FilterRequiredEventHandler> FilterPredicate::FilterRequired()
{
    return mFilterRequired;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void FilterPredicate::RegisterItemsSource(Noesis::BaseComponent* itemsSource)
{
    Noesis::INotifyCollectionChanged* notify = Noesis::DynamicCast<Noesis::INotifyCollectionChanged*>(itemsSource);
    if (notify != nullptr)
    {
        notify->CollectionChanged() += MakeDelegate(this, &FilterPredicate::OnItemsChanged);
    }

    RegisterItems(mItemsSource);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void FilterPredicate::UnregisterItemsSource(Noesis::BaseComponent* itemsSource)
{
    UnregisterItems(mItemsSource);

    Noesis::INotifyCollectionChanged* notify = Noesis::DynamicCast<Noesis::INotifyCollectionChanged*>(itemsSource);
    if (notify != nullptr)
    {
        notify->CollectionChanged() -= MakeDelegate(this, &FilterPredicate::OnItemsChanged);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void FilterPredicate::RegisterItems(Noesis::BaseComponent* itemsSource)
{
    Noesis::IList* items = Noesis::DynamicCast<Noesis::IList*>(itemsSource);
    if (items != nullptr)
    {
        int numItems = items->Count();
        for (int i = 0; i < numItems; ++i)
        {
            RegisterItem(items->GetComponent(i));
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void FilterPredicate::UnregisterItems(Noesis::BaseComponent* itemsSource)
{
    Noesis::IList* items = Noesis::DynamicCast<Noesis::IList*>(itemsSource);
    if (items != nullptr)
    {
        int numItems = items->Count();
        for (int i = 0; i < numItems; ++i)
        {
            UnregisterItem(items->GetComponent(i));
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void FilterPredicate::RegisterItem(Noesis::BaseComponent* item)
{
    Noesis::INotifyPropertyChanged* notify = Noesis::DynamicCast<Noesis::INotifyPropertyChanged*>(item);
    if (notify != nullptr)
    {
        notify->PropertyChanged() += MakeDelegate(this, &FilterPredicate::OnItemChanged);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void FilterPredicate::UnregisterItem(Noesis::BaseComponent* item)
{
    Noesis::INotifyPropertyChanged* notify = Noesis::DynamicCast<Noesis::INotifyPropertyChanged*>(item);
    if (notify != nullptr)
    {
        notify->PropertyChanged() -= MakeDelegate(this, &FilterPredicate::OnItemChanged);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void FilterPredicate::OnItemsChanged(Noesis::BaseComponent*, const Noesis::NotifyCollectionChangedEventArgs& e)
{
    switch (e.action)
    {
        case Noesis::NotifyCollectionChangedAction_Add:
        {
            RegisterItem(e.newValue);
            mSourceChanged = true;
            break;
        }
        case Noesis::NotifyCollectionChangedAction_Remove:
        {
            UnregisterItem(e.oldValue);
            mSourceChanged = true;
            break;
        }
        case Noesis::NotifyCollectionChangedAction_Replace:
        {
            UnregisterItem(e.oldValue);
            RegisterItem(e.newValue);
            mSourceChanged = true;
            break;
        }
        case Noesis::NotifyCollectionChangedAction_Move:
        {
            break;
        }
        case Noesis::NotifyCollectionChangedAction_Reset:
        {
            RegisterItems(mItemsSource);
            mSourceChanged = true;
            break;
        }
        case Noesis::NotifyCollectionChangedAction_PreReset:
        {
            UnregisterItems(mItemsSource);
            break;
        }
        default: NS_ASSERT_UNREACHABLE;
    }

    if (!mUpdatingSource && mSourceChanged)
    {
        Refresh();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void FilterPredicate::OnItemChanged(Noesis::BaseComponent* sender, const Noesis::PropertyChangedEventArgs& e)
{
    if (NeedsRefresh(sender, e.propertyName.Str()))
    {
        mSourceChanged = true;

        if (!mUpdatingSource)
        {
            Refresh();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
NS_BEGIN_COLD_REGION

NS_IMPLEMENT_REFLECTION_(FilterPredicate)
