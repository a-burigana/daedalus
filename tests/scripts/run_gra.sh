#!/bin/bash

DAEDALUS=../../cmake-build-debug/DAEDALUS

DOMAIN=grapevine
N_MIN_AGS=3
N_MAX_AGS=5
N_MIN_SECRETS=3
N_MAX_SECRETS=5

N_INSTANCES=36
count=0

print_results() {
    $DAEDALUS --domain $DOMAIN --parameters "$1" "$2" "$3" --semantics "$4"  --strategy "$5" --contraction "$6" --info
    timeout 10m $DAEDALUS --domain $DOMAIN --parameters "$1" "$2" "$3" --semantics "$4"  --strategy "$5" --contraction "$6" --print
}

echo "####################################################################################################"
echo
echo "                                         ${DOMAIN}"
echo
echo "####################################################################################################"

for (( agents_no = N_MIN_AGS; agents_no <= N_MAX_AGS; agents_no++ )); do
    for (( secrets_no = N_MIN_SECRETS; secrets_no <= N_MAX_SECRETS; secrets_no++ )); do
        for (( learning_ags_no = 1; learning_ags_no <= agents_no; learning_ags_no++ )); do
            count=$((count+1))
            echo $'\n\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Instance ' "${count}/${N_INSTANCES}" $'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n'

            print_results "$agents_no" "$secrets_no" "$learning_ags_no" kripke  bounded canonical
#            print_results "$agents_no" "$secrets_no" "$learning_ags_no" delphic unbounded
            print_results "$agents_no" "$secrets_no" "$learning_ags_no" kripke  unbounded full
        done
    done
done
