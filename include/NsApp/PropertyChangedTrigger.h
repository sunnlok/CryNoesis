////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __APP_PROPERTYCHANGEDTRIGGER_H__
#define __APP_PROPERTYCHANGEDTRIGGER_H__


#include <NsCore/Noesis.h>
#include <NsApp/TriggerBase.h>


namespace NoesisApp
{

NS_WARNING_PUSH
NS_CLANG_WARNING_DISABLE("-Wdocumentation")

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a trigger that performs actions when the bound data changes.
///
/// .. code-block:: xml
///
///    <Grid
///      xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
///      xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
///      xmlns:i="http://schemas.microsoft.com/expression/2010/interactivity"
///      xmlns:ei="http://schemas.microsoft.com/expression/2010/interactions">
///      <Slider x:Name="Percentage" Minimum="0" Maximum="100">
///        <i:Interaction.Triggers>
///          <ei:PropertyChangedTrigger Binding="{Binding Value, ElementName=Percentage}">
///            <ei:PlaySoundAction Source="Sounds/SliderDrag.wav"/>
///          </ei:PropertyChangedTrigger>
///        </i:Interaction.Triggers>
///      </Slider>
///    </Grid>
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_APP_INTERACTIVITY_API PropertyChangedTrigger: public TriggerBaseT<Noesis::DependencyObject>
{
public:
    PropertyChangedTrigger();
    virtual ~PropertyChangedTrigger();

    /// Gets or sets the binding object that the trigger listens to, and causes to fire actions
    //@{
    Noesis::BaseComponent* GetBinding() const;
    void SetBinding(Noesis::BaseComponent* value);
    //@}

    // Hides Freezable methods for convenience
    //@{
    Noesis::Ptr<PropertyChangedTrigger> Clone() const;
    Noesis::Ptr<PropertyChangedTrigger> CloneCurrentValue() const;
    //@}

public:
    static const Noesis::DependencyProperty* BindingProperty;

protected:
    void EvaluateTriggerChange();

    // Called when the binding property has changed
    virtual void EvaluateBindingChange();

    // From Freezable
    //@{
    Noesis::Ptr<Noesis::Freezable> CreateInstanceCore() const override;
    //@}

    // From DependencyObject
    //@{
    void OnPostInit() override;
    //@}

private:
    static void OnBindingChanged(Noesis::DependencyObject* d,
        const Noesis::DependencyPropertyChangedEventArgs& e);

private:
    union
    {
        struct
        {
            bool initCompleted : 1;
            bool evaluateRequired : 1;
        } mFlags;

        uint8_t mAllFlags;
    };

    NS_DECLARE_REFLECTION(PropertyChangedTrigger, TriggerBase)
};

NS_WARNING_POP

}


#endif
