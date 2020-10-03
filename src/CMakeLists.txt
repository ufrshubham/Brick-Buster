set(BRICK_BUSTER_SOURCE_FILES
    AssetMan.cpp
    Game.cpp
    GameOver.cpp
    GamePlay.cpp
    main.cpp
    MainMenu.cpp
    PauseGame.cpp
    StateMan.cpp)

set(BRICK_BUSTER_HEADER_FILES
    ../include/AssetMan.hpp
    ../include/Game.hpp
    ../include/GameOver.hpp
    ../include/GamePlay.hpp
    ../include/MainMenu.hpp
    ../include/PauseGame.hpp
    ../include/State.hpp
    ../include/StateMan.hpp
    ../include/b2_user_settings.h)

add_executable(Brick-Buster ${BRICK_BUSTER_SOURCE_FILES} ${BRICK_BUSTER_HEADER_FILES})

target_include_directories(Brick-Buster PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_link_libraries(Brick-Buster sfml-window sfml-graphics sfml-system box2d)
set_target_properties(Brick-Buster PROPERTIES 
                                   CXX_STANDARD 17 
                                   CXX_STANDARD_REQUIRED YES)

set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "${PROJECT_SOURCE_DIR}/vendor/box2d/include" "${PROJECT_SOURCE_DIR}/vendor/sfml/include")
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/vendor/box2d/lib" "${PROJECT_SOURCE_DIR}/vendor/sfml/lib")
