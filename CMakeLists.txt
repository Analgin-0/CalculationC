cmake_minimum_required(VERSION 3.23)

project(Calculator)

set(PROJECT_NAME Calculator)

add_executable(${PROJECT_NAME} srs/main.cpp srs/Calculation.cpp srs/Calculation.hpp)


#c++ 17
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

# установить проект в visual studio в качестве запускаемого
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})