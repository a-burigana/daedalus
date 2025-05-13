#!/bin/bash

DOMAIN=tiger
N_MIN_DOORS=3
N_MAX_DOORS=8
N_MIN_TIGERS=1

N_INSTANCES=27
count=0

print_results() {
    $DAEDALUS --domain $DOMAIN --parameters "$1" "$2" --semantics "$3"  --strategy "$4" --contraction "$5" --info
    timeout 20m "$DAEDALUS" --domain $DOMAIN --parameters "$1" "$2" --semantics "$3"  --strategy "$4" --contraction "$5" --print
}

echo "####################################################################################################"
echo
echo "                                         ${DOMAIN}"
echo
echo "####################################################################################################"

for (( doors_no = N_MIN_DOORS; doors_no <= N_MAX_DOORS; doors_no++ )); do
    for (( tigers_no = N_MIN_TIGERS; tigers_no < doors_no; tigers_no++ )); do
        count=$((count+1))
        echo $'\n\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Instance ' "${count}/${N_INSTANCES}" $'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n'

        print_results "$doors_no" "$tigers_no" kripke approx_bounded canonical
        print_results "$doors_no" "$tigers_no" kripke bounded rooted
    done
done
