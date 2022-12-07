# CMakeList.txt: Projekt CMake pro had, sem přidejte logiku zdrojového
# kódu a definic specifickou pro projekt.
#
cmake_minimum_required (VERSION 3.8)

# Přidejte zdrojový kód ke spustitelnému souboru tohoto projektu.
add_executable (had "had.cpp" "had.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET had PROPERTY CXX_STANDARD 20)
endif()

# TODO: V případě potřeby přidejte testy a cíle instalace.
