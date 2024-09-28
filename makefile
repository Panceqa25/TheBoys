parametrosCompilacao=-Wall -lm -g#-Wshadow
nomePrograma=theboys

all: $(nomePrograma)

$(nomePrograma): theboys.o filaEncadeada.o conjunto.o lef.o
	gcc -o $(nomePrograma) theboys.o filaEncadeada.o conjunto.o lef.o $(parametrosCompilacao)

theboys.o:  theboys.c
	gcc -c theboys.c $(parametrosCompilacao)

filaEncadeada.o: filaEncadeada.h filaEncadeada.c
	gcc -c filaEncadeada.c $(parametrosCompilacao)

conjunto.o: conjunto.h conjunto.c
	gcc -c conjunto.c $(parametrosCompilacao)

lef.o: lef.h lef.c
	gcc -c lef.c $(parametrosCompilacao)

clean:
	rm -f *.o *.gch $(nomePrograma)
