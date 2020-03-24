#include "net/socketutils.h"
#include "net/socketaddress.h"
#include "containers/vector.hpp"
#include <thread>
#include <vector>

enum possibilities { end, piedra, papel, tijera, lagarto, spock };
enum resultados { empate, serverWin, clientWin };
std::mutex mutex;

inline const char* enumToString(possibilities val)
{
    switch (val)
    {
    case piedra: return "Piedra";
    case papel: return "Papel";
    case tijera: return "Tijera";
    case lagarto: return "Lagarto";
    case spock: return "Spock";

    }
}

void result(possibilities valServer, possibilities valClient, int &serverPoints, int &clientPoints) {
    resultados resultado;
    
    switch (valServer) {
    case piedra: {
        if (valClient == piedra) {
            resultado = empate;
        }
        else if (valClient == tijera) {
            resultado = serverWin;
        }
        else if (valClient == lagarto) {
            resultado = serverWin;
        }
        else if (valClient == papel) {
            resultado = clientWin;
        }
        else if (valClient == spock) {
            resultado = clientWin;
        }
        break;
    }

    case papel: {
        if (valClient == papel) {
            resultado = empate;
        }
        else if (valClient == piedra) {
            resultado = serverWin;
        }
        else if (valClient == spock) {
            resultado = serverWin;
        }
        else if (valClient == tijera) {
            resultado = clientWin;
        }
        else if (valClient == lagarto) {
            resultado = clientWin;
        }
        break;
    }

    case tijera: {
        if (valClient == tijera) {
            resultado = empate;
        }
        else if (valClient == papel) {
            resultado = serverWin;
        }
        else if (valClient == lagarto) {
            resultado = serverWin;
        }
        else if (valClient == piedra) {
            resultado = clientWin;
        }
        else if (valClient == spock) {
            resultado = clientWin;
        }
        break;
    }

    case lagarto: {
        if (valClient == lagarto) {
            resultado = empate;
        }
        else if (valClient == papel) {
            resultado = serverWin;
        }
        else if (valClient == spock) {
            resultado = serverWin;
        }
        else if (valClient == tijera) {
            resultado = clientWin;
        }
        else if (valClient == piedra) {
            resultado = clientWin;
        }
        break;
    }

    case spock: {
        if (valClient == spock) {
            resultado = empate;
        }
        else if (valClient == piedra) {
            resultado = serverWin;
        }
        else if (valClient == tijera) {
            resultado = serverWin;
        }
        else if (valClient == papel) {
            resultado = clientWin;
        }
        else if (valClient == lagarto) {
            resultado = clientWin;
        }
        break;
    }

    }

    switch (resultado)
    {
    case empate: {
        std::cout << "Empate en esta ronda" << std::endl;
        break;
    }

    case serverWin: {
        ++serverPoints;
        std::cout << "Gana Server esta ronda" << std::endl;
        break;
    }

    case clientWin: {
        ++clientPoints;
        std::cout << "Gana Client esta ronda" << std::endl;
        break;
    }
    }

    std::cout << "Puntuacion: " << "Client -> " << clientPoints << "   ----------- Server -> " << serverPoints << std::endl << std::endl << std::endl;

}

void game(TCPSocketPtr const& _connSocket)
{
    int server;
    int client;

    int serverPoints = 0;
    int clientPoints = 0;


    for (;;)
    {
        std::cout << "Esperando jugada desde el cliente" << std::endl;
        _connSocket->receiveFrom(&client, sizeof(int));

        if (client == 0)
        {
            break;
        }
        if (serverPoints == 11)
        {
            break;
        } 
        else if (clientPoints == 11)
        {
            break;
        }
        
        std::lock_guard<std::mutex> guard(mutex);

        //Servidor saca su jugada
        server = 1 + rand() % 5;

        //Se transforma el buffer recibido en nuestras posibilidades de juego
        possibilities valClient = static_cast<possibilities>(client);
        possibilities valServer = static_cast<possibilities>(server);

        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        _connSocket->sendTo(&server, sizeof(int));

        std::cout << "Cliente juega: " << enumToString(valClient) << " ------ Servidor juega: " << enumToString(valServer) << std::endl;

        result(valServer, valClient, serverPoints, clientPoints);

    }

    std::cout << "Partida finalizada";
    
}

int main() {
    std::vector<std::thread> threads;

    SocketUtils::init();

    {
        TCPSocketPtr socket = SocketUtils::createTCPSocket(SocketUtils::INET);

        SocketAddressPtr address = SocketUtils::createIPv4FromString("127.0.0.1:9090");

        socket->bindTo(*address.get());
        socket->listenTo();

        std::vector<TCPSocketPtr> connSockets;
        
        for (;;) {
            SocketAddress addressIn;
            connSockets.push_back(socket->acceptCon(addressIn));
            threads.push_back(std::thread(game, std::ref(connSockets.back())));
            threads.back().join();
        }

    }

    SocketUtils::shutdown();

    return 0;
}