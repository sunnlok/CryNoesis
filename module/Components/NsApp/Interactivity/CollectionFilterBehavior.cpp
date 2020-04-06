////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsApp/CollectionFilterBehavior.h>
#include <NsApp/FilterPredicate.h>
#include <NsGui/ObservableCollection.h>
#include <NsGui/DependencyData.h>
#include <NsGui/PropertyMetadata.h>
#include <NsCore/ReflectionImplement.h>


using namespace NoesisApp;


////////////////////////////////////////////////////////////////////////////////////////////////////
CollectionFilterBehavior::CollectionFilterBehavior()
{
    SetReadOnlyProperty<Noesis::Ptr<FilteredCollection>>(FilteredItemsProperty,
        Noesis::MakePtr<FilteredCollection>());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
CollectionFilterBehavior::~CollectionFilterBehavior()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
FilterPredicate* CollectionFilterBehavior::GetPredicate() const
{
    return GetValue<Noesis::Ptr<FilterPredicate>>(PredicateProperty);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CollectionFilterBehavior::SetPredicate(FilterPredicate* value)
{
    SetValue<Noesis::Ptr<FilterPredicate>>(PredicateProperty, value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Noesis::BaseComponent* CollectionFilterBehavior::GetItemsSource() const
{
    return GetValue<Noesis::Ptr<Noesis::BaseComponent>>(ItemsSourceProperty);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CollectionFilterBehavior::SetItemsSource(Noesis::BaseComponent* value)
{
    SetValue<Noesis::Ptr<Noesis::BaseComponent>>(ItemsSourceProperty, value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
FilteredCollection* CollectionFilterBehavior::GetFilteredItems()
{
    return GetValue<Noesis::Ptr<FilteredCollection>>(FilteredItemsProperty);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Noesis::Ptr<Noesis::Freezable> CollectionFilterBehavior::CreateInstanceCore() const
{
    return *new CollectionFilterBehavior();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CollectionFilterBehavior::FilterItems()
{
    Noesis::BaseComponent* source = GetItemsSource();
    Noesis::IList* list = Noesis::DynamicCast<Noesis::IList*>(source);
    if (list == nullptr)
    {
        GetFilteredItems()->Clear();
        return;
    }

    int numItems = list->Count();
    if (numItems == 0)
    {
        GetFilteredItems()->Clear();
        return;
    }

    FilterPredicate* predicate = GetPredicate();
    Noesis::Ptr<FilteredCollection> filtered = Noesis::MakePtr<FilteredCollection>();

    for (int i = 0; i < numItems; ++i)
    {
        Noesis::BaseComponent* item = list->GetComponent(i);
        if (predicate == nullptr || predicate->Matches(item))
        {
            filtered->Add(item);
        }
    }

    SetReadOnlyProperty<Noesis::Ptr<FilteredCollection>>(FilteredItemsProperty, filtered);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CollectionFilterBehavior::OnFilterRequired()
{
    FilterItems();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CollectionFilterBehavior::RegisterPredicate(FilterPredicate* predicate)
{
    if (predicate != nullptr)
    {
        predicate->SetItemsSource(GetItemsSource());
        predicate->FilterRequired() += MakeDelegate(this,
            &CollectionFilterBehavior::OnFilterRequired);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CollectionFilterBehavior::UnregisterPredicate(FilterPredicate* predicate)
{
    if (predicate != nullptr)
    {
        predicate->FilterRequired() -= MakeDelegate(this,
            &CollectionFilterBehavior::OnFilterRequired);
        predicate->SetItemsSource(nullptr);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CollectionFilterBehavior::OnPredicateChanged(Noesis::DependencyObject*  d,
    const Noesis::DependencyPropertyChangedEventArgs& e)
{
    CollectionFilterBehavior* behavior = (CollectionFilterBehavior*)d;

    FilterPredicate* oldPredicate = static_cast<const Noesis::Ptr<FilterPredicate>*>(e.oldValue)->GetPtr();
    behavior->UnregisterPredicate(oldPredicate);

    FilterPredicate* newPredicate = static_cast<const Noesis::Ptr<FilterPredicate>*>(e.newValue)->GetPtr();
    behavior->RegisterPredicate(newPredicate);

    behavior->FilterItems();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CollectionFilterBehavior::OnItemsSourceChanged(Noesis::DependencyObject*  d,
    const Noesis::DependencyPropertyChangedEventArgs& e)
{
    CollectionFilterBehavior* behavior = (CollectionFilterBehavior*)d;

    FilterPredicate* predicate = behavior->GetPredicate();
    if (predicate != nullptr)
    {
        Noesis::BaseComponent* source = static_cast<const Noesis::Ptr<Noesis::BaseComponent>*>(e.newValue)->GetPtr();
        predicate->SetItemsSource(source);
    }

    behavior->FilterItems();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
NS_BEGIN_COLD_REGION

NS_IMPLEMENT_REFLECTION(CollectionFilterBehavior, "NoesisGUIExtensions.CollectionFilterBehavior")
{
    Noesis::DependencyData* data = NsMeta<Noesis::DependencyData>(Noesis::TypeOf<SelfClass>());
    data->RegisterProperty<Noesis::Ptr<FilterPredicate>>(PredicateProperty, "Predicate",
        Noesis::PropertyMetadata::Create(Noesis::Ptr<FilterPredicate>(),
            Noesis::PropertyChangedCallback(OnPredicateChanged)));
    data->RegisterProperty<Noesis::Ptr<Noesis::BaseComponent>>(ItemsSourceProperty, "ItemsSource",
        Noesis::PropertyMetadata::Create(Noesis::Ptr<Noesis::BaseComponent>(),
            Noesis::PropertyChangedCallback(OnItemsSourceChanged)));
    data->RegisterProperty<Noesis::Ptr<FilteredCollection>>(FilteredItemsProperty, "FilteredItems",
        Noesis::PropertyMetadata::Create(Noesis::Ptr<FilteredCollection>()));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
const Noesis::DependencyProperty* CollectionFilterBehavior::PredicateProperty;
const Noesis::DependencyProperty* CollectionFilterBehavior::ItemsSourceProperty;
const Noesis::DependencyProperty* CollectionFilterBehavior::FilteredItemsProperty;
