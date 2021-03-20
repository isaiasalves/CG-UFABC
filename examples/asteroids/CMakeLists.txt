project(asteroids)
add_executable(${PROJECT_NAME} main.cpp openglwindow.cpp asteroids.cpp
                               bullets.cpp ship.cpp starlayers.cpp ammobar.cpp bombs.cpp)
enable_abcg(${PROJECT_NAME})