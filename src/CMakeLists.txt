set(BRICK_BUSTER_SOURCE_FILES
    AssetMan.cpp
    Game.cpp
    GameOver.cpp
    Level.cpp
    Level1.cpp
    main.cpp
    MainMenu.cpp
    PauseGame.cpp
    StateMan.cpp
    ContactListener.cpp
    Body.cpp
    Ball.cpp
    Paddle.cpp
    Wall.cpp
    Brick.cpp)

set(BRICK_BUSTER_HEADER_FILES
    ../include/AssetMan.hpp
    ../include/Game.hpp
    ../include/GameOver.hpp
    ../include/Level.hpp
    ../include/Level1.hpp
    ../include/MainMenu.hpp
    ../include/PauseGame.hpp
    ../include/State.hpp
    ../include/StateMan.hpp
    ../include/b2_user_settings.h
    ../include/ContactListener.hpp
    ../include/Body.hpp
    ../include/Ball.hpp
    ../include/Paddle.hpp
    ../include/Wall.hpp
    ../include/Brick.hpp)

add_executable(Brick-Buster ${BRICK_BUSTER_SOURCE_FILES} ${BRICK_BUSTER_HEADER_FILES})

target_include_directories(Brick-Buster PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include/ 
                                               ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/box2d/include/ 
                                               ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/sfml/include/)

set_target_properties(Brick-Buster PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED YES)

target_link_directories(Brick-Buster PUBLIC 
                        ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/box2d/lib/release/ 
                        ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/sfml/lib/release/
                        ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/box2d/lib/debug/ 
                        ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/sfml/lib/debug/)

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

if(LINUX)
    find_library(LIBBOX2D Box2D DIRECTORY)
    target_link_libraries(Brick-Buster ${LIBBOX2D} sfml-window sfml-graphics sfml-system)
elseif(WIN32)
    target_link_libraries(Brick-Buster opengl32 freetype flac ogg openal32 vorbis vorbisenc vorbisfile winmm gdi32 
                            debug sfml-window-s-d debug sfml-graphics-s-d debug sfml-system-s-d debug box2d-d
                            optimized sfml-window-s optimized sfml-graphics-s optimized sfml-system-s optimized box2d)
else()
    message(FATAL_ERROR "Unsupported platform detected.")
endif()

