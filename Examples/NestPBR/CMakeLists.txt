project(NestPBR)

file(GLOB_RECURSE SOURCES
        src/*.cpp
)

file(GLOB_RECURSE INCLUDE
        src/*.h
        src/*.hpp
)


file(GLOB RESOURCE_FOLDERS
        resources/common/*
        ${NEST_DIRECTORY}/resources/common/*
)

if (PLATFORM_DESKTOP)
    file(GLOB NEST_DESKTOP_RESOURCES ${NEST_DIRECTORY}/resources/desktop/*)
    list(APPEND RESOURCE_FOLDERS "${NEST_DESKTOP_RESOURCES}")
    file(GLOB DESKTOP_RESOURCES resources/desktop/*)
    list(APPEND RESOURCE_FOLDERS "${DESKTOP_RESOURCES}")
elseif (PLATFORM_ANDROID)
    file(GLOB NEST_ANDROID_RESOURCES ${NEST_DIRECTORY}/resources/android/*)
    list(APPEND RESOURCE_FOLDERS "${NEST_ANDROID_RESOURCES}")
    file(GLOB ANDROID_RESOURCES resources/android/*)
    list(APPEND RESOURCE_FOLDERS "${ANDROID_RESOURCES}")
endif ()

add_app(NestPBR "${SOURCES}" "${INCLUDE}" "${RESOURCE_FOLDERS}")

# set output directories for all builds (Debug, Release, etc.)
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set_target_properties(NestPBR
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${BIN_DIRECTORY}
    )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )


set_target_properties(NestPBR
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${BIN_DIRECTORY}
)

# Dependencies - Nest
target_link_libraries(NestPBR Nest)
target_include_directories(NestPBR PRIVATE ${NEST_DIRECTORY}/include)