CC=gcc
FLAGS=  -g -Wextra

all : gettftp puttftp


gettftp :      mes_fonctions.o gettftp.o
	$(CC) mes_fonctions.o gettftp.o -o gettftp

puttftp :      mes_fonctions.o puttftp.o
	$(CC) mes_fonctions.o puttftp.o -o puttftp
	rm -f *.o

mes_fonctions.o:   mes_fonctions.c mes_fonctions.h
	$(CC) -c mes_fonctions.c $(FLAGS) -o mes_fonctions.o

gettftp.o:         gettftp.c mes_fonctions.h
	$(CC) -c gettftp.c $(FLAGS) -o gettftp.o

puttftp.o:         puttftp.c mes_fonctions.h
	$(CC) -c puttftp.c $(FLAGS) -o puttftp.o
	
clean : 
	rm -f *.o
	rm -f gettftp
	rm -f puttftp
