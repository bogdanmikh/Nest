project(NestPBR)

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

file(GLOB RESOURCE_FOLDERS res/*)

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
