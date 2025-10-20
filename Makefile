# Makefile pour INF3105 / TP2.
# Adaptez ce fichier si nécessaire.

# Choisir l'une des ces trois configurations:
#OPTIONS = -Wall
#OPTIONS = -g -O0 -Wall
OPTIONS = -O2 -Wall

all : tp2

tp2 : tp2.cpp heure.o pointst.o personne.o tableau.h
	g++ ${OPTIONS} -o tp2 tp2.cpp heure.o personne.o pointst.o
	
pointst.o : pointst.cpp pointst.h
	g++ ${OPTIONS} -c -o pointst.o pointst.cpp

heure.o : heure.cpp heure.h
	g++ ${OPTIONS} -c -o heure.o heure.cpp

personne.o : personne.cpp personne.h pointst.h
	g++ ${OPTIONS} -c -o personne.o personne.cpp

clean:
	rm -rf tp2 *~ *.o

