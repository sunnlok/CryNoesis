////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsApp/AttachableObject.h>
#include <NsCore/ReflectionImplement.h>
#include <NsGui/Visual.h>
#include <NsGui/IUITreeNode.h>


using namespace NoesisApp;


////////////////////////////////////////////////////////////////////////////////////////////////////
AttachableObject::AttachableObject(const Noesis::TypeClass* associatedType):
    mAssociatedType(associatedType), mAssociatedObject(0), mView(0)
{
    NS_ASSERT(mAssociatedType != 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
AttachableObject::~AttachableObject()
{
    if (mAssociatedObject != 0)
    {
        mAssociatedObject->Destroyed() -= MakeDelegate(this,
            &AttachableObject::OnAttachedDestroyed);
        mAssociatedObject = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Noesis::DependencyObject* AttachableObject::GetAssociatedObject() const
{
    return mAssociatedObject;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void AttachableObject::Attach(DependencyObject* associatedObject)
{
    if (mAssociatedObject != associatedObject)
    {
        if (mAssociatedObject != 0)
        {
            NS_ERROR("'%s' already attached to another object '%s'",
                GetClassType()->GetName(), mAssociatedObject->GetClassType()->GetName());
            return;
        }

        NS_ASSERT(associatedObject != 0);
        if (!mAssociatedType->IsAssignableFrom(associatedObject->GetClassType()))
        {
            NS_ERROR("Invalid associated element type '%s' for '%s'",
                associatedObject->GetClassType()->GetName(), GetClassType()->GetName());
            return;
        }

        mAssociatedObject = associatedObject;
        mAssociatedObject->Destroyed() += MakeDelegate(this,
            &AttachableObject::OnAttachedDestroyed);

        mView = FindView();

        OnAttached();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void AttachableObject::Detach()
{
    if (mAssociatedObject != 0)
    {
        OnDetaching();

        mAssociatedObject->Destroyed() -= MakeDelegate(this,
            &AttachableObject::OnAttachedDestroyed);
        mAssociatedObject = 0;
        mView = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Noesis::IView* AttachableObject::GetView() const
{
    return mView;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void AttachableObject::OnAttached()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void AttachableObject::OnDetaching()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void AttachableObject::OnAttachedDestroyed(DependencyObject* d)
{
    NS_ASSERT(mAssociatedObject == d);
    Detach();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
Noesis::IView* AttachableObject::FindView() const
{
    NS_ASSERT(mAssociatedObject != 0);

    Noesis::IView* view = 0;
    Noesis::IUITreeNode* node = Noesis::DynamicCast<Noesis::IUITreeNode*>(mAssociatedObject);

    while (view == 0 && node != 0)
    {
        Noesis::Visual* visual = Noesis::DynamicCast<Noesis::Visual*>(node);
        if (visual != 0)
        {
            view = visual->GetView();
        }

        node = node->GetNodeParent();
    }

    return view;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
NS_BEGIN_COLD_REGION

NS_IMPLEMENT_REFLECTION(AttachableObject)
{
    NsImpl<IAttachedObject>();
}
