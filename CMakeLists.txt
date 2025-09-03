
cmake_minimum_required(VERSION 3.28)
project(vdc)

add_compile_options(-Wno-all) # disable warnings

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(LIB_DIR ${CMAKE_SOURCE_DIR}/lib)


set(ARMADILLO_INCLUDE_DIR ${LIB_DIR}/armadillo-code/include)
set(GLAD_INCLUDE_DIR ${LIB_DIR}/glad/include)
set(GLAD_SOURCES ${LIB_DIR}/glad/src/glad.c)
set(GLFW_INCLUDE_DIR ${LIB_DIR}/glfw/include)
set(GLFW_LIBRARY ${LIB_DIR}/glfw/build/src/libglfw3.a)
set(IMGUI_INCLUDE_DIR ${LIB_DIR}/imgui)
set(IMGUI_SOURCES
        ${IMGUI_INCLUDE_DIR}/imgui.cpp
        ${IMGUI_INCLUDE_DIR}/imgui_draw.cpp
        ${IMGUI_INCLUDE_DIR}/imgui_widgets.cpp
        ${IMGUI_INCLUDE_DIR}/imgui_tables.cpp
        ${IMGUI_INCLUDE_DIR}/imgui_demo.cpp
        ${IMGUI_INCLUDE_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_INCLUDE_DIR}/backends/imgui_impl_opengl3.cpp
)
set(IMPLOT_INCLUDE_DIR ${LIB_DIR}/implot)
set(IMPLOT_SOURCES
        ${IMPLOT_INCLUDE_DIR}/implot.cpp
        ${IMPLOT_INCLUDE_DIR}/implot_items.cpp
        ${IMPLOT_INCLUDE_DIR}/implot_demo.cpp
)
set(JSON_INCLUDE_DIR ${LIB_DIR}/json/include)


include_directories(
        ${ARMADILLO_INCLUDE_DIR}
        ${GLFW_INCLUDE_DIR}
        ${GLAD_INCLUDE_DIR}
        ${IMGUI_INCLUDE_DIR}
        ${IMPLOT_INCLUDE_DIR}
        ${JSON_INCLUDE_DIR}
)


add_executable(vdc main.cpp
        ${IMGUI_SOURCES}
        ${IMPLOT_SOURCES}
        ${GLAD_SOURCES}
)

target_link_libraries(vdc ${GLFW_LIBRARY})
