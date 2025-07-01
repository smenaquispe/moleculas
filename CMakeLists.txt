cmake_minimum_required(VERSION 3.10)
project(MoleculeViewer)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
pkg_search_module(GLEW REQUIRED glew)
pkg_search_module(GLM REQUIRED glm)

include_directories(
    ${GLFW_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include
)

link_directories(
    ${GLFW_LIBRARY_DIRS}
    ${GLEW_LIBRARY_DIRS}
)

# Archivos fuente
set(SOURCES
    main.cpp
    src/Molecule.cpp
    src/MoleculeLoader.cpp
    src/ShaderUtils.cpp
    src/Atom.cpp
)

add_executable(MoleculeViewer ${SOURCES})

target_link_libraries(MoleculeViewer
    ${OPENGL_LIBRARIES}
    ${GLFW_LIBRARIES}
    ${GLEW_LIBRARIES}
)
