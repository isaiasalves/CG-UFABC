project(asteroids)
add_executable(${PROJECT_NAME} main.cpp openglwindow.cpp asteroids.cpp
                               bullets.cpp bombs.cpp ship.cpp starlayers.cpp ammobar.cpp ammoboxes.cpp)
enable_abcg(${PROJECT_NAME})
