#makefile

# Input, Output (eg. plots)
FLD = ./fieldline/fieldline
SUB = ./sublayer
BMP = ./easybmp/EasyBMP
<<<<<<< HEAD
ANA = ./analytical/analytical

all: main.o $(FLD).o $(BMP).o $(ANA).o
	g++ -o main main.o $(FLD).o $(BMP).o $(ANA).o
=======
FOL = ./headers/
HED = $(FOL)sublayer.h $(FOL)meshing.h $(FOL)funcs.h $(FOL)gradient.h \
	$(FOL)timer.h

ANA = ./analytical/analytical

# Algorithms 
FUNFOL = ./functions_main/
FPDM = $(FUNFOL)algFivePointDM
NPDM = $(FUNFOL)algNinePointDM

all: main.o $(FLD).o $(BMP).o $(ANA).o $(FPDM).o $(NPDM).o
	g++ -o main $?
>>>>>>> master

fieldline.o: $(FLD).cpp $(FLD).h funcs.h
	g++ -o $(FLD).o -c $(FLD).cpp

EasyBMP.o: $(BMP).cpp $(BMP).h
	g++ -o $(BMP).o -c $(BMP).cpp

main.o: main.cpp funcs.h timer.h $(SUB).h $(BMP).h meshing.h
	g++ -o main.o -c main.cpp

analytical.o: $(ANA).cpp $(FOL)funcs.h
	g++ -o $(ANA).o -c $(ANA).cpp

<<<<<<< HEAD
# removes all object files and backup files
clean:
	rm -f *.o $(FLD).o $(ANA).o $(BMP).o *~
=======
algFivePointDM.o: $(FPDM).cpp $(FOL)funcs.h
	g++ -o $(FPDM).o -c $(FPDM).cpp

algNinePointDM.o: $(NPDM).cpp $(FOL)funcs.h
	g++ -o $(NPDM).o -c $(NPDM).cpp

errorA:
	g++ -o errorA errorA.cpp

# removes all object files and backup files
clean:
	rm -f *.o $(FLD).o $(ANA).o $(BMP).o $(FPDM).o $(NPDM).o *~

reset:
	rm -f grad.dat grad.eps pot.dat pot.eps field.dat field.eps analytical.dat analytical.eps
>>>>>>> master
