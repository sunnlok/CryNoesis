if(DEDICATED_SERVER)
	return()
endif()

option(PLUGIN_Noesis "Enable the noesis plugin" ON)
if (NOT PLUGIN_Noesis)
	return()
endif()


set(NS_SDK "${SDK_DIR}/Noesis" CACHE PATH "Noesis sdk path")


set(NS_LICENSE_NAME "" CACHE STRING "The noesis Licsense Name")
set(NS_LICENSE_KEY "" CACHE STRING "The noesis Licsense Key")

if(NOT EXISTS ${NS_SDK})
 message("Noesis sdk path not found at: ${NS_SDK}")
 return()
endif()

start_sources()
sources_platform(ALL)
add_sources("CryNoesis.cpp"
	SOURCE_GROUP "Root"
		"Plugin.cpp"
        "Plugin.h"
        "StdAfx.h"
)

add_sources("CryNoesis_Interface.cpp"
	SOURCE_GROUP "Interface"
	"../Interface/INoesis.h"
	"../Interface/INoesisPlugin.h"
	"../Interface/IViewManager.h"
)

add_sources("CryNoesis_Core.cpp"
	SOURCE_GROUP "Core"
        "Core/Implementation.cpp"
		"Core/Handlers.h"
        "Core/Implementation.h"
		"Core/InputHandler.cpp"
		"Core/InputHandler.h"
		"Core/ComponentRegistration.h"
		"Core/ViewManager.cpp"
		"Core/ViewManager.h"
)

add_sources("CryNoesis_Providers.cpp"
	SOURCE_GROUP "Providers"
        "Providers/FileProvider.cpp"
		"Providers/FileProvider.h"
		"Providers/FontProvider.cpp"
        "Providers/FontProvider.h"
        "Providers/XamlProvider.cpp"
        "Providers/XamlProvider.h"
		"Providers/TextureProvider.cpp"
		"Providers/TextureProvider.h"	
)

add_sources("CryNoesis_Rendering.cpp"
	SOURCE_GROUP "Renderer"
		"Renderer/LayoutDefinitions.h"
		"Renderer/StateDefinitions.h"
		"Renderer/Renderer.cpp"
		"Renderer/Renderer.h"
		"Renderer/Texture.cpp"
		"Renderer/Texture.h"
		"Renderer/ViewRenderData.h"
)

add_sources("CryNoesis_Components.cpp"
	SOURCE_GROUP "Components"
		"Controls/ViewContainer.h"
		"Controls/ViewContainer.cpp"
)

