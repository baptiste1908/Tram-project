# CMakeList.txt : projet CMake pour Tram-project, incluez la source et définissez
# la logique spécifique au projet ici.
#

# Ajoutez une source à l'exécutable de ce projet.
add_executable (Tram-project "Tram-project.cpp" "Tram-project.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Tram-project PROPERTY CXX_STANDARD 20)
endif()

# TODO: Ajoutez des tests et installez des cibles si nécessaire.
