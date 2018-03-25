cmake_minimum_required(VERSION 3.2)

set(PROJECT TextureTest)
add_definitions(-std=c++1y)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(SRC Source/Main.cpp
	Source/SurfacelessContext.cpp
	Source/BasicLoader.cpp)

set(LIBS EGL)

add_executable(${PROJECT} ${SRC})
target_link_libraries(${PROJECT} ${LIBS})
