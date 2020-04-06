////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __APP_FILTERPREDICATE_H__
#define __APP_FILTERPREDICATE_H__


#include <NsCore/Noesis.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/Delegate.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsApp/InteractivityApi.h>
#include <NsGui/Events.h>


namespace Noesis
{
struct NotifyCollectionChangedEventArgs;
struct PropertyChangedEventArgs;
}

namespace NoesisApp
{

// Event handler type for FilterRequired event
typedef Noesis::Delegate<void()> FilterRequiredEventHandler;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Base class for Filter predicate object used by CollectionFilterBehavior.
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_APP_INTERACTIVITY_API FilterPredicate: public Noesis::BaseComponent
{
public:
    FilterPredicate();
    virtual ~FilterPredicate() = 0;

    /// Called to evaluate the predicate against the provided item
    virtual bool Matches(Noesis::BaseComponent* item) const = 0;

    /// Indicates if filter should be re-evaluated after an item property change
    virtual bool NeedsRefresh(Noesis::BaseComponent* item, const char* propertyName) const = 0;

    /// Re-evaluates the filter over the list
    void Refresh();

    /// Indicates that source collection is being updated, so we disable notifications to avoid
    /// refreshing the filter more than necessary
    void BeginUpdate();

    /// Indicates that source collection has finished the updating process, we can now re-evaluate
    /// the filter if it any changes had occurred
    void EndUpdate();

    /// Gets or sets the source collection of items
    //@{
    Noesis::BaseComponent* GetItemsSource() const;
    void SetItemsSource(Noesis::BaseComponent* value);

    /// Indicates that conditions changed and filter needs to be re-evaluated
    Noesis::DelegateEvent_<FilterRequiredEventHandler> FilterRequired();

private:
    void RegisterItemsSource(Noesis::BaseComponent* itemsSource);
    void UnregisterItemsSource(Noesis::BaseComponent* itemsSource);

    void RegisterItems(Noesis::BaseComponent* itemsSource);
    void UnregisterItems(Noesis::BaseComponent* itemsSource);
    void RegisterItem(Noesis::BaseComponent* item);
    void UnregisterItem(Noesis::BaseComponent* item);

    void OnItemsChanged(Noesis::BaseComponent* sender,
        const Noesis::NotifyCollectionChangedEventArgs& e);
    void OnItemChanged(Noesis::BaseComponent* sender,
        const Noesis::PropertyChangedEventArgs& e);

private:
    Noesis::Ptr<Noesis::BaseComponent> mItemsSource;
    FilterRequiredEventHandler mFilterRequired;
    bool mUpdatingSource;
    bool mSourceChanged;

    NS_DECLARE_REFLECTION(FilterPredicate, Noesis::BaseComponent)
};

NS_WARNING_POP

}


#endif
