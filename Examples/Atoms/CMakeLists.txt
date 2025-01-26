project(Atoms)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES
        main.cpp
        src/*.cpp
)

file(GLOB_RECURSE INCLUDE
        src/*.h
        src/*.hpp
        include/*.hpp
)


file(GLOB RESOURCE_FOLDERS
        resources/*
        ${NEST_DIRECTORY}/resources/common/*
)

file(GLOB NEST_DESKTOP_RESOURCES ${NEST_DIRECTORY}/resources/desktop/*)
list(APPEND RESOURCE_FOLDERS "${NEST_DESKTOP_RESOURCES}")


add_app(Atoms "${SOURCES}" "${INCLUDE}" "${RESOURCE_FOLDERS}")

# set output directories for all builds (Debug, Release, etc.)
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set_target_properties(Atoms
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${BIN_DIRECTORY}
    )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )


set_target_properties(Atoms
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${BIN_DIRECTORY}
)

# Dependencies - Nest
target_link_libraries(Atoms Nest)
target_include_directories(Atoms PRIVATE ${NEST_DIRECTORY}/include)