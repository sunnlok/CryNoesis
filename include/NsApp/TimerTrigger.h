////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __APP_TIMERTRIGGER_INTERACTIVITY_H__
#define __APP_TIMERTRIGGER_INTERACTIVITY_H__


#include <NsCore/Noesis.h>
#include <NsApp/EventTrigger.h>


namespace NoesisApp
{

NS_WARNING_PUSH
NS_CLANG_WARNING_DISABLE("-Wdocumentation")

////////////////////////////////////////////////////////////////////////////////////////////////////
/// A trigger that is triggered by a specified event and fires after a delay.
///
/// .. code-block:: xml
///
///    <Grid
///      xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
///      xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
///      xmlns:i="http://schemas.microsoft.com/expression/2010/interactivity"
///      xmlns:ei="http://schemas.microsoft.com/expression/2010/interactions">
///      <Button Content="Close">
///        <i:Interaction.Triggers>
///          <i:TimerTrigger EventName="Click" MillisecondsPerTick="500" TotalTicks="1">
///            <i:InvokeCommandAction Command="{Binding CloseCommand}"/>
///          </i:TimerTrigger>
///        </i:Interaction.Triggers>
///      </Button>
///    </Grid>
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_APP_INTERACTIVITY_API TimerTrigger: public EventTrigger
{
public:
    TimerTrigger();
    ~TimerTrigger();

    /// Gets or sets the number of milliseconds to wait between ticks
    //@{
    int GetMillisecondsPerTick() const;
    void SetMillisecondsPerTick(int ms);
    //@}

    /// Gets or sets the total number of ticks to be fired before the trigger is finished
    //@{
    int GetTotalTicks() const;
    void SetTotalTicks(int ticks);
    //@}

    // Hides Freezable methods for convenience
    //@{
    Noesis::Ptr<TimerTrigger> Clone() const;
    Noesis::Ptr<TimerTrigger> CloneCurrentValue() const;
    //@}

public:
    static const Noesis::DependencyProperty* MillisecondsPerTickProperty;
    static const Noesis::DependencyProperty* TotalTicksProperty;

protected:
    /// From Freezable
    //@{
    Noesis::Ptr<Noesis::Freezable> CreateInstanceCore() const override;
    //@}

    /// From AttachableObject
    //@{
    void OnDetaching() override;
    //@}

    /// From EventTriggerBase
    //@{
    void OnEvent() override;
    //@}

private:
    void StartTimer();
    void StopTimer();
    uint32_t OnTimerTick();

private:
    int mTimer;
    int mTickCount;

    NS_DECLARE_REFLECTION(TimerTrigger, EventTrigger)
};

NS_WARNING_POP

}


#endif
