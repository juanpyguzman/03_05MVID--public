# Actividad Opcional 04 - IA, Final Castle

## Fecha Entrega, Conv1 = 12/04/20, Conv2 = 24/04/20

## Value = 35%

Desarrollar un mini juego, consiste en liberar a los prisioneros de una fortaleza.

Existe un grupo de guardias cuyo objetivo es vigilarlos y evitar que escapen. Los guardias, para mantener ocupados a los prisioneros, les obligan a realizar trabajos forzados. Concretamente, deben transportar cajas de un lado a otro de la fortaleza. La mitad de los prisioneros están desempeñando esas tareas, mientras la otra mitad está en un patio cercano, paseando y esperando su turno para relevar al otro grupo de prisioneros. Un grupo de soldados, aliados de los prisioneros, debe rodear la fortaleza y abrir las puertas para que los prisioneros escapen. En ningún momento del juego se produce combate, es decir, no hay armas, ni combate de ningún tipo, ni siquiera cuerpo a cuerpo.

Para la realización del ejercicio se adjuntan dos mapas, uno visual de tamaño 1024 x 1024, y otro de costes de tamaño 128 x 128, que debe usarse para el cálculo de caminos. Las posiciones iniciales de los soldados al comienzo del juego serán las dos bases grises de la esquina inferior derecha. Solo es posible entrar o salir de la fortaleza por medio de las puertas (en rojo en el mapa), y no están incluidas en el mapa de costes. Una puerta puede ser abierta por un soldado cuando pasa a una distancia menor de la determinada por la variable MinDistanceToDoor. Un guardia la puede cerrar si está a una distancia menor que MinDistanceToDoor y no hay ningún soldado dentro de ese radio.

## Interior de la fortaleza
- Zona verde, es la zona donde la mitad de los prisioneros descansan, solo se entra en ella para descansar y solo se sale de ella para trabajar o huir. Mientras descansan los prisioneros vagabundean por esa zona.
- Zona amarilla, es la zona en la que los prisioneros trabajan, transportan cajas desde cualquiera de los tres cuadrados de color violeta a la zona ovalada violeta del otro extremo. Los prisioneros no deben salir de ella y los guardas no deben entrar en ella (excepto si se está en estado de Alarma).

## Exterior de la fortaleza
- Rio, zona intransitable, los agentes no pueden pisarla, la única forma de cruzarlos es través de los puentes.
- Paredes de la fortaleza, zona intransitable, los agentes solo pueden entrar y salir de la fortaleza a través de las puertas (en rojo).

## Tipos de agentes
- Guardas, pasean por los alrededores de la zona de trabajo y también pueden hacerlo por las estancias de la fortaleza, pero no hacen un recorrido cíclico. Si sospechan de algo (ven un soldado o una puerta abierta), se acercan a comprobar. Si lo confirman, dan la alarma.
- Prisioneros, si están trabajando, cargan mercancía en una zona determinada y la transportan a la otra zona. A continuación, vuelven a la zona de carga y repiten el proceso, de manera cíclica. Cuando van cargados, y debido al peso, su velocidad se reduce a la mitad. Cuando se da la situación de Alarma, dejan las cajas que transportan y comienzan a buscar una puerta abierta para huir. Si la encuentran, intentan salir por ella y ya no vuelven a entrar a la fortaleza, huyendo a una base de los soldados. Si no están trabajando, esperan paseando en el patio anexo a la zona de trabajo, los que pasean también intentan escapar en el estado de Alarma.
- Soldados, situados fuera de la fortaleza, deben acercarse a ella y abrir sus puertas para que escapen los prisioneros

## Estado de Alarma.
En situaciones normales, los agentes están desarrollando sus tareas, y no están en estado de Alarma. En caso de que los guardias perciban algo que se sale de lo normal, pasan a Sospecha. Si se verifican sus sospechas, los guardias pasan a estado de Alarma. En este estado, los guardias persiguen a aquellos prisioneros y/o soldados que tengan más cerca, y los persiguen constantemente, si alcanzan a un prisionero, este automáticamente se va a la zona de descanso y se queda encerrado allí hasta que el estado de Alarma acaba. Un guardia pasa a Sospecha cuando observa una puerta abierta o bien un soldado a una distancia entre las variables MaxDistanceToAlarm y MinDistanceToAlarm. Un guardia pasa a Alarma si la distancia es menor a MinDistanceToAlarm. Un guardia puede pasar de Normal a Alarma sin pasar por Sospecha. Una vez un agente pasa a Alarma, todos los agentes han de tener conocimiento de que se está en Alarma. Si los guardias no ven soldados o puertas abiertas durante un periodo mayor que TimeInAlarm, se pasa automáticamente a una situación normal. Al volver a la situación normal, los prisioneros que aún estén dentro de la fortaleza volverán a su rutina habitual.

El número mínimo de agentes de cada tipo en el juego es de 10. En distancias cortas, se deben usar los algoritmos de movimiento vistos en clase, combinando diversos tipos de movimiento por un mismo agente en función de su posición y el estado de la partida. El algoritmo para calcular el recorrido de distancias largas debe ser A*. La toma de decisiones debe ser mediante un conjunto de máquinas de estado. El movimiento debe ser independiente del razonamiento. El juego ha de funcionar al menos a 25 fps, con fluidez y sin parones. Se puede usar el esqueleto que se facilitó para prácticas anteriores.

Se adjuntan varias imágenes en formato bmp:
- mapa.bmp - representación gráfica del nivel de juego, se debe visualizar en pantalla en todo momento, 1024 x 1024 pixels.
- costes.bmp - mapa de costes del nivel, 128 x 128 pixels, cada pixel del mapa de costes equivale a 8 del mapa gráfico, color blanco es suelo transitable, color negro es suelo no pisable.
- guard.bmp, slave.bmp, soldier.bmp - representación gráfica de los agentes.

## Criterios de mejora:
- Uso de A* a varios niveles de diferente granularidad
- Uso de grafos de adyacencia para que los guardias recorran todas las estancias de la fortaleza en su labor de vigilancia
- Uso de un pathfinder común a todos los agentes, estos no calculan el camino, se lo piden al pathfinder. El pathfinder debe optimizar su gestión almacenando caminos (o parciales de caminos), se puede apoyar en grafos de adyacencia.
- Precalcular ciertos caminos
- Uso de comunicación entre agentes mediante mensajes
- Crear agentes gestores para cada grupo de agentes que gestionen el comportamiento del grupo