#! /bin/bash
# Script con la finalidad de lanzar tres clientes que funcionan autom�ticamente

echo "IP seleccionada: $1"
echo "Puerto seleccionada: $2"
echo "Lanzando clientes..."

xterm -e ./bin/Clientemanual $1 $2 1 &
xterm -e ./bin/Clientemanual $1 $2 2 &
xterm -e ./bin/Clientemanual $1 $2 3 &
