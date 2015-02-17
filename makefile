#makefile

FLD = ./fieldline/fieldline
BMP = ./easybmp/EasyBMP
ANA = ./analytical/analytical
FOL = ./headers/
HED = $(FOL)sublayer.h $(FOL)meshing.h $(FOL)funcs.h $(FOL)gradient.h \
	$(FOL)timer.h

all: main.o $(FLD).o $(BMP).o $(ANA).o
	g++ -o main main.o $(FLD).o $(BMP).o $(ANA).o

fieldline.o: $(FLD).cpp $(FLD).h funcs.h
	g++ -o $(FLD).o -c $(FLD).cpp

EasyBMP.o: $(BMP).cpp $(BMP).h
	g++ -o $(BMP).o -c $(BMP).cpp

main.o: main.cpp $(BMP).h $(HED)
	g++ -o main.o -c main.cpp

analytical.o: $(ANA).cpp funcs.h
	g++ -o $(ANA).o -c $(ANA).cpp

# removes all object files and backup files
clean:
	rm -f *.o $(FLD).o $(ANA).o $(BMP).o *~

reset:
	rm -f grad.dat grad.eps pot.dat pot.eps field.dat field.eps