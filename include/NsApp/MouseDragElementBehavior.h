////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __APP_MOUSEDRAGELEMENTBEHAVIOR_H__
#define __APP_MOUSEDRAGELEMENTBEHAVIOR_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsApp/InteractivityApi.h>
#include <NsApp/Behavior.h>
#include <NsGui/FrameworkElement.h>
#include <NsDrawing/Point.h>


namespace Noesis
{
class TranslateTransform;
typedef Delegate<void(BaseComponent*, const MouseEventArgs&)> MouseEventHandler;
}

namespace NoesisApp
{

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251)
NS_CLANG_WARNING_DISABLE("-Wdocumentation")

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Moves the attached element in response to mouse drag gestures on the element.
///
/// .. code-block:: xml
///
///    <Grid
///      xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
///      xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
///      xmlns:i="http://schemas.microsoft.com/expression/2010/interactivity"
///      xmlns:ei="http://schemas.microsoft.com/expression/2010/interactions">
///      <Border Background="Silver" Margin="100">
///        <Rectangle Width="100" Height="50" Fill="Red">
///          <i:Interaction.Behaviors>
///            <ei:MouseDragElementBehavior ConstraintToParentBounds="True" />
///          </i:Interaction.Behaviors>
///        </Rectangle>
///      </Border>
///    </Grid>
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_APP_INTERACTIVITY_API MouseDragElementBehavior:
    public BehaviorT<Noesis::FrameworkElement>
{
public:
    MouseDragElementBehavior();
    ~MouseDragElementBehavior();

    /// Gets or sets the X position of the dragged element, relative to the left of the root element
    //@{
    float GetX() const;
    void SetX(float x);
    //@}

    /// Gets or sets the Y position of the dragged element, relative to the top of the root element
    //@{
    float GetY() const;
    void SetY(float y);
    //@}

    /// Gets or sets a value indicating whether the dragged element is constrained to stay within
    /// the bounds of its Parent container
    //@{
    bool GetConstrainToParentBounds() const;
    void SetConstrainToParentBounds(bool value);
    //@}

    /// Occurs when a drag gesture is initiated
    Noesis::DelegateEvent_<Noesis::MouseEventHandler> DragBegun();

    /// Occurs when a drag gesture update is processed
    Noesis::DelegateEvent_<Noesis::MouseEventHandler> Dragging();

    /// Occurs when a drag gesture is finished
    Noesis::DelegateEvent_<Noesis::MouseEventHandler> DragFinished();

public:
    static const Noesis::DependencyProperty* XProperty;
    static const Noesis::DependencyProperty* YProperty;
    static const Noesis::DependencyProperty* ConstrainToParentBoundsProperty;

protected:
    // From Freezable
    //@{
    Noesis::Ptr<Noesis::Freezable> CreateInstanceCore() const override;
    //@}

    // From Behavior
    //@{
    void OnAttached() override;
    void OnDetaching() override;
    //@}

private:
    void OnMouseLeftButtonDown(Noesis::BaseComponent* sender,
        const Noesis::MouseButtonEventArgs& e);
    void OnMouseLeftButtonUp(Noesis::BaseComponent* sender, const Noesis::MouseButtonEventArgs& e);
    void OnMouseMove(Noesis::BaseComponent* sender, const Noesis::MouseEventArgs& e);
    void OnLostMouseCapture(Noesis::BaseComponent* sender, const Noesis::MouseEventArgs& e);

    void StartDrag(Noesis::Point relativePosition);
    void Drag(Noesis::Point relativePosition);
    void EndDrag();

    void UpdateTransform(float x, float y);
    void UpdatePosition(float x, float y);
    void UpdatePosition();

    static void OnXChanged(Noesis::DependencyObject* d,
        const Noesis::DependencyPropertyChangedEventArgs& e);
    static void OnYChanged(Noesis::DependencyObject* d,
        const Noesis::DependencyPropertyChangedEventArgs& e);
    static void OnConstrainToParentBoundsChanged(Noesis::DependencyObject* d,
        const Noesis::DependencyPropertyChangedEventArgs& e);

private:
    bool mSettingPosition;
    Noesis::Point mRelativePosition;
    Noesis::Ptr<Noesis::TranslateTransform> mTransform;

    Noesis::MouseEventHandler mDragBegun;
    Noesis::MouseEventHandler mDragging;
    Noesis::MouseEventHandler mDragFinished;

    NS_DECLARE_REFLECTION(MouseDragElementBehavior, BehaviorT<Noesis::FrameworkElement>)
};

NS_WARNING_POP

}


#endif
