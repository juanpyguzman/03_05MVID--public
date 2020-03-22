#include "net/socketutils.h"
#include "net/socketaddress.h"
#include <thread>

enum possibilities { end, piedra, papel, tijera, lagarto, spock};

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

int main() {
    SocketUtils::init();

    {
        TCPSocketPtr socket = SocketUtils::createTCPSocket(SocketUtils::INET);

        SocketAddress addressFrom;
        SocketAddressPtr address = SocketUtils::createIPv4FromString("127.0.0.1:9090");

        socket->connectTo(*address.get());

        int hand;
        int buffer;
        bool end = false;

        for (int i = 0; i < 5; ++i)
        {
            //Cliente saca su jugada
            //int hand = rand() % 5;
            std::cout << "Introduzca su jugada del 1 al 5: ";
            std::cin >> hand;

            socket->sendTo(&hand, sizeof(int));

            if (hand == 0)
            {
                break;
            }

            socket->receiveFrom(&buffer, sizeof(int));

            //Se transforma el buffer recibido en nuestras posibilidades de juego
            possibilities valServer = static_cast<possibilities>(buffer);
            possibilities valClient = static_cast<possibilities>(hand);

            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            std::cout << "Cliente juega: " << enumToString(valClient) << " ------ Servidor juega: " << enumToString(valServer) << std::endl;
            if (i == 4) {
                hand = 0;
                socket->sendTo(&hand, sizeof(int));
            }

        }

        std::cout << "Partida finalizada";

    }

    SocketUtils::shutdown();
}