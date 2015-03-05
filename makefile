#makefile

# Input, Output (eg. plots)
FLD = ./fieldline/fieldline
BMP = ./easybmp/EasyBMP
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

full: 	main5.o main9.o $(FLD).o $(BMP).o $(ANA).o $(FPDM).o $(NPDM).o
	g++ -o main5 main5.o $(FLD).o $(BMP).o $(ANA).o $(FPDM).o $(NPDM).o
	g++ -o main9 main9.o $(FLD).o $(BMP).o $(ANA).o $(FPDM).o $(NPDM).o

fieldline.o: $(FLD).cpp $(FLD).h funcs.h
	g++ -o $(FLD).o -c $(FLD).cpp

EasyBMP.o: $(BMP).cpp $(BMP).h
	g++ -o $(BMP).o -c $(BMP).cpp

main.o: main.cpp $(BMP).h $(HED)
	g++ -o main.o -c main.cpp

main5.o: main5.cpp $(BMP).h $(HED)
	 g++ -o main5.o -c main5.cpp

main9.o: main9.cpp $(BMP).h $(HED)
	 g++ -o main9.o -c main9.cpp

analytical.o: $(ANA).cpp $(FOL)funcs.h
	g++ -o $(ANA).o -c $(ANA).cpp

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
	rm -f grad.dat grad.eps pot.dat pot.eps field.dat field.eps analytical.dat analytical.eps stats.dat
