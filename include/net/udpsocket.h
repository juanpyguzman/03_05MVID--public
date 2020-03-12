//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef VIU_UDP_SOCKET_H
#define VIU_UDP_SOCKET_H 1

#include <memory>
#include <WinSock2.h>

class SocketAddress;
class UDPSocket {
  public:
    UDPSocket(SOCKET inSocket);
    ~UDPSocket();

    int bindTo(const SocketAddress& inToAddress) const;
    int sendTo(const void* inData, int inLen, const SocketAddress& inTo) const;
    int receiveFrom(void* inBuffer, int inLen, SocketAddress& outFrom) const ;
    int setBlockingMode(bool blocking) const;
  private:
    friend class SocketUtils;
    SOCKET socket_;
};

typedef std::unique_ptr<UDPSocket> UDPSocketPtr;

#endif
