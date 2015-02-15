#!/bin/bash

###############################################################
#               --multiiter.sh--                              #
#run this script first                                        #
#(followed by errors.bsh)                                     #
#
###############################################################


for i in {1000..10000..1000}
do

if [ -e ./mat_test${i}.dat]
   then
      rm ./mat_test${i}.dat
      echo "removed previos ./mat_test${i}.dat"
fi

	./main -10 10 0.1 3 $i 2 
	mv mat_test.dat mat_test${i}.dat


done

 exit 0

