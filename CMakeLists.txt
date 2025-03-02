project("game")
cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SDL2_DIR "${CMAKE_SOURCE_DIR}/lib/SDL2")
set(SDL2_IMAGE_DIR "${CMAKE_SOURCE_DIR}/lib/SDL2_image")
set(SDL2_NET_DIR "${CMAKE_SOURCE_DIR}/lib/SDL2_net")
set(SDL2_TTF_DIR "${CMAKE_SOURCE_DIR}/lib/SDL2_ttf")
set(SDL2_MIXER_DIR "${CMAKE_SOURCE_DIR}/lib/SDL2_mixer")
include_directories(
    "${SDL2_DIR}/include"
    "${SDL2_IMAGE_DIR}/include"
    "${SDL2_NET_DIR}/include"
    "${SDL2_TTF_DIR}/include"
    "${SDL2_MIXER_DIR}/include"
)

#add src files
file(GLOB_RECURSE SRC_FILES src/*.cpp include/*.hpp)

#create executable
add_executable(Game main.cpp ${SRC_FILES})

#define SDL_MAIN_HANDLED globally before including SDL headers
target_compile_definitions(Game PRIVATE SDL_MAIN_HANDLED)

#link SDL2, SDL2_image, SDL2_net, SDL2_ttf, and SDL2_mixer libraries
target_link_libraries(Game 
    "${SDL2_DIR}/lib/x64/SDL2.lib" 
    "${SDL2_IMAGE_DIR}/lib/x64/SDL2_image.lib"
    "${SDL2_NET_DIR}/lib/x64/SDL2_net.lib"
    "${SDL2_TTF_DIR}/lib/x64/SDL2_ttf.lib"
    "${SDL2_MIXER_DIR}/lib/x64/SDL2_mixer.lib"
)

add_custom_command(TARGET Game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${SDL2_DIR}/lib/x64/SDL2.dll"
    "${SDL2_IMAGE_DIR}/lib/x64/SDL2_image.dll"
    "${SDL2_NET_DIR}/lib/x64/SDL2_net.dll"
    "${SDL2_TTF_DIR}/lib/x64/SDL2_ttf.dll"
    "${SDL2_MIXER_DIR}/lib/x64/SDL2_mixer.dll"
    "${SDL2_IMAGE_DIR}/lib/x64/libjpeg-9.dll"
    "${SDL2_IMAGE_DIR}/lib/x64/libpng16-16.dll"
    "${SDL2_IMAGE_DIR}/lib/x64/libtiff-5.dll"
    "${SDL2_IMAGE_DIR}/lib/x64/libwebp-4.dll"
    "${SDL2_IMAGE_DIR}/lib/x64/zlib1.dll"
    $<TARGET_FILE_DIR:Game>
)

message(STATUS "presnet cmake DIR: ${CMAKE_SOURCE_DIR}")
message(STATUS "SDL2_DIR: ${SDL2_DIR}")
message(STATUS "SDL2_IMAGE_DIR: ${SDL2_IMAGE_DIR}")
message(STATUS "SDL2_NET_DIR: ${SDL2_NET_DIR}")
message(STATUS "SDL2_TTF_DIR: ${SDL2_TTF_DIR}")
message(STATUS "SDL2_MIXER_DIR: ${SDL2_MIXER_DIR}")

#check if DLL files exist and report errors
if(NOT EXISTS "${SDL2_DIR}/lib/x64/SDL2.dll")
    message(FATAL_ERROR "SDL2.dll not found at ${SDL2_DIR}/lib/x64/SDL2.dll")
endif()
if(NOT EXISTS "${SDL2_IMAGE_DIR}/lib/x64/SDL2_image.dll")
    message(FATAL_ERROR "SDL2_image.dll not found at ${SDL2_IMAGE_DIR}/lib/x64/SDL2_image.dll")
endif()
if(NOT EXISTS "${SDL2_NET_DIR}/lib/x64/SDL2_net.dll")
    message(FATAL_ERROR "SDL2_net.dll not found at ${SDL2_NET_DIR}/lib/x64/SDL2_net.dll")
endif()
if(NOT EXISTS "${SDL2_TTF_DIR}/lib/x64/SDL2_ttf.dll")
    message(FATAL_ERROR "SDL2_ttf.dll not found at ${SDL2_TTF_DIR}/lib/x64/SDL2_ttf.dll")
endif()
if(NOT EXISTS "${SDL2_MIXER_DIR}/lib/x64/SDL2_mixer.dll")
    message(FATAL_ERROR "SDL2_mixer.dll not found at ${SDL2_MIXER_DIR}/lib/x64/SDL2_mixer.dll")
endif()
