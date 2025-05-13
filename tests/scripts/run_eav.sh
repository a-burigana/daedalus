#!/bin/bash

DOMAIN=eavesdropping
N_MIN_AGS=2
N_MAX_AGS=8
N_MIN_STEPS=2
N_MAX_STEPS=8

N_INSTANCES=49
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

for (( ags_no = N_MIN_AGS; ags_no <= N_MAX_AGS; ags_no++ )); do
    for (( steps_no = N_MIN_STEPS; steps_no < N_MAX_STEPS; steps_no++ )); do
        count=$((count+1))
        echo $'\n\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Instance ' "${count}/${N_INSTANCES}" $'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n'

        print_results "$ags_no" "$steps_no" kripke approx_bounded canonical
        print_results "$ags_no" "$steps_no" kripke bounded rooted
    done
done
