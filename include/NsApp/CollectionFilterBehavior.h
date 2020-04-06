////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __APP_COLLECTIONFILTERBEHAVIOR_H__
#define __APP_COLLECTIONFILTERBEHAVIOR_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsApp/InteractivityApi.h>
#include <NsApp/Behavior.h>
#include <NsGui/FrameworkElement.h>


namespace Noesis
{
struct DependencyPropertyChangedEventArgs;
template<class T> class ObservableCollection;
}

namespace NoesisApp
{

class FilterPredicate;
typedef Noesis::ObservableCollection<Noesis::BaseComponent> FilteredCollection;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251)
NS_CLANG_WARNING_DISABLE("-Wdocumentation")

////////////////////////////////////////////////////////////////////////////////////////////////////
/// A behavior that filters a collection depending on the supplied Predicate object.
///
/// .. code-block:: xml
///
///    <Grid
///      xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
///      xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
///      xmlns:i="http://schemas.microsoft.com/expression/2010/interactivity"
///      xmlns:noesis="clr-namespace:NoesisGUIExtensions;assembly=Noesis.GUI.Extensions">
///      <i:Interaction.Behaviors>
///        <noesis:CollectionFilterBehavior x:Name="FilterBehavior"
///          ItemsSource="{Binding Books}"
///          Predicate="{Binding TextPredicate}" />
///      </i:Interaction.Behaviors>
///      <TextBox Text="{Binding SearchText}"/>
///      <ListBox ItemsSource="{Binding FilteredItems, ElementName=FilterBehavior}" />
///    </Grid>
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_APP_INTERACTIVITY_API CollectionFilterBehavior:
    public BehaviorT<Noesis::FrameworkElement>
{
public:
    CollectionFilterBehavior();
    ~CollectionFilterBehavior();

    /// Gets or sets the predicate object used to filter the collection
    //@{
    FilterPredicate* GetPredicate() const;
    void SetPredicate(FilterPredicate* value);
    //@}

    /// Gets or sets the collection of items to be filtered
    //@{
    Noesis::BaseComponent* GetItemsSource() const;
    void SetItemsSource(Noesis::BaseComponent* value);
    //@}

    /// Gets the filtered collection of items
    //@{
    FilteredCollection* GetFilteredItems();
    //@}

public:
    static const Noesis::DependencyProperty* PredicateProperty;
    static const Noesis::DependencyProperty* ItemsSourceProperty;
    static const Noesis::DependencyProperty* FilteredItemsProperty;

protected:
    /// From Freezable
    //@{
    Noesis::Ptr<Noesis::Freezable> CreateInstanceCore() const override;
    //@}

private:
    void FilterItems();
    void OnFilterRequired();

    void RegisterPredicate(FilterPredicate* predicate);
    void UnregisterPredicate(FilterPredicate* predicate);

    static void OnItemsSourceChanged(Noesis::DependencyObject* d,
        const Noesis::DependencyPropertyChangedEventArgs& e);
    static void OnPredicateChanged(Noesis::DependencyObject* d,
        const Noesis::DependencyPropertyChangedEventArgs& e);

    NS_DECLARE_REFLECTION(CollectionFilterBehavior, BehaviorT<Noesis::FrameworkElement>)
};

NS_WARNING_POP

}


#endif
