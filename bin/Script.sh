 #!/bin/bash


echo "Début de traitement"
#myArr=("10" "9" "8" "7" "6" "5" "4" "3" "2" "1")
myArr=("2 4 1" "2 4 2")
myArr1=("He1+" "He2+")
myArr2=(1000 1000)


for idx in ${!myArr[*]}
do
    while [[ $(pgrep -x AIFIRA2023Sim | wc -l) -gt 6 ]]
    do
	sleep 5
    done
        #Init
    value=${myArr[$idx]}
        idx_fichier=$(($idx))
        echo Index : $idx / Fichier : $value
        # Creation du fichier a partir du fichier de base (fichier temporaire)
        cp vrml_base.mac base_$idx_fichier_bis.mac
        # Mise en place de la variable
        #sed -e "s/%energy/$value/g" base_$idx_fichier_bis.mac > base_$idx_fichier.mac
	sed -e "s/%ion/$value/g" base_$idx_fichier_bis.mac > base_$idx_fichier.mac
        # Suppression fichier temporaire
        rm base_$idx_fichier_bis.mac
	./AIFIRA2023Sim ${myArr1[$idx]}_EJ212_1mm 100000 base_$idx_fichier.mac &
	#./AIFIRA2023Sim He2+_Config2_ZnS_0.1_${myArr[$idx]}MeV 100 base_$idx_fichier.mac &
	sleep 4
	#rm base_$idx_fichier.mac
done
echo "Fin de traitement"


