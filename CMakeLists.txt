cmake_minimum_required(VERSION 3.10)
project(TinyWingsReskin)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Trouver SFML
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 3.0 COMPONENTS System Window Graphics Audio Network REQUIRED)

# Inclure les répertoires
include_directories(include)
include_directories(Engine)

# Bibliothèque Engine
add_library(Engine STATIC
    Engine/Engine.cpp
    Engine/GameObject.cpp
    Engine/GravityComponent.cpp
    Engine/HillComponent.cpp
    Engine/InputHandler.cpp
    Engine/ModuleManager.cpp
    Engine/ResourceModule.cpp
    Engine/Scene.cpp
    Engine/SceneModule.cpp
    Engine/ScoreRenderer.cpp
    Engine/SpriteRendererComponent.cpp
    Engine/TimerComponent.cpp
    Engine/TimerRenderer.cpp
    Engine/WindowModule.cpp
)

target_link_libraries(Engine SFML::Graphics SFML::Window SFML::System SFML::Audio SFML::Network)

# Exécutable Game
add_executable(Game
    Game/Background.cpp
    Game/BaseScene.cpp
    Game/Client.cpp
    Game/MainScene.cpp
    Game/LevelGenerator.cpp
    Game/PlayScene.cpp
    Game/TestState.cpp
    Game/PlayerInput.cpp
    Game/Server.cpp
    Game/World.cpp
)

target_link_libraries(Game Engine SFML::Graphics SFML::Window SFML::System SFML::Audio SFML::Network)

# Copier les assets
file(COPY Assets DESTINATION ${CMAKE_BINARY_DIR})