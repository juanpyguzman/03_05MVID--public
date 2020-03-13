//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#include "net/socketaddress.h"
#include "net/udpsocket.h"

#include <iostream>

UDPSocket::UDPSocket(SOCKET inSocket) : socket_(inSocket) {}

UDPSocket::~UDPSocket() {
  closesocket(socket_);
}

int UDPSocket::bindTo(const SocketAddress& inToAddress) const {
  const int err = bind(socket_, &inToAddress.sockAddr_, static_cast<int>(inToAddress.getSize()));

  if (err != 0) {
    std::cout << "Error Binding Socket" << std::endl;
    return WSAGetLastError();
  }
  return NO_ERROR;
}

int UDPSocket::sendTo(const void* inData, int inLen, const SocketAddress& inTo) const {
  const int byteSentCount = sendto(socket_, static_cast<const char*>(inData), inLen, 0,
    &inTo.sockAddr_, static_cast<int>(inTo.getSize()));

  if (byteSentCount >= 0) {
    std::cout << "Sent " << byteSentCount << " bytes" << std::endl;
    return byteSentCount;
  } else {
    std::cout << "Error Sending Data" << std::endl;
    return -WSAGetLastError();
  }
}

int UDPSocket::receiveFrom(void* inBuffer, int inLen, SocketAddress& outFrom) const {
  int fromLength = static_cast<int>(outFrom.getSize());
  const int readByteCount = recvfrom(socket_, static_cast<char*>(inBuffer), inLen,
    0, &outFrom.sockAddr_, &fromLength);

  if (readByteCount >= 0) {
    std::cout << "Received " << readByteCount << " bytes" << std::endl;
    return readByteCount;
  } else {
    std::cout << "Error Receiving Data Socket" << std::endl;
    return -WSAGetLastError();
  }
}

int UDPSocket::setBlockingMode(bool blocking) const {
#if _WIN32
  u_long arg = blocking ? 1 : 0;
  const int result = ioctlsocket(socket_, FIONBIO, &arg);
#else
  int flags = fcntl(socket_, F_GETFL, 0);
  flags = inShouldBeNonBlocking ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK);
  fcntl(socket_, F_SETFL, flags);
#endif
  if (result == SOCKET_ERROR) {
    std::cout << "Error Changing Mode To Socket" << std::endl;
    return WSAGetLastError();
  } else {
    return NO_ERROR;
  }
}