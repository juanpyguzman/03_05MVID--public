#include "net/socketutils.h"
#include "net/socketaddress.h"
#include "containers/vector.hpp"
#include <thread>
#include <vector>

enum possibilities { end, piedra, papel, tijera, lagarto, spock };

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

void game(TCPSocketPtr& _connSocket)
{
    int server;
    int client;

    for (;;)
    {
        std::cout << "Esperando jugada desde el cliente" << std::endl;
        _connSocket->receiveFrom(&client, sizeof(int));

        if (client == 0)
        {
            break;
        }

        //Servidor saca su jugada
        server = 1 + rand() % 5;

        //Se transforma el buffer recibido en nuestras posibilidades de juego
        possibilities valClient = static_cast<possibilities>(client);
        possibilities valServer = static_cast<possibilities>(server);

        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        _connSocket->sendTo(&server, sizeof(int));

        std::cout << "Cliente juega: " << enumToString(valClient) << " ------ Servidor juega: " << enumToString(valServer) << std::endl;


    }

    std::cout << "Partida finalizada";

}

int main() {
    SocketUtils::init();
    //std::vector<std::thread> threadsVector;

    {
        TCPSocketPtr socket = SocketUtils::createTCPSocket(SocketUtils::INET);

        SocketAddressPtr address = SocketUtils::createIPv4FromString("127.0.0.1:9090");

        socket->bindTo(*address.get());
        socket->listenTo();

        SocketAddress addressIn;
        TCPSocketPtr connSocket = socket->acceptCon(addressIn);
       // std::thread thread = std::thread(game, connSocket);
       // thread.detach();

        game(connSocket);
    }

    SocketUtils::shutdown();

    return 0;
}