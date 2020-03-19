# Actividad 03 - Pathfinding

## Fecha Entrega, Conv1 = 05/04/20, Conv2 = 24/04/20

## Value = 30%

Implementar el algoritmo de A* tal como se ha visto en clase. Dado un mapa de 1024x1024 pixels se pide calcular un camino, seleccionando con el ratón en dos puntos cualquiera del mapa (se tomará el primero como origen y el segundo como destino). El camino deberá ser el más corto posible. Se deberá visualizar el camino calculado, y hacer que un agente lo recorra. Se debe medir el tiempo máximo que tarda el algoritmo en calcular el camino más largo (de extremo a extremo del mapa). Se puede usar cualquier técnica de optimización para mejorar el tiempo de cálculo.

Se adjuntan varias imagenes en formato bmp:
- mapa.bmp - representación gráfica del nivel de juego, se debe visualizar en pantalla en todo momento, 1024 x 1024 pixels.
- costes.bmp - mapa de costes del nivel, 128 x 128 pixels, cada pixel del mapa de costes equivale a 8 del mapa gráfico, color blanco es suelo pisable, color negro es suelo no pisable.
- guard.bmp, slave.bmp, soldier.bmp - representación gráfica de los agentes.

Para dibujar el camino sobre el mapa una vez calculado usar la función SDL_RenderFillRect, y para leer el mapa de costes se puede usar la clase Map.