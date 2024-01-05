cmake_minimum_required(VERSION 3.0)
project(TramProject) # Remplacez TramProject par le nom réel de votre projet

# Spécifier la version de C++
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Trouver SFML. Assurez-vous que SFML est installé et que CMake peut le trouver
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Ajouter les fichiers sources de votre projet
add_executable(TramProject main.cpp superviseur.hpp metro.hpp) # Listez tous vos fichiers .cpp ici

# Lier votre projet avec les bibliothèques de SFML
target_link_libraries(TramProject sfml-graphics sfml-window sfml-system)

#file(COPY ${CMAKE_SOURCE_DIR}/images/ DESTINATION ${CMAKE_BINARY_DIR}/images/)