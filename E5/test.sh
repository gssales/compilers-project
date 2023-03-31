#!/bin/sh
i=0
while [[ $i -le 30 ]]
do
	if [[ $i = 21 ]]; then
		((i++))
		continue
	fi
	printf -v t "%02i\n" $i
	echo ijk$t
	./etapa5 < ./test/ijk$t > "./out/ijk$t.iloc"
	python3 ilocsim.py -t -s -x --stack 1024 --data 500 "./out/ijk$t.iloc" > "./sim/ijk$t.sim"
	((i++))
done

echo ijk1A
./etapa5 < ./test/ijk1A > ./out/ijk1A.iloc
python3 ilocsim.py -t -s -x --stack 1024 --data 500 ./out/ijk1A.iloc > ./sim/ijk1A.sim

echo ijk1B
./etapa5 < ./test/ijk1B > ./out/ijk1B.iloc
python3 ilocsim.py -t -s -x --stack 1024 --data 500 ./out/ijk1B.iloc > ./sim/ijk1B.sim
