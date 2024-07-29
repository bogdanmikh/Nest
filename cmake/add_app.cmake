include(cmake/target_platform_app.cmake)

function(add_app APP_NAME SOURCE INCLUDE RESOURCE_FOLDERS)

    set_source_files_properties(${ASSETS} PROPERTIES VS_TOOL_OVERRIDE "None")

    add_target_platform_app(${APP_NAME} "${SOURCE}" "${INCLUDE}" "${RESOURCE_FOLDERS}")

    set_source_files_properties(${ASSETS} PROPERTIES
            VS_DEPLOYMENT_LOCATION "."
            MACOSX_PACKAGE_LOCATION "Resources"
            )

    # source_group("headers" FILES ${INCLUDE})
    # source_group("src" FILES ${SOURCE})
    source_group("assets" FILES ${ASSETS})

    if (PLATFORM_WIN32 OR PLATFORM_LINUX)
        foreach (RESOURCE_FOLDER IN ITEMS ${RESOURCE_FOLDERS})
            # Copy assets to target folder
            file(COPY ${RESOURCE_FOLDER} DESTINATION "${BIN_DIRECTORY}/")
        endforeach ()
    elseif (PLATFORM_MACOS AND (NOT (CMAKE_GENERATOR STREQUAL Xcode)))
        foreach (RESOURCE_FOLDER IN ITEMS ${RESOURCE_FOLDERS})
            # Copy assets to target folder
            file(COPY ${RESOURCE_FOLDER} DESTINATION "${BIN_DIRECTORY}/${APP_NAME}.app/Contents/Resources")
        endforeach ()
    elseif (PLATFORM_IOS)
        foreach (RESOURCE_FOLDER IN ITEMS ${RESOURCE_FOLDERS})
            # Copy assets to target folder
            file(COPY ${RESOURCE_FOLDER} DESTINATION "${BIN_DIRECTORY}/${APP_NAME}.app")
        endforeach ()
    endif ()

endfunction()