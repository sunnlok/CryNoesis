////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __APP_CONDITIONBEHAVIOR_H__
#define __APP_CONDITIONBEHAVIOR_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsApp/InteractivityApi.h>
#include <NsApp/Behavior.h>
#include <NsApp/TriggerBase.h>


namespace NoesisApp
{

class ConditionalExpression;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251)
NS_CLANG_WARNING_DISABLE("-Wdocumentation")

////////////////////////////////////////////////////////////////////////////////////////////////////
/// A behavior that attaches to a trigger and controls the conditions to fire the actions.
///
/// .. code-block:: xml
///
///    <Grid
///      xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
///      xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
///      xmlns:i="http://schemas.microsoft.com/expression/2010/interactivity"
///      xmlns:ei="http://schemas.microsoft.com/expression/2010/interactions">
///      <TextBlock Text="{Binding Count}" Background="{StaticResource ValidBackground}">
///        <i:Interaction.Triggers>
///          <ei:PropertyChangedTrigger Binding="{Binding Path=Count}">
///            <i:Interaction.Behaviors>
///              <ei:ConditionBehavior>
///                <ei:ConditionalExpression ForwardChaining="Or">
///                  <ei:ComparisonCondition LeftOperand="{Binding Count}" Operator="LessThan" RightOperand="0" />
///                  <ei:ComparisonCondition LeftOperand="{Binding Count}" Operator="GreaterThan" RightOperand="100" />
///                </ei:ConditionalExpression>
///              </ei:ConditionBehavior>
///            </i:Interaction.Behaviors>
///            <ei:ChangePropertyAction PropertyName="Background" Value="{StaticResource InvalidBackground}"/>
///          </ei:PropertyChangedTrigger>
///        </i:Interaction.Triggers>
///      </TextBlock>
///    </Grid>
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_APP_INTERACTIVITY_API ConditionBehavior: public BehaviorT<TriggerBase>
{
public:
    ConditionBehavior();
    ~ConditionBehavior();

    /// Gets or sets the Conditon object on the behavior
    //@{
    ConditionalExpression* GetCondition() const;
    void SetCondition(ConditionalExpression* value);
    //@}

public:
    static const Noesis::DependencyProperty* ConditionProperty;

protected:
    /// From Freezable
    //@{
    Noesis::Ptr<Noesis::Freezable> CreateInstanceCore() const override;
    //@}

    /// From Behavior
    //@{
    void OnAttached() override;
    void OnDetaching() override;
    //@}

private:
    void OnPreviewInvoke(Noesis::BaseComponent* sender, const PreviewInvokeEventArgs& e);

    NS_DECLARE_REFLECTION(ConditionBehavior, BehaviorT<TriggerBase>)
};

NS_WARNING_POP

}


#endif
