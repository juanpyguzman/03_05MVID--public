//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef VIU_SOCKET_ADDRESS_H
#define VIU_SOCKET_ADDRESS_H 1

#include <cstdint>
#include <memory>
#include <WinSock2.h>
#include <Ws2tcpip.h>

class SocketAddress {
  public:
    SocketAddress() = default;

    SocketAddress(uint32_t address, uint16_t port) {
      sockaddr_in* sockA = getAsSockAddrIn();
      sockA->sin_family = AF_INET;
      sockA->sin_addr.S_un.S_addr = htonl(address);
      sockA->sin_port = htons(port);
    }

    SocketAddress(const sockaddr& sockAddr) {
      memcpy(&sockAddr_, &sockAddr, sizeof(sockaddr));
    }

    static size_t getSize() { return sizeof(sockaddr); }

  private:
    sockaddr_in* getAsSockAddrIn() {
      return reinterpret_cast<sockaddr_in*>(&sockAddr_);
    }

    sockaddr sockAddr_;

    friend class UDPSocket;
    friend class TCPSocket;
};

typedef std::unique_ptr<SocketAddress> SocketAddressPtr;

#endif
