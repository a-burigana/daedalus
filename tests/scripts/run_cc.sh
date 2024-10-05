#!/bin/bash

DAEDALUS=../../cmake-build-debug/DAEDALUS

DOMAIN=collaboration_communication
N_MIN_AGS=2
N_MAX_AGS=4
N_MAX_ROOMS=5
MIN_GOAL_ID=1
MAX_GOAL_ID=4

N_INSTANCES=24
count=0

print_results() {
    $DAEDALUS --domain $DOMAIN --parameters "$1" "$2" "$3" "$4" --semantics "$5"  --strategy "$6" --info
    timeout 10m $DAEDALUS --domain $DOMAIN --parameters "$1" "$2" "$3" "$4" --semantics "$5"  --strategy "$6" --print
}

for (( agents_no = N_MIN_AGS; agents_no <= N_MAX_AGS; agents_no++ )); do
    for (( rooms_no = agents_no+1; rooms_no <= N_MAX_ROOMS; rooms_no++ )); do
        for (( goal_id = MIN_GOAL_ID; goal_id <= MAX_GOAL_ID; goal_id++ )); do
            count=$((count+1))
            echo $'\n\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Instance ' "${count}/${N_INSTANCES}" $'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n'

            boxes_no=$((rooms_no-1))

            print_results "$agents_no" "$rooms_no" "$boxes_no" "$goal_id" kripke  bounded
            print_results "$agents_no" "$rooms_no" "$boxes_no" "$goal_id" delphic unbounded
            print_results "$agents_no" "$rooms_no" "$boxes_no" "$goal_id" kripke  unbounded
        done
    done
done
