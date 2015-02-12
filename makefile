#makefile

FLD = ./fieldline/fieldline
SUB = ./sublayer
BMP = ./easybmp/EasyBMP
ANA = ./analytical/analytical

all: main.o $(FLD).o $(BMP).o $(ANA).o
	g++ -o main main.o $(FLD).o $(BMP).o $(ANA).o

fieldline.o: $(FLD).cpp $(FLD).h funcs.h
	g++ -o $(FLD).o -c $(FLD).cpp

EasyBMP.o: $(BMP).cpp $(BMP).h
	g++ -o $(BMP).o -c $(BMP).cpp

main.o: main.cpp funcs.h timer.h $(SUB).h $(BMP).h meshing.h
	g++ -o main.o -c main.cpp

analytical.o: $(ANA).cpp funcs.h
	g++ -o $(ANA).o -c $(ANA).cpp

# removes all object files and backup files
clean:
	rm -f *.o $(FLD).o $(ANA).o $(BMP).o *~
