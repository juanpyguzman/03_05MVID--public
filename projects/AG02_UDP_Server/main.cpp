#include "net/socketutils.h"
#include "net/socketaddress.h"

int main() {
  SocketUtils::init();

  {
    UDPSocketPtr socket = SocketUtils::createUDPSocket(SocketUtils::INET);

    SocketAddressPtr address = SocketUtils::createIPv4FromString("127.0.0.1:9090");

    socket->bindTo(*address.get());

    constexpr size_t bufferLength = 255;
    char buffer[bufferLength];

    SocketAddress addressFrom;
    int bytesReceived = socket->receiveFrom(buffer, bufferLength * sizeof(char), addressFrom);

    std::cout << bytesReceived << " bytes Received, Message = " << buffer << std::endl;

    socket->sendTo("Adios", 6 * sizeof(char), addressFrom);
  }

  SocketUtils::shutdown();
}