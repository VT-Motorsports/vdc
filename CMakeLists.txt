
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


# --- ChassisSim Engine component (standalone Win64 DLL) ---------------------
# Builds ONLY the torque vectoring wrapper as a 64-bit DLL. It pulls in nothing
# from the GUI app (the wrapper ships its own ImVec4 shim), so the vdc target
# above is left completely untouched. Build with a 64-bit MinGW toolchain so the
# resulting tv_engine.dll uses the stable Win64 C ABI the MSVC-built host loads.
add_library(tv_engine SHARED
        src/engine/tv_engine.cpp
)
target_include_directories(tv_engine PRIVATE ${CMAKE_SOURCE_DIR}/src/engine)
set_target_properties(tv_engine PROPERTIES
        PREFIX ""              # emit tv_engine.dll, not libtv_engine.dll
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)


# --- ChassisSim engine map component (standalone Win64 DLL) -----------------
# Minimal debug component: throttle + engine rpm -> four wheel torques via a
# hardcoded torque map. Completely independent of the tv_engine target above and
# of the GUI app; build it with the same 64-bit MinGW toolchain so engine_map.dll
# uses the stable Win64 C ABI the MSVC-built host loads.
add_library(engine_map SHARED
        src/engine/engine_map.cpp
)
target_include_directories(engine_map PRIVATE ${CMAKE_SOURCE_DIR}/src/engine)
set_target_properties(engine_map PROPERTIES
        PREFIX ""              # emit engine_map.dll, not libengine_map.dll
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)
