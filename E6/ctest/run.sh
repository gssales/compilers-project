#!/bin/bash
i=0
while [[ $i -lt 10 ]]
do
	if [ $i -gt 0 ] && [ $i -lt 4 ];
	then
		printf -v t "ex%i.c" $i
		echo $t
		gcc -S $t
	fi

	printf -v t "test%i.c" $i
	echo $t
	gcc -S $t

	if [[ $i -gt 5 ]];
	then
		printf -v t "test%i-2.c" $i
		echo $t
		gcc -S $t
	fi

	((i++))
done
