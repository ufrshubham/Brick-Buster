cmake_minimum_required(VERSION 3.0.0)
project(Brick-Buster VERSION 0.1.0)

# Makes sure custom settings from incldue/b2_user_settings.h are used by box2d.
add_compile_definitions(B2_USER_SETTINGS)

add_subdirectory(src)

# Copies assets and its contents to build directory.
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
