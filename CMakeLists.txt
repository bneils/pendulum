cmake_minimum_required(VERSION 3.0)
project(Pendulum)
find_package(SDL2 REQUIRED)

add_executable(pendulum
	src/main.cpp
	src/vector.cpp
	src/draw.cpp
)

target_link_libraries(pendulum
	SDL2::SDL2
)
