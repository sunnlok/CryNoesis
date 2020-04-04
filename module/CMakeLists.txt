if(DEDICATED_SERVER)
	return()
endif()

option(PLUGIN_Noesis "Enable the noesis plugin" ON)
if (NOT PLUGIN_Noesis)
	return()
endif()

set(NS_SDK "${SDK_DIR}/Noesis" CACHE PATH "Noesis sdk path")

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

add_sources("CryNoesis_Core.cpp"
	SOURCE_GROUP "Core"
        "Core/Implementation.cpp"
		"Core/Handlers.h"
        "Core/Implementation.h"
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
 PUBLIC
  ../Interface
)

find_library(Noesis Noesis PATHS "${NS_SDK}/Lib/windows_x86_64")
target_link_libraries(${THIS_PROJECT} PRIVATE "${Noesis}")

#Enable cpp 17 support
set_target_properties(${THIS_PROJECT} PROPERTIES CXX_STANDARD 17)
#Enable edit and continue support
target_compile_options( ${THIS_PROJECT} PRIVATE /ZI )

deploy_runtime_files(${NS_SDK}/Bin/windows_x86_64/Noesis.dll)
#deploy_runtime_files(${NS_SDK}/Bin/windows_x86_64/NoesisApp.dll)