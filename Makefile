#*****************************************************************
# File:   Makefile
# Author: Abel Chils Trabanco
# Date:   enero 2017
# Coms:   Genera cliente y servidor para el apartado 3 de la práctica 5
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

#Librerias
SOCKET=Socket
RESTAURANTE=Restaurante
MONUMENTO=Monumento
JSONDOWNLOAD=JsonDownload
SEMAFORO=Semaphore

#Modulos servidor
DBMONUMENTOSRESTAURANTES=DbMonumentosRestaurantes
DBSESION=DbSesion
GESTORCONEXIONES=GestorConexiones
GESTORPRECIOS=GestorPrecios

#Modulos cliente
CLIENTEMANUAL=ClienteManual

# #################### #
# FLAGS DE COMPILACION #
# #################### #

CPPFLAGS=-I. -I/usr/local/include -O2 -std=c++11 -lsockets # Flags compilacion

LDFLAGS=-L/usr/local/lib -pthread -lcurl # Flags linkado threads

SOCKETSFLAGS=-lsocket -lnsl # Flags linkado sockets (Solaris SunOS)

.PHONY:all

all: ${GESTORCONEXIONES} ${CLIENTEMANUAL}

# SOCKETS
# Compilacion libreria de Sockets
${SOCKET}.o: src/librerias/Socket/${SOCKET}.h src/librerias/Socket/${SOCKET}.cpp
	${CPP} -c ${CPPFLAGS} src/librerias/Socket/${SOCKET}.cpp
#-----------------------------------------------------------
${RESTAURANTE}.o: src/librerias/Restaurante/${RESTAURANTE}.h src/librerias/Restaurante/${RESTAURANTE}.cpp
	${CPP} -c ${CPPFLAGS} src/librerias/Restaurante/${RESTAURANTE}.cpp
#-----------------------------------------------------------
${SEMAFORO}.o: src/librerias/Semaphore/${SEMAFORO}.h src/librerias/Semaphore/${SEMAFORO}.cpp
	${CPP} -c ${CPPFLAGS} src/librerias/Semaphore/${SEMAFORO}.cpp
#-----------------------------------------------------------
${MONUMENTO}.o: src/librerias/Monumento/${MONUMENTO}.h src/librerias/Monumento/${MONUMENTO}.cpp
	${CPP} -c ${CPPFLAGS} src/librerias/Monumento/${MONUMENTO}.cpp
#-----------------------------------------------------------
${JSONDOWNLOAD}.o: src/librerias/JsonDownload/${JSONDOWNLOAD}.h src/librerias/JsonDownload/${JSONDOWNLOAD}.cpp
	${CPP} -c ${CPPFLAGS} src/librerias/JsonDownload/${JSONDOWNLOAD}.cpp
#-----------------------------------------------------------
${DBMONUMENTOSRESTAURANTES}.o: src/servidor/DbMonumentosRestaurantes/${DBMONUMENTOSRESTAURANTES}.h src/servidor/DbMonumentosRestaurantes/${DBMONUMENTOSRESTAURANTES}.cpp
	${CPP} -c ${CPPFLAGS} src/servidor/DbMonumentosRestaurantes/${DBMONUMENTOSRESTAURANTES}.cpp
#-----------------------------------------------------------
${DBSESION}.o: src/servidor/DbSesion/${DBSESION}.h src/servidor/DbSesion/${DBSESION}.cpp
	${CPP} -c ${CPPFLAGS} src/servidor/DbSesion/${DBSESION}.cpp
#-----------------------------------------------------------
${GESTORPRECIOS}.o: src/servidor/GestorPrecios/${GESTORPRECIOS}.h src/servidor/GestorPrecios/${GESTORPRECIOS}.cpp
	${CPP} -c ${CPPFLAGS} src/servidor/GestorPrecios/${GESTORPRECIOS}.cpp
#-----------------------------------------------------------
# Compilacion Servidor
${GESTORCONEXIONES}.o: src/servidor/GestorConexiones/${GESTORCONEXIONES}.cpp
	${CPP} -c ${CPPFLAGS} src/servidor/GestorConexiones/${GESTORCONEXIONES}.cpp
#-----------------------------------------------------------
# Compilacion ClienteManual
${CLIENTEMANUAL}.o: src/cliente/${CLIENTEMANUAL}.cpp
	${CPP} -c ${CPPFLAGS} src/cliente/${CLIENTEMANUAL}.cpp
#-----------------------------------------------------------
# Linkado Servidor
${GESTORCONEXIONES}: ${GESTORCONEXIONES}.o ${DBMONUMENTOSRESTAURANTES}.o ${RESTAURANTE}.o ${MONUMENTO}.o ${JSONDOWNLOAD}.o ${GESTORPRECIOS}.o ${DBSESION}.o ${SOCKET}.o ${SEMAFORO}.o
	${CPP} ${GESTORCONEXIONES}.o ${DBMONUMENTOSRESTAURANTES}.o ${RESTAURANTE}.o ${MONUMENTO}.o ${JSONDOWNLOAD}.o ${GESTORPRECIOS}.o ${DBSESION}.o ${SOCKET}.o ${SEMAFORO}.o -o Servidor ${LDFLAGS}#${SOCKETSFLAGS} #descomentar para Hendrix
#-----------------------------------------------------------
# Linkado ClienteManual
${CLIENTEMANUAL}: ${CLIENTEMANUAL}.o ${SOCKET}.o
	${CPP} ${CLIENTEMANUAL}.o ${SOCKET}.o -o ${CLIENTEMANUAL} ${LDFLAGS}#${SOCKETSFLAGS} #descomentar para Hendrix
#-----------------------------------------------------------

# LIMPIEZA

clean:
	$(RM) ${GESTORCONEXIONES}.o
	$(RM) ${GESTORPRECIOS}.o
	$(RM) ${DBSESION}.o
	$(RM) ${SOCKET}.o
	$(RM) ${DBMONUMENTOSRESTAURANTES}.o
	$(RM) ${RESTAURANTE}.o
	$(RM) ${MONUMENTO}.o
	$(RM) ${JSONDOWNLOAD}.o
	$(RM) ${SEMAFORO}.o
	$(RM) ${GESTORCONEXIONES}
	$(RM) ${CLIENTEMANUAL}
