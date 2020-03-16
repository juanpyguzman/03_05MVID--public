/*	
	EJ01
	El vector de la librería standard de C++ (std::vector) no es thread safe,
	implementar una clase templatizada propia sobre std::vector que sea thread safe,
	que implemente al menos los siguientes métodos análogos a los del propio std::vector
	(donde T es el tipo del template que tiene que albergar nuestro vector).
	La declaración de los métodos a implementar están en el fichero include / containers / vector.hpp.

	EJ02
	Implementar un caso de uso similar al visto en la Actividad Guiada donde se haga uso de un vector
	para provocar una condición de carrera, primero se prueba con std::vector
	y luego con el nuevo vector se debe solucionar con éxito.

*/

#include "containers/vector.hpp"
#include <cstdint>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

Vector<uint32_t> safeVector;
std::vector<uint32_t> unsafeVector;

static void increaseElement(uint32_t size, bool isSafe) {

	if (isSafe)
	{
		for (uint32_t j = 0; j < size; ++j){
			safeVector.push_back(j);
		}
	}

	else
	{
		for (uint32_t j = 0; j < size; ++j) {
			unsafeVector.push_back(j);
		}
	}
}


int main() {
	std::cout << " Start" << std::endl;

	bool isSafe = true;
	
	for (uint32_t i = 0; i < 100; ++i) {
		std::vector<std::thread> threads;

		for (uint32_t i = 0; i < 30; ++i)
		{
			threads.push_back(std::thread(increaseElement, 1000, isSafe));
		}

		for (auto& t : threads) {
			t.join();
		}
	

		if (isSafe)
		{
			if (safeVector.size() != 30000) {
				std::cout << "Error in simulation " << i << ", Size of vector:  " << safeVector.size() << std::endl;
			}
			safeVector.clear();
		}

		else
		{
			if (unsafeVector.size() != 30000) {
				std::cout << "Error in simulation " << i << ", Size of vector:  " << unsafeVector.size() << std::endl;
			}
			unsafeVector.clear();
		}
	}

	std::cout << " End" << std::endl;

	return 0;
}