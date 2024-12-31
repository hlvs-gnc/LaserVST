cmake_minimum_required(VERSION 3.14.0)

option(SMTG_ENABLE_VST3_PLUGIN_EXAMPLES "Enable VST 3 Plug-in Examples" OFF)
option(SMTG_ENABLE_VST3_HOSTING_EXAMPLES "Enable VST 3 Hosting Examples" OFF)

set(CMAKE_OSX_DEPLOYMENT_TARGET 10.13 CACHE STRING "")

set(vst3sdk_SOURCE_DIR "C:/Users/Utilizador/Tools/VST_SDK/vst3sdk")
if(NOT vst3sdk_SOURCE_DIR)
    message(FATAL_ERROR "Path to VST3 SDK is empty!")
endif()

project(Laser
    # This is your plug-in version number. Change it here only.
    # Version number symbols usable in C++ can be found in
    # source/version.h and ${PROJECT_BINARY_DIR}/projectversion.h.
    VERSION 1.0.0.0 
    DESCRIPTION "Laser VST 3 Plug-in"
)

set(SMTG_VSTGUI_ROOT "${vst3sdk_SOURCE_DIR}")

add_subdirectory(${vst3sdk_SOURCE_DIR} ${PROJECT_BINARY_DIR}/vst3sdk)
smtg_enable_vst3_sdk()

smtg_add_vst3plugin(Laser
    source/version.h
    source/laser_cids.h
    source/laser_processor.h
    source/laser_processor.cpp
    source/laser_controller.h
    source/laser_controller.cpp
    source/laser_entry.cpp
)

#- VSTGUI Wanted ----
if(SMTG_ENABLE_VSTGUI_SUPPORT)
    target_sources(Laser
        PRIVATE
            resource/laser_editor.uidesc
    )
    target_link_libraries(Laser
        PRIVATE
            vstgui_support
    )
    smtg_target_add_plugin_resources(Laser
        RESOURCES
            "resource/laser_editor.uidesc"
    )
endif(SMTG_ENABLE_VSTGUI_SUPPORT)
# -------------------

smtg_target_add_plugin_snapshots (Laser
    RESOURCES
        resource/2527AAF6AD4053EEA92E6064887869B6_snapshot.png
        resource/2527AAF6AD4053EEA92E6064887869B6_snapshot_2.0x.png
)

target_link_libraries(Laser
    PRIVATE
        sdk
)

smtg_target_configure_version_file(Laser)

if(SMTG_MAC)
    smtg_target_set_bundle(Laser
        BUNDLE_IDENTIFIER com.radar.laser
        COMPANY_NAME "Radar"
    )
    smtg_target_set_debug_executable(Laser
        "/Applications/VST3PluginTestHost.app"
        "--pluginfolder;$(BUILT_PRODUCTS_DIR)"
    )
elseif(SMTG_WIN)
    target_sources(Laser PRIVATE 
        resource/win32resource.rc
    )
    if(MSVC)
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Laser)

        smtg_target_set_debug_executable(Laser
            "$(ProgramW6432)/Steinberg/VST3PluginTestHost/VST3PluginTestHost.exe"
            "--pluginfolder \"$(OutDir)/\""
        )
    endif()
endif(SMTG_MAC)
