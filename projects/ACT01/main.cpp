/* EJ01

El vector de la librería standard de C++ (std::vector) no es thread safe,
implementar una clase templatizada propia sobre std::vector que sea thread safe,
que implemente al menos los siguientes métodos análogos a los del propio std::vector
(donde T es el tipo del template que tiene que albergar nuestro vector).
La declaración de los métodos a implementar están en el fichero include / containers / vector.hpp.

*/

#include "containers/vector.hpp"
#include <cstdint>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>


Vector<uint32_t> vec;
std::vector<uint32_t> unsafevec;

void bubbleSort(Vector<uint32_t> vector, uint32_t numThreads, uint32_t thread) {
	uint32_t temp;
	for (uint32_t i = 1 + thread * vector.size() / numThreads; i < (1 + thread) * vector.size() / numThreads; i++)
		for (uint32_t j = 0 + thread * vector.size() / numThreads; j < (1 + thread) * vector.size() / numThreads - 1; j++)
			if (vector[j] > vector[j + 1]) {
				temp = vector[j];
				vector[j] = vector[j + 1];
				vector[j + 1] = temp;
			}
}


int main() {
	vec.resize(100);
	for (int i = 0; i < vec.size(); i++)
	{
		vec[i] = rand() % (1000);
		std::cout << vec[i] << std::endl;
	}


	std::cout << std::endl << "Ordenamos" << std::endl << std::endl;


	//std::vector<std::thread> threads;
	Vector<std::thread> safeThreads;
	uint32_t numThreads = 4;
	Vector<uint32_t> temp;

	for (uint32_t n = 0; n < 4; n++)
	{
		safeThreads[n] = std::thread(bubbleSort, &vec, numThreads, n);
	}

	for (auto& t : safeThreads) {
		t.join();
	}

	for (int i = 0; i < vec.size(); i++)
	{
		std::cout << vec[i] << std::endl;
	}

}