add_sources("CryNoesis_NsApp.cpp"
	SOURCE_GROUP "Components\\\\NsApp\\\\src"
		"Components/NsApp/Interactivity/App.Interactivity.cpp"
		"Components/NsApp/Interactivity/AttachableObject.cpp"
		"Components/NsApp/Interactivity/Behavior.cpp"
		"Components/NsApp/Interactivity/BehaviorCollection.cpp"
		"Components/NsApp/Interactivity/ChangePropertyAction.cpp"
		"Components/NsApp/Interactivity/CollectionFilterBehavior.cpp"
		"Components/NsApp/Interactivity/CollectionSortBehavior.cpp"
		"Components/NsApp/Interactivity/ComparisonCondition.cpp"
		"Components/NsApp/Interactivity/ComparisonLogic.cpp"
		"Components/NsApp/Interactivity/ComparisonLogic.h"
		"Components/NsApp/Interactivity/ConditionalExpression.cpp"
		"Components/NsApp/Interactivity/ConditionBehavior.cpp"
		"Components/NsApp/Interactivity/ControlStoryboardAction.cpp"
		"Components/NsApp/Interactivity/DataBindingHelper.cpp"
		"Components/NsApp/Interactivity/DataBindingHelper.h"
		"Components/NsApp/Interactivity/DataTrigger.Interactivity.cpp"
		"Components/NsApp/Interactivity/EventTrigger.Interactivity.cpp"
		"Components/NsApp/Interactivity/EventTriggerBase.cpp"
		"Components/NsApp/Interactivity/FilterPredicate.cpp"
		"Components/NsApp/Interactivity/GamepadTrigger.cpp"
		"Components/NsApp/Interactivity/GoToStateAction.cpp"
		"Components/NsApp/Interactivity/Interaction.cpp"
		"Components/NsApp/Interactivity/InvokeCommandAction.cpp"
		"Components/NsApp/Interactivity/KeyTrigger.cpp"
		"Components/NsApp/Interactivity/LaunchUriOrFileAction.cpp"
		"Components/NsApp/Interactivity/MouseDragElementBehavior.cpp"
		"Components/NsApp/Interactivity/PlaySoundAction.cpp"
		"Components/NsApp/Interactivity/PropertyChangedTrigger.cpp"
		"Components/NsApp/Interactivity/RemoveElementAction.cpp"
		"Components/NsApp/Interactivity/SelectAction.cpp"
		"Components/NsApp/Interactivity/SelectAllAction.cpp"
		"Components/NsApp/Interactivity/SetFocusAction.cpp"
		"Components/NsApp/Interactivity/SortComparer.cpp"
		"Components/NsApp/Interactivity/StoryboardAction.cpp"
		"Components/NsApp/Interactivity/StoryboardCompletedTrigger.cpp"
		"Components/NsApp/Interactivity/StoryboardTrigger.cpp"
		"Components/NsApp/Interactivity/StringFilterPredicate.cpp"
		"Components/NsApp/Interactivity/StringSortComparer.cpp"
		"Components/NsApp/Interactivity/StyleInteraction.cpp"
		"Components/NsApp/Interactivity/TargetedTriggerAction.cpp"
		"Components/NsApp/Interactivity/TimerTrigger.cpp"
		"Components/NsApp/Interactivity/TranslateZoomRotateBehavior.cpp"
		"Components/NsApp/Interactivity/TriggerAction.Interactivity.cpp"
		"Components/NsApp/Interactivity/TriggerActionCollection.cpp"
		"Components/NsApp/Interactivity/TriggerBase.Interactivity.cpp"
		"Components/NsApp/Interactivity/TriggerCollection.cpp"
	SOURCE_GROUP "Components\\\\NsApp\\\\include"
		"../include/NsApp/AttachableCollection.h"
	    "../include/NsApp/AttachableCollection.inl"
	    "../include/NsApp/AttachableObject.h"
	    "../include/NsApp/Behavior.h"
	    "../include/NsApp/BehaviorCollection.h"
		"../include/NsApp/ChangePropertyAction.h"
		"../include/NsApp/CollectionFilterBehavior.h"
		"../include/NsApp/CollectionSortBehavior.h"
		"../include/NsApp/ComparisonCondition.h"
		"../include/NsApp/ConditionalExpression.h"
		"../include/NsApp/ConditionBehavior.h"
		"../include/NsApp/ControlStoryboardAction.h"
		"../include/NsApp/DataTrigger.h"
		"../include/NsApp/EventTrigger.h"
		"../include/NsApp/EventTriggerBase.h"
		"../include/NsApp/FilterPredicate.h"
		"../include/NsApp/GamepadTrigger.h"
		"../include/NsApp/GoToStateAction.h"
		"../include/NsApp/IAttachedObject.h"
		"../include/NsApp/Interaction.h"
		"../include/NsApp/InteractivityApi.h"
		"../include/NsApp/InvokeCommandAction.h"
		"../include/NsApp/KeyTrigger.h"
		"../include/NsApp/LaunchUriOrFileAction.h"
		"../include/NsApp/MouseDragElementBehavior.h"
		"../include/NsApp/PlaySoundAction.h"
		"../include/NsApp/PropertyChangedTrigger.h"
		"../include/NsApp/RemoveElementAction.h"
		"../include/NsApp/SelectAction.h"
		"../include/NsApp/SelectAllAction.h"
		"../include/NsApp/SetFocusAction.h"
		"../include/NsApp/SortComparer.h"
		"../include/NsApp/StoryboardAction.h"
		"../include/NsApp/StoryboardCompletedTrigger.h"
		"../include/NsApp/StoryboardTrigger.h"
		"../include/NsApp/StringFilterPredicate.h"
		"../include/NsApp/StringSortComparer.h"
		"../include/NsApp/StyleInteraction.h"
		"../include/NsApp/TargetedTriggerAction.h"
		"../include/NsApp/TimerTrigger.h"
		"../include/NsApp/TranslateZoomRotateBehavior.h"
		"../include/NsApp/TriggerAction.h"
		"../include/NsApp/TriggerActionCollection.h"
		"../include/NsApp/TriggerBase.h"
		"../include/NsApp/TriggerCollection.h"
)


add_sources("NoUberFile"
	SOURCE_GROUP "Root"
		"StdAfx.cpp"
		"resource.h"
)

end_sources()

CryEngineModule(CryNoesis PCH "StdAfx.cpp" SOLUTION_FOLDER "CryPlugins")

target_include_directories(${THIS_PROJECT}
 PRIVATE
  "${CRYENGINE_DIR}/Code/CryEngine/CryCommon"
  "${NS_SDK}/Include"
  "../Include"
 PUBLIC
  ../Interface
)


unset(${NoesisLib} CACHE)
find_library(NoesisLib NAMES Noesis PATHS "${NS_SDK}/Lib/windows_x86_64")
target_link_libraries(${THIS_PROJECT} PRIVATE "${NoesisLib}")

#Enable cpp 17 support
set_target_properties(${THIS_PROJECT} PROPERTIES CXX_STANDARD 17)
#Enable edit and continue support
target_compile_options( ${THIS_PROJECT} PRIVATE /ZI )

target_compile_definitions(${THIS_PROJECT} PRIVATE NS_LICENSE_KEY=${NS_LICENSE_KEY} NS_LICENSE_NAME=${NS_LICENSE_NAME}) 

deploy_runtime_files(${NS_SDK}/Bin/windows_x86_64/Noesis.dll)
#deploy_runtime_files(${NS_SDK}/Bin/windows_x86_64/NoesisApp.dll)