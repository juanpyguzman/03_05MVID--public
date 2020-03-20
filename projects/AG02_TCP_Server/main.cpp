#include "net/socketutils.h"
#include "net/socketaddress.h"
#include <string>
#include <thread>

int main() {
  SocketUtils::init();

  {
    TCPSocketPtr socket = SocketUtils::createTCPSocket(SocketUtils::INET);

    SocketAddressPtr address = SocketUtils::createIPv4FromString("127.0.0.1:9090");

    socket->bindTo(*address.get());
    socket->listenTo();

    SocketAddress addressIn;
    TCPSocketPtr connSocket = socket->acceptCon(addressIn);

    constexpr size_t bufferLength = 255;
    char buffer[bufferLength];

    for (;;) {
      connSocket->receiveFrom(buffer, bufferLength);
      std::cout << "Received = " << buffer << std::endl;

      if (std::string(buffer) == "exit") {
        std::cout << "Exiting = " << buffer << std::endl;
        break;
      }

      std::this_thread::sleep_for(std::chrono::milliseconds(500));

      int n = std::stoi(buffer);
      sprintf(buffer, "%d\0", n * n);
      connSocket->sendTo(buffer, std::string(buffer).size() + 1);
    }
  }

  SocketUtils::shutdown();

  return 0;
}
