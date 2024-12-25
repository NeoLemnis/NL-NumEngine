#!/bin/bash

# Vérifier si compile_commands.json existe
if [ ! -f "./compile_commands.json" ]; then
    echo "Erreur : compile_commands.json introuvable."
    echo "Assurez-vous d'avoir généré ce fichier avec CMake."
    exit 1
fi

# Exécuter run-clang-tidy
run-clang-tidy -p . -header-filter=".*" -fix -j $(nproc) -quiet
