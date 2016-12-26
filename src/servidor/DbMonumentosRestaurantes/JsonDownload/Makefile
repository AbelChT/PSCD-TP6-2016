#*****************************************************************
# File:   Makefile
# Author: PSCD-Unizar
# Date:   noviembre 2015
# Coms:   Ejemplo de uso de "CImage" y "curl" para el trabajo obligatorio
#         Compilar como "make"
#*****************************************************************

RM = rm -rf
CPP=g++

# ####### ###### # 
#
MAIN=main
target=${MAIN}
imageDownloader=ImageDownloader

# ######
#
CPPFLAGS=-I. -I/usr/local/include -O2 -std=c++11
LDFLAGS=-L/usr/X11R6/lib -L/usr/local/lib -lm -pthread -lcurl -lX11 

.PHONY:all

all: ${target}

${MAIN}: ${MAIN}.o ${imageDownloader}.o
	${CPP} ${imageDownloader}.o ${MAIN}.o -o ${MAIN} ${LDFLAGS}

${MAIN}.o: ${MAIN}.cpp 
	${CPP} -c ${CPPFLAGS} ${MAIN}.cpp

${imageDownloader}.o: ${imageDownloader}.h ${imageDownloader}.cpp
	${CPP} -c ${CPPFLAGS} ${imageDownloader}.cpp

clean:
	$(RM) *.o
	$(RM) ${target}

echo:
	echo ${target}
	echo -n "cppflags: " ${CPPFLAGS}
	echo -n "ldflags: " ${LDFLAGS}
