#include "net/socketutils.h"
#include "net/socketaddress.h"
#include <thread>

enum possibilities { end, piedra, papel, tijera, lagarto, spock};
enum resultados { empate, serverWin, clientWin };

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

void instructions() {
    std::cout << "Piedra, papel, tijera, lagarto, Spock" << std::endl;
    std::cout << "Juego jugador VS ordenador" << std::endl << std::endl;
    std::cout << ".-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-." << std::endl;
    std::cout << "En cada ronda introduce:" << std::endl;
    std::cout << "1. Para jugar Piedra" << std::endl;
    std::cout << "2. Para jugar Papel" << std::endl;
    std::cout << "3. Para jugar Tijera" << std::endl;
    std::cout << "4. Para jugar Lagarto" << std::endl;
    std::cout << "5. Para jugar Spock" << std::endl;
    std::cout << ".-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-." << std::endl;
    std::cout << "Gana el primero en llegar a 11 puntos" << std::endl;
    std::cout << "¡Buena Suerte!" << std::endl;
    std::cout << ".-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-." << std::endl << std::endl << std::endl;   
}

void result(possibilities valServer, possibilities valClient, int& serverPoints, int& clientPoints) {
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

int main() {
    SocketUtils::init();

    {
        instructions();

        TCPSocketPtr socket = SocketUtils::createTCPSocket(SocketUtils::INET);

        SocketAddress addressFrom;
        SocketAddressPtr address = SocketUtils::createIPv4FromString("127.0.0.1:9090");

        socket->connectTo(*address.get());

        int hand;
        int buffer;
        
        int serverPoints = 0;
        int clientPoints = 0;

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

            result(valServer, valClient, serverPoints, clientPoints);

        }

        std::cout << "Partida finalizada";

    }

    SocketUtils::shutdown();
}