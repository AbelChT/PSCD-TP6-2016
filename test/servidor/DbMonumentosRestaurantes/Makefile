#*****************************************************************
# File:   Makefile
# Author: Abel Chils Trabanco
# Date:   enero 2017
# Coms:   Genera sockets, cliente y servidor para la práctica 5
#         Ejecutar como "make"
#         Para compilar en Hendrix (Solaris OS), descomentar
#         "#${SOCKETSFLAGS}"
#*****************************************************************

# ######## #
# COMANDOS #
# ######## #
RM = rm -f # Comando de limpieza

CPP=g++ # Comando de compilacion

# ######################## #
# VARIABLES GLOBALES       #
# ######################## #
MAIN=prueba
MONUMENTO=monumento
RESTAURANTE=restaurante
JSONDOWNLOAD=JsonDownload
JSONCONVERTER=json
DBMONUMENTOSRESTAURANTES=DbMonumentosRestaurantes

# #################### #
# FLAGS DE COMPILACION #
# #################### #

CPPFLAGS=-I. -I/usr/local/include -O2 -std=c++11 # Flags compilacion

LDFLAGS=-lcurl # Flags linkado threads


.PHONY:all

all: ${MAIN}

# SOCKETS
# Compilacion libreria de Sockets
${MAIN}.o: ${MAIN}.cpp
	${CPP} -c ${CPPFLAGS} ${MAIN}.cpp
#-----------------------------------------------------------
${JSONDOWNLOAD}.o: ../../../src/librerias/JsonDownload/${JSONDOWNLOAD}.h ../../../src/librerias/JsonDownload/${JSONDOWNLOAD}.cpp
	${CPP} -c ${CPPFLAGS} ${LDFLAGS} ../../../src/librerias/JsonDownload/${JSONDOWNLOAD}.cpp
#-----------------------------------------------------------
#-----------------------------------------------------------
${DBMONUMENTOSRESTAURANTES}.o: ../../../src/servidor/DbMonumentosRestaurantes/${DBMONUMENTOSRESTAURANTES}.h ../../../src/servidor/DbMonumentosRestaurantes/${DBMONUMENTOSRESTAURANTES}.cpp
	${CPP} -c ${CPPFLAGS} ../../../src/servidor/DbMonumentosRestaurantes/${DBMONUMENTOSRESTAURANTES}.cpp
#-----------------------------------------------------------
# Linkado
${MAIN}: ${MAIN}.o ${JSONDOWNLOAD}.o ${DBMONUMENTOSRESTAURANTES}.o
	${CPP} ${LDFLAGS} ${MAIN}.o ${JSONDOWNLOAD}.o ${DBMONUMENTOSRESTAURANTES}.o -o ${MAIN}
#-----------------------------------------------------------

# LIMPIEZA
clean:
	$(RM) ${MAIN}.o ${MAIN}
	$(RM) ${JSONDOWNLOAD}.o
	$(RM) ${DBMONUMENTOSRESTAURANTES}.o
