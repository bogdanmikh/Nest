cmake_minimum_required(VERSION 3.22)

set(NameProject NestCraft)

set(CMAKE_CXX_STANDARD 20)

project(${NameProject})

add_subdirectory(Nest)
add_subdirectory(NestUI)

add_executable(${NameProject} ${SOURCESNEST} ${SOURCESNESTUI})

#------------------------------------------------------------------------------------------------------------------#
#add_subdirectory(Nest)

# GLFW
add_subdirectory(Vendor/glfw)

target_link_libraries(${NameProject} glfw)
target_include_directories(${NameProject} PRIVATE Vendor/glfw/include)

# GLAD
add_subdirectory(Vendor/glad)

target_link_libraries(${NameProject} glad)
target_include_directories(${NameProject} PRIVATE Vendor/glad/include)

# GLM
add_subdirectory(Vendor/glm)

target_link_libraries(${NameProject} glm)
target_include_directories(${NameProject} PRIVATE Vendor/glm)

# STB_IMAGE
add_subdirectory(Vendor/stb_image)

target_link_libraries(${NameProject} stb_image)
target_include_directories(${NameProject} PRIVATE Vendor/stb_image)

# imgui
add_subdirectory(Vendor/imgui)

target_link_libraries(${NameProject} imgui)
target_include_directories(${NameProject} PRIVATE Vendor/imgui)

# Nest
target_include_directories(${NameProject} PRIVATE Nest/include)

# NestUI
target_include_directories(${NameProject} PRIVATE NestUI/include)


