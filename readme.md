# Cry-Noesis


Implementation of Noesis GUI for cryengine.


### Prerequisites

1. [Requires the stage api in my cryengine for on the StageApiRework Branch.](https://github.com/sunnlok/CRYENGINE/tree/StageApiRework)

2. Add Shader folder into your Asset directory

3. [Noesis SDK](https://www.noesisengine.com/developers/downloads.php) - this is needed for Noesis.dll and SDK if you want to rebuild the CryNoesis Plugin

### Plugin installation

You two ways to use this plugin

1. you can just drop in the CryNoesis.dll into your Project's bin folder make sure to all add Noesis.dll into your 
custom Cryengine root bin folder also i.e `[Cryengine_Root]/bin/win_x64` afterwards make sure to add `StageApiRework` files into your Custom Cryengine, files will go into `[Cryengine_root]Code/CryEngine/CryCommon/CryRenderer/` and `[Cryengine_root]Code/CryEngine/RenderDLL/Common/` Respectfully.

Once done Rebuild your Custom Cryengine and then Run your game.

# Step two is if you want to Rebuild the plugin #

2. Just as step 1 with adding `StageApiRework` once finished you can `CryNoesis` folder itself into your `[Cryengine_Root]/CryPlugins` and then inside of Cmake Directory located in `[Cryengine_Root]/Tools/CMake` you'll see 
a file called `Build.cmake` Open that file in VS Code or Notepad++ and this look for `line 38` there you'll you add this 

```
# Only allow building legacy GameDLL's with the engine, assuming that we're not using the project system
if(OPTION_ENGINE AND NOT PROJECT_BUILD)
	# 2. Games
	add_subdirectories_glob("Code/Game*")
    # this is location of CryNoesis
	add_subdirectories_glob("Code/CryPlugins/CryNoesis*") 
endif()

```

Afterwards make sure to add the [Noesis SDK](https://www.noesisengine.com/developers/downloads.php) - download it
create a folder inside `[Cryengine_Root]/Code/SDKs` Called `Noesis` 

Once complete you can run `cry_cmake.exe` and build both the engine and CryNoesis plugin