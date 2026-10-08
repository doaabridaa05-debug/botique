cmake_minimum_required(VERSION 3.16)

# Nom du projet et langage utilisé
project(boutique_tui LANGUAGES CXX)

# On utilise le standard C++17
set(CMAKE_CXX_STANDARD 17)

# C++17 est obligatoire
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Désactive les extensions spécifiques au compilateur pour plus de portabilité
set(CMAKE_CXX_EXTENSIONS OFF)

# Configuration spéciale pour Visual Studio (MSVC)
# Configuration spéciale pour Visual Studio (MSVC)
if(MSVC)
  add_compile_options(/utf-8)
endif()

# Module CMake pour télécharger automatiquement des bibliothèques externes
include(FetchContent)

# Module CMake pour télécharger automatiquement des bibliothèques externes
FetchContent_Declare(
  ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI.git
  GIT_TAG v6.1.9
)

# Téléchargement et intégration automatique de FTXUI dans le projet
FetchContent_MakeAvailable(ftxui)

# Création de l'exécutable principal du projet
add_executable(boutique_app 
    main.cpp 
    Magasin.cpp 
    Interface.cpp
)

# Lien avec les modules de la bibliothèque FTXUI
target_link_libraries(boutique_app PRIVATE 
    ftxui::screen 
    ftxui::dom 
    ftxui::component
)















