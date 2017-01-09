#*****************************************************************
# File:   Makefile
# Author: Abel Chils Trabanco
#         Jorge Aznar López
#         Daniel Fraile Belmonte
# Date:   enero 2017
# Coms:   Ejecutar como "make"
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
UTM2LL=LatLong-UTMconversion

#Modulos servidor
DBMONUMENTOSRESTAURANTES=DbMonumentosRestaurantes
DBSESION=DbSesion
GESTORCONEXIONES=GestorConexiones
GESTORPRECIOS=GestorPrecios
SERVIDOR=Servidor

#Modulos cliente
CLIENTEMANUAL=Clientemanual
CLIENTEAUTOMATICO=Clienteauto

#Target
TARGET=${SERVIDOR} ${CLIENTEMANUAL} ${CLIENTEAUTOMATICO}

# #################### #
# FLAGS DE COMPILACION #
# #################### #

CPPFLAGS=-I. -I/usr/local/include -O2 -std=c++11 -lsockets # Flags compilacion

LDFLAGS=-L/usr/local/lib -pthread -lcurl # Flags linkado threads

SOCKETSFLAGS=-lsocket -lnsl # Flags linkado sockets (Solaris SunOS)

.PHONY:all

all: ${TARGET}


# Compilacion de librerias servidor
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
${UTM2LL}.o: src/librerias/UTM2LL/${UTM2LL}.h src/librerias/UTM2LL/${UTM2LL}.cpp
	${CPP} -c $(CPPFLAGS) src/librerias/UTM2LL/${UTM2LL}.cpp ${LDFLAGS}
#-----------------------------------------------------------
${GESTORCONEXIONES}.o: src/servidor/GestorConexiones/${GESTORCONEXIONES}.cpp
	${CPP} -c ${CPPFLAGS} src/servidor/GestorConexiones/${GESTORCONEXIONES}.cpp
#-----------------------------------------------------------

# Compilacion de librerias cliente
${CLIENTEMANUAL}.o: src/cliente/${CLIENTEMANUAL}.cpp
	${CPP} -c $(CPPFLAGS) src/cliente/${CLIENTEMANUAL}.cpp ${LDFLAGS}
#-----------------------------------------------------------
${CLIENTEAUTOMATICO}.o: src/cliente/${CLIENTEAUTOMATICO}.cpp
	${CPP} -c $(CPPFLAGS) src/cliente/${CLIENTEAUTOMATICO}.cpp ${LDFLAGS}

#-----------------------------------------------------------
# Linkado
${SERVIDOR}: ${GESTORCONEXIONES}.o ${DBMONUMENTOSRESTAURANTES}.o ${RESTAURANTE}.o ${MONUMENTO}.o ${JSONDOWNLOAD}.o ${GESTORPRECIOS}.o ${DBSESION}.o ${SOCKET}.o ${SEMAFORO}.o ${UTM2LL}.o
	${CPP} ${GESTORCONEXIONES}.o ${DBMONUMENTOSRESTAURANTES}.o ${RESTAURANTE}.o ${MONUMENTO}.o ${JSONDOWNLOAD}.o ${GESTORPRECIOS}.o ${DBSESION}.o ${SOCKET}.o ${SEMAFORO}.o ${UTM2LL}.o -o ${SERVIDOR} ${LDFLAGS} ${SOCKETSFLAGS} #descomentar para Hendrix
#-----------------------------------------------------------
# Linkado
${CLIENTEMANUAL}: ${CLIENTEMANUAL}.o ${SOCKET}.o
	${CPP} ${CLIENTEMANUAL}.o ${SOCKET}.o -o ${CLIENTEMANUAL} ${LDFLAGS} ${SOCKETSFLAGS}
#-----------------------------------------------------------
${CLIENTEAUTOMATICO}: ${CLIENTEAUTOMATICO}.o ${SOCKET}.o
	${CPP} ${CLIENTEAUTOMATICO}.o ${SOCKET}.o -o ${CLIENTEAUTOMATICO} ${LDFLAGS} ${SOCKETSFLAGS}
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
	$(RM) ${GESTORPRECIOS}.o
	$(RM) ${UTM2LL}.o
	$(RM) ${CLIENTEMANUAL}.o:
	$(RM) ${CLIENTEAUTOMATICO}.o:
	$(RM) ${SERVIDOR}
	$(RM) ${CLIENTEMANUAL}
	$(RM) ${CLIENTEAUTOMATICO}
