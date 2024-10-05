#!/bin/bash

DAEDALUS=../../cmake-build-debug/DAEDALUS

DOMAIN=consecutive_numbers
MIN_N=5
MAX_N=20

N_INSTANCES=16
count=0

print_results() {
    $DAEDALUS --domain $DOMAIN --parameters "$1" --semantics "$2"  --strategy "$3" --info
    timeout 10m $DAEDALUS --domain $DOMAIN --parameters "$1" --semantics "$2"  --strategy "$3" --print
}

for (( n = MIN_N; n <= MAX_N; n++ )); do
    count=$((count+1))
    echo $'\n\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Instance ' "${count}/${N_INSTANCES}" $'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n'

    print_results "$n" kripke  bounded
    print_results "$n" delphic unbounded
    print_results "$n" kripke  unbounded
done
