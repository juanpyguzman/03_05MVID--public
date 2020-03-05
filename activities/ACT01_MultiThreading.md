# Actividad 01 - MultiThreading

## Fecha Entrega, Conv1 = 15/03/20, Conv2 = 24/04/20

## Value = 10%

## EJ01

El vector de la librería standard de C++ (std::vector) no es thread safe, implementar una clase templatizada propia sobre std::vector que sea thread safe, que implemente al menos los siguientes métodos análogos a los del propio std::vector (donde T es el tipo del template que tiene que albergar nuestro vector). La declaración de los métodos a implementar están en el fichero include/containers/vector.hpp.

## EJ02

Implementar un caso de uso similar al visto en la Actividad Guiada donde se haga uso de un vector para provocar una condición de carrera, primero se prueba con std::vector y luego con el nuevo vector se debe solucionar con éxito.