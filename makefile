# makefile de base pour compiler un fichier main.c en main.o



# nom du fichier qui implémente la loi du groupe
groupe = c2c4

all:	main.o set.o $(groupe).o
	gcc -o main main.o set.o $(groupe).o

main.o:	main.c set.h loi.h
	gcc -c -Wall -Ofast main.c

set.o: set.c set.h
	gcc -c -Wall -Ofast set.c

$(groupe).o: $(groupe).c loi.h
	gcc -c -Wall -Ofast $(groupe).c

clean:
	rm -f main *.o

run: main
	./main

