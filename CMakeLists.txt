cmake_minimum_required(VERSION 3.6.0)
project(Automobile-Traffic-Simulation VERSION 0.1.0 LANGUAGES C)
    
add_executable(Automobile-Traffic-Simulation 
    external/stb/stb_image.c
    external/glad/glad.c
    source/main.c 
    source/algorithms.c
    source/cars.c
    source/roads.c
    source/map.c
    source/render.c
    source/camera.c
    source/shader.c
    source/gui.c
    source/cross.c
    source/search_tools.c
    source/spawn_logs.c
    source/traffic_density.c
    source/traffic_light.c
    )

if (WIN32)
    target_link_libraries(Automobile-Traffic-Simulation ${CMAKE_SOURCE_DIR}/external/GLFW/glfw3.lib)
else()
    target_link_libraries(Automobile-Traffic-Simulation glfw m)
endif()

include_directories(${CMAKE_SOURCE_DIR}/external ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/source)
