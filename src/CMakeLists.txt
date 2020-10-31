set(BRICK_BUSTER_SOURCE_FILES
    AssetMan.cpp
    Game.cpp
    GameOver.cpp
    Level.cpp
    Level1.cpp
    Level2.cpp
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
    ../include/Level2.hpp
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

set_target_properties(Brick-Buster PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED YES)

target_include_directories(Brick-Buster PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include/ 
                                               ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/box2d/include/ 
                                               ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/sfml/include/)

target_link_libraries(Brick-Buster sfml-window sfml-graphics sfml-system box2d)
