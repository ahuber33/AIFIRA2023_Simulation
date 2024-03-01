 #!/bin/bash


for (( i=1 ; i<=8 ; i++ ))
do
    while [[ $(pgrep -x AIFIRA2023Sim | wc -l) -gt 7 ]]
    do
	sleep 10
    done
    ./AIFIRA2023Sim b_${i} 500 vrml.mac &
    sleep 10
    done
