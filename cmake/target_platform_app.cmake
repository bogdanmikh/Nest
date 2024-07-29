include(cmake/platform.cmake)

if(PLATFORM_WIN32)

    function(add_target_platform_app TARGET_NAME SOURCE INCLUDE ASSETS)
        add_executable(${TARGET_NAME} ${SOURCE} ${INCLUDE} ${ASSETS})
        set_source_files_properties(${ASSETS} PROPERTIES VS_DEPLOYMENT_CONTENT 1)
    endfunction()

elseif(PLATFORM_ANDROID)

    function(add_target_platform_app TARGET_NAME SOURCE INCLUDE ASSETS)
        add_library(${TARGET_NAME} SHARED ${SOURCE} ${INCLUDE} ${ASSETS})
    endfunction()

elseif(PLATFORM_LINUX)

    function(add_target_platform_app TARGET_NAME SOURCE INCLUDE ASSETS)
        add_executable(${TARGET_NAME} ${SOURCE} ${INCLUDE} ${ASSETS})
    endfunction()

elseif(PLATFORM_MACOS)

    function(add_target_platform_app TARGET_NAME SOURCE INCLUDE ASSETS)

        set(APPLE_RESOURCES
                ${PROJECT_SOURCE_DIR}/native/OSX/AssetsOSX.xcassets
                )

        if(CMAKE_GENERATOR STREQUAL Xcode)
            list (APPEND ASSETS "${APPLE_RESOURCES}")
        else()
            set (ASSETS ${APPLE_RESOURCES})
        endif()

        add_executable(${TARGET_NAME} MACOSX_BUNDLE ${SOURCE} ${INCLUDE} ${ASSETS})

        set_target_properties(${TARGET_NAME} PROPERTIES
                XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "com.vivo.${TARGET_NAME}"
                MACOSX_BUNDLE_INFO_PLIST "${PROJECT_SOURCE_DIR}/native/OSX/Info.plist"
                # XCODE_ATTRIBUTE_CODE_SIGN_ENTITLEMENTS "${PROJECT_SOURCE_DIR}/native/OSX/VivoVideoEditor.entitlements"
                XCODE_ATTRIBUTE_ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon"
                RESOURCE "${ASSETS}"
                )
        source_group("Resources" FILES "${ASSETS}")

        target_compile_definitions(${TARGET_NAME} PUBLIC GL_SILENCE_DEPRECATION)

    endfunction()

elseif(PLATFORM_IOS)

    function(add_target_platform_app TARGET_NAME SOURCE INCLUDE ASSETS)

        set(APPLE_RESOURCES
                ${PROJECT_SOURCE_DIR}/native/IOS/LaunchScreen.storyboard
                ${PROJECT_SOURCE_DIR}/native/IOS/Assets.xcassets
                )

        if(CMAKE_GENERATOR STREQUAL Xcode)
            list (APPEND ASSETS "${APPLE_RESOURCES}")
        else()
            set (ASSETS ${APPLE_RESOURCES})
        endif()

        add_executable(${TARGET_NAME} MACOSX_BUNDLE ${SOURCE} ${INCLUDE} ${ASSETS})

        set(CMAKE_OSX_DEPLOYMENT_TARGET "14.0" CACHE STRING "Minimum iOS deployment version")

        set_target_properties(${TARGET_NAME} PROPERTIES
                XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "com.md.${TARGET_NAME}"
                MACOSX_BUNDLE_INFO_PLIST "${PROJECT_SOURCE_DIR}/native/IOS/Info.plist"
                XCODE_ATTRIBUTE_ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon"
                RESOURCE "${ASSETS}"
                XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer"
                BUILD_RPATH "@executable_path"
                # XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "Dev Team"
                )
        source_group("Resources" FILES "${ASSETS}")

        target_compile_definitions(${TARGET_NAME} PUBLIC GL_SILENCE_DEPRECATION)
        target_compile_definitions(${TARGET_NAME} PUBLIC GLES_SILENCE_DEPRECATION)

    endfunction()

else()
    message(FATAL_ERROR "Unknown platform")
endif()