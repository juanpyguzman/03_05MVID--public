//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef VIU_TCP_SOCKET_H
#define VIU_TCP_SOCKET_H 1

#include <memory>
#include <WinSock2.h>

class SocketAddress;
class TCPSocket {
  public:
    TCPSocket(SOCKET inSocket) : socket_(inSocket) {}
    ~TCPSocket();

    int connectTo(const SocketAddress& inAddress) const;
    int bindTo(const SocketAddress& inToAddress) const;
    int listenTo(int inBackLog = 32) const;
    std::unique_ptr<TCPSocket> acceptCon(SocketAddress& inFromAddress) const;
    int sendTo(const void* inData, int inLen) const;
    int receiveFrom(void* inBuffer, int inLen) const;
  private:
    friend class SocketUtils;
    SOCKET socket_;
};

typedef std::unique_ptr<TCPSocket> TCPSocketPtr;

#endif
