cmake_minimum_required(VERSION 3.0)
project(TramProject)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)


find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(TramProject main.cpp  "metro.cpp"  "superviseur.cpp" "station.cpp" ) 


target_link_libraries(TramProject sfml-graphics sfml-window sfml-system)

