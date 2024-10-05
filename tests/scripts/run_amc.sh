#!/bin/bash

DAEDALUS=../../cmake-build-debug/DAEDALUS

DOMAIN=active_muddy_children
N_MIN_CHILDREN=5
N_MAX_CHILDREN=8
N_MIN_MUDDY=2

N_INSTANCES=40
count=0

print_results() {
    $DAEDALUS --domain $DOMAIN --parameters "$1" "$2" "$3" --semantics "$4"  --strategy "$5" --info
    timeout 10m $DAEDALUS --domain $DOMAIN --parameters "$1" "$2" "$3" --semantics "$4"  --strategy "$5" --print
}

echo "####################################################################################################"
echo
echo "                                         ${DOMAIN}"
echo
echo "####################################################################################################"

for (( children_no = N_MIN_CHILDREN; children_no <= N_MAX_CHILDREN; children_no++ )); do
    for (( muddy_no = N_MIN_MUDDY; muddy_no <= children_no; muddy_no++ )); do
        for (( is_muddy_0 = 0; is_muddy_0 <= 1; is_muddy_0++ )); do
            if [[ "$is_muddy_0" == 1 || "$muddy_no" < "$children_no" ]]; then
                count=$((count+1))
                echo $'\n\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Instance ' "${count}/${N_INSTANCES}" $'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n'

                print_results "$children_no" "$muddy_no" "$is_muddy_0" kripke  bounded
                print_results "$children_no" "$muddy_no" "$is_muddy_0" delphic unbounded
                print_results "$children_no" "$muddy_no" "$is_muddy_0" kripke  unbounded
            fi
        done
    done
done
