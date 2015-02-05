#makefile

FLD = ./fieldline/fieldline
SUB = ./sublayer/sublayer
BMP = ./easybmp/EasyBMP

all: main.o $(FLD).o EasyBMP.o
	g++ -o main main.o $(FLD).o $(BMP).o

fieldline.o: $(FLD).cpp $(FLD).h funcs.h
	g++ -o $(FLD).o -c $(FLD).cpp

EasyBMP.o: $(BMP).cpp $(BMP).h
	g++ -o $(BMP).o -c $(BMP).cpp

main.o: main.cpp funcs.h timer.h $(SUB).h $(BMP).h
	g++ -o main.o -c main.cpp

# removes all object files and backup files
clean:
	rm -f *.o $(FLD).o *~
