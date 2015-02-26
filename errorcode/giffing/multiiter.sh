#!/bin/bash

###############################################################
#               --multiiter.sh--                              #
#run this script first                                        #
#(followed by errors.bsh)                                     #
#
###############################################################



for i in {1000..30000..500} ## {x1..x2..x3} iterates from x1 to x2 in steps of x3
do

if [ -e ./pot${i}.dat ]
   then
      rm ./pot${i}.dat
      echo "removed previos ./pot${i}.dat"
fi

	./main -10 10 0.2 3 $i 2 
	mv pot.dat ./pot${i}.dat


done

 exit 0

