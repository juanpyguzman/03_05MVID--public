# Actividad 02 - MultiPlayer, Rock-Paper-Scissors-Lizard-Spock

## Fecha Entrega, Conv1 = 22/03/20, Conv2 = 24/04/20

## Value = 25%

Implementar el juego Piedra, Papel, Tijeras, Lagarto, Spock para dos jugadores a través de red. Se desarrollarán dos aplicaciones de consola, una tomará el rol del servidor y la otra de cliente. El servidor deberá ser capaz de jugar varias partidas a la vez con varios clientes, guardando el estado de cada partida de manera independiente. Un cliente solo puede jugar una partida simultanea contra el servidor. La IA de ambos procesos será aleatoria. La simulación se ejecuta en el servidor. Una partida se juega al mejor de 20 tiradas entre el servidor y un cliente, es decir, el primero de los dos que llegue a 11 puntos se considera ganador de esa partida.

La elección tanto del protocolo de red como de comunicaciones es libre. Se adjuntará una pequeña memoria justificando la elección y un pequeño cronograma con las comunicaciones entre cliente y servidor (al estilo de los vistos en clase).
