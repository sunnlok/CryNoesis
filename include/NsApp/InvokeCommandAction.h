////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __APP_INVOKECOMMANDACTION_H__
#define __APP_INVOKECOMMANDACTION_H__


#include <NsCore/Noesis.h>
#include <NsApp/InteractivityApi.h>
#include <NsApp/TargetedTriggerAction.h>


namespace Noesis
{
NS_INTERFACE ICommand;
}

namespace NoesisApp
{

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251)
NS_CLANG_WARNING_DISABLE("-Wdocumentation")

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Executes the specified *ICommand* when invoked.
///
/// .. code-block:: xml
///
///    <Grid
///      xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
///      xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
///      xmlns:i="http://schemas.microsoft.com/expression/2010/interactivity">
///      <Button Content="Close">
///        <i:Interaction.Triggers>
///          <i:EventTrigger EventName="Click">
///            <i:InvokeCommandAction Command="{Binding CloseCommand}"/>
///          </i:EventTrigger>
///        </i:Interaction.Triggers>
///      </Button>
///    </Grid>
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_APP_INTERACTIVITY_API InvokeCommandAction:
    public TargetedTriggerActionT<Noesis::DependencyObject>
{
public:
    InvokeCommandAction();
    ~InvokeCommandAction();

    /// Gets or sets the name of the command this action should invoke
    //@{
    const char* GetCommandName() const;
    void SetCommandName(const char* name);
    //@}

    /// Gets or sets the command this action should invoke. Has more priority than *CommandName*
    //@{
    Noesis::ICommand* GetCommand() const;
    void SetCommand(Noesis::ICommand* command);
    //@}

    /// Gets or sets the command parameter
    //@{
    Noesis::BaseComponent* GetCommandParameter() const;
    void SetCommandParameter(Noesis::BaseComponent* parameter);
    //@}

public:
    static const Noesis::DependencyProperty* CommandProperty;
    static const Noesis::DependencyProperty* CommandParameterProperty;

protected:
    /// From Freezable
    //@{
    Noesis::Ptr<Noesis::Freezable> CreateInstanceCore() const override;
    //@}

    /// From TriggerAction
    //@{
    void Invoke(Noesis::BaseComponent* parameter) override;
    //}

private:
    Noesis::ICommand* ResolveCommand() const;

private:
    Noesis::String mCommandName;

    NS_DECLARE_REFLECTION(InvokeCommandAction, TargetedTriggerActionT<Noesis::DependencyObject>)
};

NS_WARNING_POP

}


#endif
