#include "net/socketutils.h"
#include "net/socketaddress.h"
#include <thread>

int main() {
  SocketUtils::init();

  {
    TCPSocketPtr socket = SocketUtils::createTCPSocket(SocketUtils::INET);

    SocketAddressPtr address = SocketUtils::createIPv4FromString("127.0.0.1:9090");

    socket->connectTo(*address.get());

    constexpr size_t bufferLength = 255;
    char buffer[bufferLength];

    for (uint8_t i = 1; i < 10; ++i) {
      sprintf(buffer, "%d\0", i);
      socket->sendTo(buffer, std::string(buffer).size() + 1);

      socket->receiveFrom(buffer, bufferLength);
      std::cout << "Received = " << buffer << std::endl;

      std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    socket->sendTo("exit", 5 * sizeof(char));
  }

  SocketUtils::shutdown();
}
