#*****************************************************************
# File:   Makefile
# Author: PSCD-Unizar
# Date:   noviembre 2015
# Coms:   Genera sockets, cliente y servidor para la prÃ¡ctica 5
#         Ejecutar como "make"
#         Para compilar en Hendrix (Solaris OS), descomentar
#         "#${SOCKETSFLAGS}"
#*****************************************************************
# ######## #
# COMANDOS #
# ######## #
RM = rm -f # Comando de limpieza
CC = g++ # Comando de compilacion
# ######################## #
# VARIABLES GLOBALES       #
# Definimos:               #
#   * LibrerÃ­a de sockets  #
#   * Cliente              #
#   * Servidor             #
#   * ServidorMulticliente #
# ######################## #
SOCKET=Socket
CLIENTA=Clienteauto
CLIENTM=Clientemanual
MULTISERVER=ServidorMulticliente

# #################### #
# FLAGS DE COMPILACION #
# #################### #
CPPFLAGS=-I. -O2 -std=c++11 -lsockets # Flags compilacion
LDFLAGS=-pthread # Flags linkado threads
SOCKETSFLAGS=-lsocket -lnsl # Flags linkado sockets (Solaris SunOS)

.PHONY:all

all: ${CLIENTA} ${CLIENTM}

# SOCKETS
# Compilacion libreria de Sockets
${SOCKET}.o: ${SOCKET}.h ${SOCKET}.cpp
	${CC} -c ${CPPFLAGS} ${SOCKET}.cpp
#-----------------------------------------------------------
# CLIENTE AUTOMATICO
# Compilacion
${CLIENTA}.o: ${CLIENTA}.cpp
	${CC} -c ${CPPFLAGS} ${CLIENTA}.cpp

# Linkado
${CLIENTA}: ${SOCKET}.o ${CLIENTA}.o
	${CC} ${LDFLAGS} ${SOCKET}.o ${CLIENTA}.o -o ${CLIENTA} ${SOCKETSFLAGS}
#-----------------------------------------------------------
# CLIENTE MANUAL
# Compilacion
${CLIENTM}.o: ${CLIENTM}.cpp
	${CC} -c ${CPPFLAGS} ${CLIENTM}.cpp

# Linkado
${CLIENTM}: ${SOCKET}.o ${CLIENTM}.o
	${CC} ${LDFLAGS} ${SOCKET}.o ${CLIENTM}.o -o ${CLIENTM} ${SOCKETSFLAGS}
#-----------------------------------------------------------
# LIMPIEZA

clean:
	$(RM) ${CLIENTA}.o
	$(RM) ${CLIENTM}.o
	$(RM) ${SOCKET}.o

