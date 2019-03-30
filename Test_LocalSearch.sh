#!/bin/bash

for i in CNF_Formulas/*.cnf
do 
	printf "$i ";
	for j in {1..10}
	do
		res1=$(date +%s.%N)
		./LocalSearch < $i;
		res2=$(date +%s.%N)
		dt=$(echo "$res2 - $res1" | bc)
		printf " $dt "
	done
	printf "\n"
done

for i in HARD_CNF_Formulas/*.cnf
do 
	printf "$i ";
	for j in {1..10}
	do
		res1=$(date +%s.%N)
		./LocalSearch < $i;
		res2=$(date +%s.%N)
		dt=$(echo "$res2 - $res1" | bc)
		printf " $dt "
	done
	printf "\n"
done



