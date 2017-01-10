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

CPPFLAGS=-I. -I/usr/local/include -O2 -std=c++11 #-lsockets # Flags compilacion

LDFLAGS=-L/usr/local/lib -pthread -lcurl # Flags linkado threads

SOCKETSFLAGS=-lsocket -lnsl # Flags linkado sockets (Solaris SunOS)

.PHONY:all

all: ${TARGET}


# Compilacion de librerias servidor
bin/${SOCKET}.o: src/librerias/Socket/${SOCKET}.h src/librerias/Socket/${SOCKET}.cpp
	${CPP} -c ${CPPFLAGS} src/librerias/Socket/${SOCKET}.cpp -o bin/${SOCKET}.o
#-----------------------------------------------------------
bin/${RESTAURANTE}.o: src/librerias/Restaurante/${RESTAURANTE}.h src/librerias/Restaurante/${RESTAURANTE}.cpp
	${CPP} -c ${CPPFLAGS} src/librerias/Restaurante/${RESTAURANTE}.cpp -o bin/${RESTAURANTE}.o
#-----------------------------------------------------------
bin/${SEMAFORO}.o: src/librerias/Semaphore/${SEMAFORO}.h src/librerias/Semaphore/${SEMAFORO}.cpp
	${CPP} -c ${CPPFLAGS} src/librerias/Semaphore/${SEMAFORO}.cpp -o bin/${SEMAFORO}.o
#-----------------------------------------------------------
bin/${MONUMENTO}.o: src/librerias/Monumento/${MONUMENTO}.h src/librerias/Monumento/${MONUMENTO}.cpp
	${CPP} -c ${CPPFLAGS} src/librerias/Monumento/${MONUMENTO}.cpp -o bin/${MONUMENTO}.o
#-----------------------------------------------------------
bin/${JSONDOWNLOAD}.o: src/librerias/JsonDownload/${JSONDOWNLOAD}.h src/librerias/JsonDownload/${JSONDOWNLOAD}.cpp
	${CPP} -c ${CPPFLAGS} src/librerias/JsonDownload/${JSONDOWNLOAD}.cpp -o bin/${JSONDOWNLOAD}.o
#-----------------------------------------------------------
bin/${DBMONUMENTOSRESTAURANTES}.o: src/servidor/DbMonumentosRestaurantes/${DBMONUMENTOSRESTAURANTES}.h src/servidor/DbMonumentosRestaurantes/${DBMONUMENTOSRESTAURANTES}.cpp
	${CPP} -c ${CPPFLAGS} src/servidor/DbMonumentosRestaurantes/${DBMONUMENTOSRESTAURANTES}.cpp -o bin/${DBMONUMENTOSRESTAURANTES}.o
#-----------------------------------------------------------
bin/${DBSESION}.o: src/servidor/DbSesion/${DBSESION}.h src/servidor/DbSesion/${DBSESION}.cpp
	${CPP} -c ${CPPFLAGS} src/servidor/DbSesion/${DBSESION}.cpp -o bin/${DBSESION}.o
#-----------------------------------------------------------
bin/${GESTORPRECIOS}.o: src/servidor/GestorPrecios/${GESTORPRECIOS}.h src/servidor/GestorPrecios/${GESTORPRECIOS}.cpp
	${CPP} -c ${CPPFLAGS} src/servidor/GestorPrecios/${GESTORPRECIOS}.cpp -o bin/${GESTORPRECIOS}.o
#-----------------------------------------------------------
bin/${UTM2LL}.o: src/librerias/UTM2LL/${UTM2LL}.h src/librerias/UTM2LL/${UTM2LL}.cpp
	${CPP} -c $(CPPFLAGS) src/librerias/UTM2LL/${UTM2LL}.cpp -o bin/${UTM2LL}.o
#-----------------------------------------------------------
bin/${GESTORCONEXIONES}.o: src/servidor/GestorConexiones/${GESTORCONEXIONES}.cpp
	${CPP} -c ${CPPFLAGS} src/servidor/GestorConexiones/${GESTORCONEXIONES}.cpp  -o bin/${GESTORCONEXIONES}.o
#-----------------------------------------------------------

# Compilacion de librerias cliente
bin/${CLIENTEMANUAL}.o: src/cliente/${CLIENTEMANUAL}.cpp
	${CPP} -c $(CPPFLAGS) src/cliente/${CLIENTEMANUAL}.cpp -o bin/${CLIENTEMANUAL}.o
#-----------------------------------------------------------
bin/${CLIENTEAUTOMATICO}.o: src/cliente/${CLIENTEAUTOMATICO}.cpp
	${CPP} -c $(CPPFLAGS) src/cliente/${CLIENTEAUTOMATICO}.cpp -o bin/${CLIENTEAUTOMATICO}.o

#-----------------------------------------------------------
# Linkado
${SERVIDOR}: bin/${GESTORCONEXIONES}.o bin/${DBMONUMENTOSRESTAURANTES}.o bin/${RESTAURANTE}.o bin/${MONUMENTO}.o bin/${JSONDOWNLOAD}.o bin/${GESTORPRECIOS}.o bin/${DBSESION}.o bin/${SOCKET}.o bin/${SEMAFORO}.o bin/${UTM2LL}.o
	${CPP} bin/${GESTORCONEXIONES}.o bin/${DBMONUMENTOSRESTAURANTES}.o bin/${RESTAURANTE}.o bin/${MONUMENTO}.o bin/${JSONDOWNLOAD}.o bin/${GESTORPRECIOS}.o bin/${DBSESION}.o bin/${SOCKET}.o bin/${SEMAFORO}.o bin/${UTM2LL}.o -o  bin/${SERVIDOR} ${LDFLAGS} #${SOCKETSFLAGS} #descomentar para Hendrix
#-----------------------------------------------------------
# Linkado
${CLIENTEMANUAL}: bin/${CLIENTEMANUAL}.o bin/${SOCKET}.o
	${CPP} bin/${CLIENTEMANUAL}.o bin/${SOCKET}.o -o bin/${CLIENTEMANUAL} ${LDFLAGS} #${SOCKETSFLAGS}
#-----------------------------------------------------------
${CLIENTEAUTOMATICO}: bin/${CLIENTEAUTOMATICO}.o bin/${SOCKET}.o
	${CPP} bin/${CLIENTEAUTOMATICO}.o bin/${SOCKET}.o -o bin/${CLIENTEAUTOMATICO} ${LDFLAGS} #${SOCKETSFLAGS}
#-----------------------------------------------------------

# LIMPIEZA

clean:
	$(RM) bin/${GESTORCONEXIONES}.o
	$(RM) bin/${GESTORPRECIOS}.o
	$(RM) bin/${DBSESION}.o
	$(RM) bin/${SOCKET}.o
	$(RM) bin/${DBMONUMENTOSRESTAURANTES}.o
	$(RM) bin/${RESTAURANTE}.o
	$(RM) bin/${MONUMENTO}.o
	$(RM) bin/${JSONDOWNLOAD}.o
	$(RM) bin/${SEMAFORO}.o
	$(RM) bin/${GESTORPRECIOS}.o
	$(RM) bin/${UTM2LL}.o
	$(RM) bin/${CLIENTEMANUAL}.o:
	$(RM) bin/${CLIENTEAUTOMATICO}.o:
	$(RM) bin/${SERVIDOR}
	$(RM) bin/${CLIENTEMANUAL}
	$(RM) bin/${CLIENTEAUTOMATICO}
	$(RM) bin/Clienteauto.o
	$(RM) bin/Clientemanual.o
	$(RM) bin/monumentos.json
	$(RM) bin/restaurantes.json
