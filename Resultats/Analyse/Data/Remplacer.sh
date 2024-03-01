#!/bin/bash

# Chemin vers le répertoire contenant les fichiers à traiter
#repertoire="/chemin/vers/votre/repertoire"

# Ancienne chaîne de caractères à remplacer
#ancienne_chaine="ZnS_0.1mm_500us_0"
ancienne_chaine="_0"

# Nouvelle chaîne de caractères de remplacement
#nouvelle_chaine="Linearite_"
nouvelle_chaine="_"

# Naviguer vers le répertoire
#cd "$repertoire" || exit

# Boucler sur tous les fichiers dans le répertoire
for fichier in *; do
    # Vérifier si le fichier est un fichier ordinaire
    if [ -f "$fichier" ]; then
        # Utiliser le programme 'rename' pour renommer les fichiers
        nouveau_nom=$(echo "$fichier" | sed "s/$ancienne_chaine/$nouvelle_chaine/")

        # Renommer le fichier
        mv "$fichier" "$nouveau_nom"

        echo "Fichier renommé : $fichier -> $nouveau_nom"
    fi
done

echo "Opération terminée."
