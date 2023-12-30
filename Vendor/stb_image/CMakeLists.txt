project(stb_image)

set(SOURCES stb_image.cpp)
set(HEADERS stb_image.h)

add_library(stb_image STATIC ${SOURCES} ${HEADERS})
target_include_directories(stb_image PRIVATE include)