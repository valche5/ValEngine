project(ValEngine)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(./src SRC_LIST)

set(CMAKE_CXX_FLAGS "-std=c++11 -g -ggdb -O0 -Wall -Wextra")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(lib/SOIL)
include_directories(lib/SOIL/src/)
include_directories(lib/)
add_subdirectory(lib/glm)

link_directories(${CMAKE_SOURCE_DIR}/lib/assimp/lib)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(OpenGL REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)

add_definitions(${Qt5Core_DEFINITIONS})
add_definitions(${Qt5Qui_DEFINITIONS})
add_definitions(${Qt5Widgets_DEFINITIONS})
add_definitions(${Qt5OpenGL_DEFINITIONS})

set(Qt5_LIBRARIES ${Qt5Core_LIBRARIES} ${Qt5Gui_LIBRARIES} ${Qt5Widgets_LIBRARIES} ${Qt5OpenGL_LIBRARIES})

FILE(GLOB folder_shader  shaders/*.* shaders/*/*.*)

include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${Qt5Widgets_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${SRC_LIST} ${folder_shader})

target_link_libraries(${PROJECT_NAME} libassimp.dylib)
target_link_libraries(${PROJECT_NAME} SOIL ${Qt5_LIBRARIES} ${OPENGL_LIBRARIES})
