#!/bin/bash

DAEDALUS=../../cmake-build-debug/DAEDALUS

DOMAIN=gossip
N_MIN_AGS=3
N_MAX_AGS=5
N_MIN_SECRETS=3
MIN_GOAL_ID=1
MAX_GOAL_ID=9

N_INSTANCES=54
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

for (( agents_no = N_MIN_AGS; agents_no <= N_MAX_AGS; agents_no++ )); do
    for (( secrets_no = N_MIN_SECRETS; secrets_no <= agents_no; secrets_no++ )); do
        for (( goal_id = MIN_GOAL_ID; goal_id <= MAX_GOAL_ID; goal_id++ )); do
            count=$((count+1))
            echo $'\n\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Instance ' "${count}/${N_INSTANCES}" $'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n'

            print_results "$agents_no" "$secrets_no" "$goal_id" kripke  bounded
            print_results "$agents_no" "$secrets_no" "$goal_id" delphic unbounded
            print_results "$agents_no" "$secrets_no" "$goal_id" kripke  unbounded
        done
    done
done
