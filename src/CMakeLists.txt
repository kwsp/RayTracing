add_executable(RayTracing
    main.cpp
    Renderer.cpp
    Camera.cpp
)
target_include_directories(RayTracing PRIVATE .)
target_compile_definitions(RayTracing PRIVATE GLM_ENABLE_EXPERIMENTAL)
target_compile_definitions(RayTracing PRIVATE WL_PLATFORM_WINDOWS)
target_link_libraries(RayTracing
    PRIVATE
    Walnut
)