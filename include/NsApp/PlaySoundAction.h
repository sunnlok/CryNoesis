////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __APP_PLAYSOUNDACTION_H__
#define __APP_PLAYSOUNDACTION_H__


#include <NsCore/Noesis.h>
#include <NsApp/InteractivityApi.h>
#include <NsApp/TriggerAction.h>
#include <NsGui/Uri.h>


namespace NoesisApp
{

NS_WARNING_PUSH
NS_CLANG_WARNING_DISABLE("-Wdocumentation")

////////////////////////////////////////////////////////////////////////////////////////////////////
/// An action that will play a sound to completion.
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
///          <i:EventTrigger EventName="Click">
///            <ei:PlaySoundAction Source="Sounds/ExitButton.wav"/>
///          </i:EventTrigger>
///        </i:Interaction.Triggers>
///      </Button>
///    </Grid>
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_APP_INTERACTIVITY_API PlaySoundAction final:
    public TriggerActionT<Noesis::DependencyObject>
{
public:
    PlaySoundAction();
    ~PlaySoundAction();

    /// Gets or sets the URI that defines the location of the sound file
    //@{
    const Noesis::Uri& GetSource() const;
    void SetSource(const Noesis::Uri& source);
    //@}

    /// Controls the volume of the sound
    //@{
    float GetVolume() const;
    void SetVolume(float volume);
    //@}

public:
    static const Noesis::DependencyProperty* SourceProperty;
    static const Noesis::DependencyProperty* VolumeProperty;

protected:
    /// From Freezable
    //@{
    Noesis::Ptr<Noesis::Freezable> CreateInstanceCore() const override;
    //@}

    /// From TriggerAction
    //@{
    void Invoke(Noesis::BaseComponent* parameter) override;
    //}

    NS_DECLARE_REFLECTION(PlaySoundAction, TriggerActionT<Noesis::DependencyObject>)
};

NS_WARNING_POP

}


#endif
