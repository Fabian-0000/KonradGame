cmake_minimum_required(VERSION 3.5.0)

# Project Settings
project(game)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

set(SOURCE_MAIN 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DarkMode.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Mouse.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MainLoop.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/View.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Keyboard.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MainView.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Collision.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Collider.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Object.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Engine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Controller.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Level.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Input.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Library.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Player.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Time.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Animation.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Camera.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Elevator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/State.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Particle.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Outro.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MainMenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/PauseMenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SaveData.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Entrance.cpp
)

set(SOURCE_OBJECTS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/CameraView.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/ElevatorDoor.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/Panel.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/Destroyable.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/Door.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/Chandelier.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/GlassBox.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/Gun.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/Lever.cpp  
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/WarpDoor.cpp  
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/LockedWarpDoor.cpp  
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/Fan.cpp  
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/Button.cpp  
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/Lightning.cpp  
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/OnShootPower.cpp  
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/Darkness.cpp  
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/Gravitational.cpp  
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/StaticDestroyable.cpp  
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/OnPowerDisappear.cpp  
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/Key.cpp  
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/Rad.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/MovingPlatformHorizontal.cpp  
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/MovingPlatformVertical.cpp  
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/DisappearingPlatform.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/OnPlayerJumpToggle.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/OnTimerToggle.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/OnCollisionWarp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/DestroyableWall.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/SlotMachine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/Teddy.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/OutroTrigger.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/OnCollisionLoadLevel1.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/RevolvingDoor.cpp
)

set(SOURCE_ENEMIES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Enemies/Gonraed.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Enemies/Gondrae.cpp
)

set(HEADER_MAIN
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Vector2.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Rect.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DarkMode.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Mouse.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MainLoop.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/View.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Keyboard.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MainView.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Collision.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Collider.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Transform.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Object.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Engine.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Controller.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Level.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Input.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Library.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Player.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Time.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Animation.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Camera.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Elevator.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/State.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Particle.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Outro.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MainMenu.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/PauseMenu.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/BaseState.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SaveData.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Entrance.hpp
)

set(HEADER_OBJECTS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/CameraView.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/ElevatorDoor.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/Panel.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/Destroyable.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/Door.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/Chandelier.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/GlassBox.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/Gun.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/Lever.hpp  
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/WarpDoor.hpp  
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/LockedWarpDoor.hpp  
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/Fan.hpp  
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/Button.hpp  
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/Lightning.hpp  
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/OnShootPower.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/Darkness.hpp   
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/Gravitational.hpp  
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/StaticDestroyable.hpp  
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/OnPowerDisappear.hpp  
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/Key.hpp  
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/Rad.hpp  
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/MovingPlatformHorizontal.hpp  
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/MovingPlatformVertical.hpp  
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/DisappearingPlatform.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/OnPlayerJumpToggle.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/OnTimerToggle.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/OnCollisionWarp.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/DestroyableWall.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/SlotMachine.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/Teddy.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/OutroTrigger.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/OnCollisionLoadLevel1.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Objects/RevolvingDoor.hpp
)

set(HEADER_ENEMIES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Enemies/Gonraed.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Enemies/Gondrae.hpp
)

# Libs
add_subdirectory(libs/SFML)
add_subdirectory(libs/SDL)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-w)
endif()

if(MSVC)
    add_compile_options(/W0)
endif()

# Executable
add_executable(game ${SOURCE_MAIN} ${SOURCE_OBJECTS} ${SOURCE_ENEMIES} ${HEADER_MAIN} ${HEADER_OBJECTS} ${HEADER_ENEMIES})

source_group("Header Files\\Objects" FILES ${HEADER_OBJECTS})
source_group("Source Files\\Objects" FILES ${SOURCE_OBJECTS})

source_group("Header Files\\Enemies" FILES ${HEADER_ENEMIES})
source_group("Source Files\\Enemies" FILES ${SOURCE_ENEMIES})

target_include_directories(game PUBLIC libs/SFML/include)
target_include_directories(game PUBLIC libs/SDL/include)
target_include_directories(game PUBLIC libs/Json/include)
target_link_libraries(game PUBLIC sfml-graphics)
target_link_libraries(game PUBLIC SDL3::SDL3)
target_link_libraries(game PUBLIC opengl32)

if(WIN32)
    target_link_libraries(game PUBLIC dwmapi)
    set_target_properties(game PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
