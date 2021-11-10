cmake_minimum_required(VERSION 0.1) #beta

set(project_name webbtrader)

project(${project_name})

include_directories(include ./src)

set(CMAKE_CXX_STANDARD 17)

add_executable(${project_name}
  ./main.cpp
  ./src/stockParser.hpp
  ./src/shareProcessor.hpp
  ./src/bar.hpp
  ./src/strategy.hpp
  ./src/definitions.hpp
  )

target_link_libraries(${project_name} stdc++fs)
