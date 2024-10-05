#!/bin/bash

DAEDALUS=../../cmake-build-debug/DAEDALUS

DOMAIN=coin_in_the_box
MIN_ID=1
MAX_ID=5

N_INSTANCES=5
count=0

print_results() {
    $DAEDALUS --domain $DOMAIN --parameters "$1" --semantics "$2"  --strategy "$3" --info
    timeout 10m $DAEDALUS --domain $DOMAIN --parameters "$1" --semantics "$2"  --strategy "$3" --print
}

for (( i = MIN_ID; i <= MAX_ID; i++ )); do
    count=$((count+1))
    echo $'\n\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Instance ' "${count}/${N_INSTANCES}" $'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n'

    print_results "$i" kripke bounded
    print_results "$i" delphic unbounded
    print_results "$i" kripke unbounded
done
