### Build ImGui and backends (GLFW, Vulkan)

# GLFW
find_package(glfw3 CONFIG REQUIRED)

# Vulkan
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DVK_PROTOTYPES")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_PROTOTYPES")
find_package(Vulkan REQUIRED)

# Compile ImGui into a static library

cmake_path(GET CMAKE_CURRENT_LIST_DIR PARENT_PATH PARENT_DIR)
set(EXTERN_PATH ${PARENT_DIR}/extern)
message("EXTERN_PATH: ${EXTERN_PATH}")

set(IMGUI_DIR ${EXTERN_PATH}/imgui)
set(IMGUI_SRC
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp)

set(IMGUI_INCLUDE
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

# glm
find_package(glm CONFIG REQUIRED)

# stb
find_package(Stb REQUIRED)

# Walnut
set(WALNUT_DIR ${EXTERN_PATH}/Walnut)
set(WALNUT_SRC
    ${WALNUT_DIR}/Walnut/src/Walnut/Application.cpp
    ${WALNUT_DIR}/Walnut/src/Walnut/Image.cpp
    ${WALNUT_DIR}/Walnut/src/Walnut/Random.cpp
    ${WALNUT_DIR}/Walnut/src/Walnut/Input/Input.cpp
    ${WALNUT_DIR}/Walnut/src/Walnut/Input/Input.cpp
)
add_library(Walnut STATIC ${IMGUI_SRC} ${WALNUT_SRC} )
target_include_directories(Walnut PUBLIC 
    ${IMGUI_INCLUDE}
    ${WALNUT_DIR}/Walnut/src
    ${Stb_INCLUDE_DIR}
)
target_link_libraries(Walnut
    PUBLIC
    glfw
    Vulkan::Vulkan
    glm::glm
)