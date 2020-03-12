//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#include "net/tcpsocket.h"
#include "net/socketaddress.h""

#include <iostream>

TCPSocket::~TCPSocket() {
  closesocket(socket_);
}

int TCPSocket::connectTo(const SocketAddress& inAddress) const {
  const int err = connect(socket_, &inAddress.sockAddr_, static_cast<int>(inAddress.getSize()));

  if (err < 0) {
    std::cout << "Error Connecting Socket" << std::endl;
    return -WSAGetLastError();
  }
  return NO_ERROR;
}

int TCPSocket::bindTo(const SocketAddress& inBindAddress) const {
  const int error = bind(socket_, &inBindAddress.sockAddr_, static_cast<int>(inBindAddress.getSize()));

  if (error != 0) {
    std::cout << "Error Binding Socket" << std::endl;
    return WSAGetLastError();
  }
  return NO_ERROR;
}

int TCPSocket::listenTo(int inBackLog) const {
  const int err = listen(socket_, inBackLog);

  if (err < 0) {
    std::cout << "Error Listening Socket" << std::endl;
    return -WSAGetLastError();
  }
  return NO_ERROR;
}

TCPSocketPtr TCPSocket::acceptCon(SocketAddress& inFromAddress) const {
  int length = static_cast<int>(inFromAddress.getSize());
  const SOCKET newSocket = accept(socket_, &inFromAddress.sockAddr_, &length);
  if (newSocket != INVALID_SOCKET) {
    return TCPSocketPtr(std::make_unique<TCPSocket>(newSocket));
  }

  std::cout << "Error Accepting Socket" << std::endl;
  return nullptr;
}

int TCPSocket::sendTo(const void* inData, int inLen) const {
  const int bytesSentCount = send(socket_, static_cast<const char*>(inData), inLen, 0);

  if (bytesSentCount < 0) {
    std::cout << "Error Sending Data" << std::endl;
    return -WSAGetLastError();
  }
  return bytesSentCount;
}

int TCPSocket::receiveFrom(void* inData, int inLen) const {
  const int bytesReceivedCount = recv(socket_, static_cast<char*>(inData), inLen, 0);

  if (bytesReceivedCount < 0) {
    std::cout << "Error Receiving Data" << std::endl;
    return -WSAGetLastError();
  }
  return bytesReceivedCount;
}