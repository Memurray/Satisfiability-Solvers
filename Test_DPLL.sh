#!/bin/bash

for i in CNF_Formulas/*.cnf
do 
	printf "$i ";
	res1=$(date +%s.%N)
	./DPLL < $i;
	res2=$(date +%s.%N)
	dt=$(echo "$res2 - $res1" | bc)
	printf " $dt\n"
done

for i in HARD_CNF_Formulas/*.cnf
do 
	printf "$i ";
	res1=$(date +%s.%N)
	./DPLL < $i;
	res2=$(date +%s.%N)
	dt=$(echo "$res2 - $res1" | bc)
	printf " $dt\n"
done




