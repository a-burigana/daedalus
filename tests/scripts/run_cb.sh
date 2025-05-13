#!/bin/bash

DOMAIN=coin_in_the_box
MIN_ID=1
MAX_ID=5

N_INSTANCES=6
count=0

print_results() {
    $DAEDALUS --domain $DOMAIN --parameters "$1" --semantics "$2" --strategy "$3" --contraction "$4" --info
    timeout 20m "$DAEDALUS" --domain $DOMAIN --parameters "$1" --semantics "$2"  --strategy "$3" --contraction "$4" --print
}

echo "####################################################################################################"
echo
echo "                                         ${DOMAIN}"
echo
echo "####################################################################################################"

for (( i = MIN_ID; i <= MAX_ID; i++ )); do
    count=$((count+1))
    echo $'\n\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Instance ' "${count}/${N_INSTANCES}" $'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n'

    print_results "$i" kripke approx_bounded canonical
    print_results "$i" kripke bounded rooted
done
