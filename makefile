#makefile

FLD = ./fieldline/fieldline
SUB = ./sublayer/sublayer

all: main.o $(FLD).o
	g++ -o main main.o $(FLD).o

fieldline.o: $(FLD).cpp $(FLD).h funcs.h
	g++ -o $(FLD).o -c $(FLD).cpp

main.o: main.cpp funcs.h timer.h $(SUB).h
	g++ -o main.o -c main.cpp

analytical: analytical.o
	g++ -o analytical analytical.o

analytical.o: analytical.cpp
	g++ -o analytical.o -c analytical.cpp

# removes all object files and backup files
clean:
	rm *.o $(FLD).o *~
