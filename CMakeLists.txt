	# CMakeList.txt: CMake-Projektdatei der obersten Ebene. Führen Sie hier die globale Konfiguration aus,
# und schließen Sie Unterprojekte ein.
#
cmake_minimum_required (VERSION 3.0)

project ("Xouver")


# Schließen Sie Unterprojekte ein.
add_subdirectory ("XouverRT")
add_subdirectory ("XouverKit")
add_subdirectory ("XouverC")
