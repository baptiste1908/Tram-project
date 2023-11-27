# CMakeList.txt : projet CMake pour Tram-project, incluez la source et définissez
# la logique spécifique au projet ici.
#

# Ajoutez une source à l'exécutable de ce projet.
add_executable (Tram-project "Tram-project.cpp" "Tram-project.h")

target_link_libraries(Tram-project ${CMAKE_SOURCE_DIR}/lib/sfml-audio.lib)
target_link_libraries(Tram-project ${CMAKE_SOURCE_DIR}/lib/sfml-graphics.lib)
target_link_libraries(Tram-project ${CMAKE_SOURCE_DIR}/lib/sfml-main.lib)
target_link_libraries(Tram-project ${CMAKE_SOURCE_DIR}/lib/sfml-network.lib)
target_link_libraries(Tram-project ${CMAKE_SOURCE_DIR}/lib/sfml-system.lib)
target_link_libraries(Tram-project ${CMAKE_SOURCE_DIR}/lib/sfml-window.lib)

include_directories(${CMAKE_SOURCE_DIR}/include)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Tram-project PROPERTY CXX_STANDARD 20)
endif()

# TODO: Ajoutez des tests et installez des cibles si nécessaire.
