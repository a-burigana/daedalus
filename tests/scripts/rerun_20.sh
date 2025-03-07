#!/bin/bash

DAEDALUS=../../cmake-build-debug/DAEDALUS

print_amc() {
    $DAEDALUS --domain amc --parameters "$1" "$2" "$3" --semantics "$4"  --strategy "$5" --contraction "$6" --info
    timeout 20m "$DAEDALUS" --domain amc --parameters "$1" "$2" "$3" --semantics "$4"  --strategy "$5" --contraction "$6" --print
}

print_cc() {
    $DAEDALUS --domain cc --parameters "$1" "$2" "$3" "$4" --semantics "$5"  --strategy "$6" --contraction "$7" --info
    timeout 20m "$DAEDALUS" --domain cc --parameters "$1" "$2" "$3" "$4" --semantics "$5"  --strategy "$6" --contraction "$7" --print
}

print_eav() {
    $DAEDALUS --domain eav --parameters "$1" "$2" --semantics "$3"  --strategy "$4" --contraction "$5" --info
    timeout 20m "$DAEDALUS" --domain eav --parameters "$1" "$2" --semantics "$3"  --strategy "$4" --contraction "$5" --print
}

print_gos() {
    $DAEDALUS --domain gos --parameters "$1" "$2" "$3" --semantics "$4"  --strategy "$5" --contraction "$6" --info
    timeout 20m "$DAEDALUS" --domain gos --parameters "$1" "$2" "$3" --semantics "$4"  --strategy "$5" --contraction "$6" --print
}

print_gra() {
    $DAEDALUS --domain gra --parameters "$1" "$2" "$3" --semantics "$4"  --strategy "$5" --contraction "$6" --info
    timeout 20m "$DAEDALUS" --domain gra --parameters "$1" "$2" "$3" --semantics "$4"  --strategy "$5" --contraction "$6" --print
}

print_sc() {
    $DAEDALUS --domain sc --parameters "$1" "$2" "$3" --semantics "$4"  --strategy "$5" --contraction "$6" --info
    timeout 20m "$DAEDALUS" --domain sc --parameters "$1" "$2" "$3" --semantics "$4"  --strategy "$5" --contraction "$6" --print
}

print_tig() {
    $DAEDALUS --domain tig --parameters "$1" "$2" --semantics "$3"  --strategy "$4" --contraction "$5" --info
    timeout 20m "$DAEDALUS" --domain tig --parameters "$1" "$2" --semantics "$3"  --strategy "$4" --contraction "$5" --print
}

echo "####################################################################################################"
echo
echo "                                         amc"
echo
echo "####################################################################################################"

amc_param_1=(10 11 12 12 12 12 12 12)
amc_param_2=( 9  8  2  3  4  5  6  7)
amc_param_3=( 1  1  1  1  1  1  1  1)

for i in {0..7}; do
    print_amc "${amc_param_1[$i]}" "${amc_param_2[$i]}" "${amc_param_3[$i]}" kripke unbounded full
done



# echo "####################################################################################################"
# echo
# echo "                                         cc"
# echo
# echo "####################################################################################################"

# cc_param_1=(2 2 3 3 4 4 4 4)
# cc_param_2=(5 5 5 5 5 5 5 5)
# cc_param_3=(4 4 4 4 4 4 4 4)
# cc_param_4=(1 4 1 4 1 2 3 4)

# for i in {0..7}; do
#     print_cc "${cc_param_1[$i]}" "${cc_param_2[$i]}" "${cc_param_3[$i]}" "${cc_param_4[$i]}" kripke unbounded full
# done



# echo "####################################################################################################"
# echo
# echo "                                         eav"
# echo
# echo "####################################################################################################"

# eav_param_1=(8)
# eav_param_2=(7)

# for i in {0..0}; do
#     print_eav "${eav_param_1[$i]}" "${eav_param_2[$i]}" kripke unbounded full
# done



echo "####################################################################################################"
echo
echo "                                         gos"
echo
echo "####################################################################################################"

gos_param_1=(5)
gos_param_2=(3)
gos_param_3=(9)

for i in {0..0}; do
    print_gos "${gos_param_1[$i]}" "${gos_param_2[$i]}" "${gos_param_3[$i]}" kripke unbounded full
done



# echo "####################################################################################################"
# echo
# echo "                                         gra"
# echo
# echo "####################################################################################################"

# gra_param_1=(5 5 5 5)
# gra_param_2=(5 5 5 5)
# gra_param_3=(1 2 3 4)

# for i in {0..3}; do
#     print_gra "${gra_param_1[$i]}" "${gra_param_2[$i]}" "${gra_param_3[$i]}" kripke unbounded full
# done



echo "####################################################################################################"
echo
echo "                                         sc"
echo
echo "####################################################################################################"

sc_param_1=(6 6)
sc_param_2=(7 8)
sc_param_3=(4 3)

for i in {0..1}; do
    print_sc "${sc_param_1[$i]}" "${sc_param_2[$i]}" "${sc_param_3[$i]}" kripke unbounded full
done



echo "####################################################################################################"
echo
echo "                                         tig"
echo
echo "####################################################################################################"

tig_param_1=(8)
tig_param_2=(5)

for i in {0..0}; do
    print_tig "${tig_param_1[$i]}" "${tig_param_2[$i]}" kripke unbounded full
done
