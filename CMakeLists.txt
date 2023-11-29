# CMakeList.txt : fichier projet CMake de niveau supérieur, effectuez une configuration globale
# et incluez les sous-projets ici.
#
cmake_minimum_required (VERSION 3.8)

project ("Tram-project")

set (CMAKE_CXX_STANDARD 17)

if(MSVC)
    set(SFML_DIR "C:/Program Files/SFML/lib/cmake/SFML")
    set(SFML_STATIC_LIBRARIES TRUE)
    set(CMAKE_CXX_FLAGS_RELEASE "/MT")
    set(CMAKE_CXX_FLAGS_DEBUG "/MTd")
endif()

find_package(SFML 2.6.1 COMPONENTS window system graphics REQUIRED)
link_libraries(sfml-graphics sfml-window)

# Incluez les sous-projets.
add_subdirectory ("Tram-project")


