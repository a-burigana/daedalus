#!/bin/bash

DOMAIN=consecutive_numbers
MIN_N=5
MAX_N=60

N_INSTANCES=36
count=0

print_results() {
    $DAEDALUS --domain $DOMAIN --parameters "$1" --semantics "$2"  --strategy "$3" --contraction "$4" --info
    timeout 20m "$DAEDALUS" --domain $DOMAIN --parameters "$1" --semantics "$2"  --strategy "$3" --contraction "$4" --print
}

echo "####################################################################################################"
echo
echo "                                         ${DOMAIN}"
echo
echo "####################################################################################################"

for (( n = MIN_N; n <= MAX_N; n++ )); do
    count=$((count+1))
    echo $'\n\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Instance ' "${count}/${N_INSTANCES}" $'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n'

    print_results "$n" kripke approx_bounded canonical
    print_results "$n" kripke bounded rooted
done
