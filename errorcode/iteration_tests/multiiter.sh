#!/bin/bash



for i in {1000..10000..1000}
do
	rm ./mat_test${i}.dat
	./main -10 10 0.1 3 $i 2 
	mv mat_test.dat mat_test${i}.dat


done

 exit 0

