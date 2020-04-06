////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsApp/InvokeCommandAction.h>
#include <NsGui/ICommand.h>
#include <NsGui/DependencyData.h>
#include <NsCore/ReflectionImplement.h>
#include <NsCore/ReflectionHelper.h>


using namespace NoesisApp;


////////////////////////////////////////////////////////////////////////////////////////////////////
InvokeCommandAction::InvokeCommandAction()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
InvokeCommandAction::~InvokeCommandAction()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
const char* InvokeCommandAction::GetCommandName() const
{
    return mCommandName.Str();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void InvokeCommandAction::SetCommandName(const char* name)
{
    mCommandName = name;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Noesis::ICommand* InvokeCommandAction::GetCommand() const
{
    return Noesis::DynamicCast<Noesis::ICommand*>(GetValue<Noesis::Ptr<BaseComponent>>(CommandProperty).GetPtr());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void InvokeCommandAction::SetCommand(Noesis::ICommand* command)
{
    SetValue<Noesis::Ptr<BaseComponent>>(CommandProperty, (BaseComponent*)command->GetBaseObject());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Noesis::BaseComponent* InvokeCommandAction::GetCommandParameter() const
{
    return GetValue<Noesis::Ptr<BaseComponent>>(CommandParameterProperty);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void InvokeCommandAction::SetCommandParameter(BaseComponent* parameter)
{
    SetValue<Noesis::Ptr<BaseComponent>>(CommandProperty, parameter);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Noesis::Ptr<Noesis::Freezable> InvokeCommandAction::CreateInstanceCore() const
{
    return *new InvokeCommandAction();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void InvokeCommandAction::Invoke(BaseComponent*)
{
    if (GetAssociatedObject() != 0)
    {
        Noesis::ICommand* command = ResolveCommand();
        BaseComponent* param = GetCommandParameter();
        if (command != 0 && command->CanExecute(param))
        {
            command->Execute(param);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Noesis::ICommand* InvokeCommandAction::ResolveCommand() const
{
    Noesis::ICommand* command = GetCommand();
    if (command != 0)
    {
        return command;
    }

    if (!mCommandName.Empty())
    {
        Noesis::Symbol propName(mCommandName.Str());

        DependencyObject* associatedObject = GetAssociatedObject();
        NS_ASSERT(associatedObject != 0);

        const Noesis::TypeClass* type = associatedObject->GetClassType();
        Noesis::TypeClassProperty p = Noesis::FindProperty(type, propName);
        if (p.property != 0)
        {
            return Noesis::DynamicCast<Noesis::ICommand*>(p.property->GetComponent(associatedObject).GetPtr());
        }
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
NS_BEGIN_COLD_REGION

NS_IMPLEMENT_REFLECTION(InvokeCommandAction, "NoesisApp.InvokeCommandAction")
{
    Noesis::DependencyData* data = NsMeta<Noesis::DependencyData>(Noesis::TypeOf<SelfClass>());
    data->RegisterProperty<Noesis::Ptr<BaseComponent>>(CommandProperty, "Command",
        Noesis::PropertyMetadata::Create(Noesis::Ptr<BaseComponent>()));
    data->RegisterProperty<Noesis::Ptr<BaseComponent>>(CommandParameterProperty, "CommandParameter",
        Noesis::PropertyMetadata::Create(Noesis::Ptr<BaseComponent>()));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
const Noesis::DependencyProperty* InvokeCommandAction::CommandProperty;
const Noesis::DependencyProperty* InvokeCommandAction::CommandParameterProperty;
