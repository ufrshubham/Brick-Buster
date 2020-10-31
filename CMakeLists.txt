cmake_minimum_required(VERSION 3.0.0)
project(Brick-Buster VERSION 0.1.0)

# Makes sure custom settings from incldue/b2_user_settings.h are used by box2d.
add_compile_definitions(B2_USER_SETTINGS)
add_compile_definitions(SFML_STATIC)

option(BOX2D_BUILD_UNIT_TESTS "Build the Box2D unit tests" OFF)
option(BOX2D_BUILD_TESTBED "Build the Box2D testbed" OFF)
option(BOX2D_BUILD_DOCS "Build the Box2D documentation" OFF)
option(BOX2D_USER_SETTINGS "Override Box2D settings with b2UserSettings.h" ON)

add_subdirectory(src)
add_subdirectory(vendor/box2d)
add_subdirectory(vendor/sfml)

# Copies assets and its contents to build directory.
if(MSVC)
    file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/src/Debug)
    file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/src/Release)
elseif(UNIX AND NOT APPLE)
    file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/src)
endif()
