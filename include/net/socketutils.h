//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef VIU_SOCKET_UTILS_H
#define VIU_SOCKET_UTILS_H 1

#include "socketaddress.h"
#include "tcpsocket.h"
#include "udpsocket.h"

#include <iostream>

class SocketUtils {
  public:

    enum SocketAddressFamily {
      INET = AF_INET,
      INET6 = AF_INET6
    };

    static bool init() {
#if _WIN32
      WSADATA wsaData;
      const int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
      if (iResult != NO_ERROR) {
        std::cout << "WSA Startup Failed" << std::endl;
        return false;
      }
      std::cout << "WSA Started" << std::endl;
#endif
      return true;
    }

  static void shutdown() {
#if _WIN32
    WSACleanup();
#endif
  }

  static SocketAddressPtr createIPv4FromString(const std::string& in) {
    const auto pos = in.find_last_of(':');
    std::string host, service;
    if (pos != std::string::npos) {
      host = in.substr(0, pos);
      service = in.substr(pos + 1);
    }
    else {
      host = in;
      service = "0";   //use default port...
    }
    addrinfo hint;
    memset(&hint, 0, sizeof(hint));
    hint.ai_family = AF_INET;
    addrinfo* list;
    const int error = getaddrinfo(host.c_str(), service.c_str(), &hint, &list);
    addrinfo* list_head = list;
    if (error != 0 && list != nullptr) {
      freeaddrinfo(list_head);
      return nullptr;
    }
    while (!list->ai_addr && list->ai_next) {
      list = list->ai_next;
    }
    if (!list->ai_addr) {
      freeaddrinfo(list_head);
      return nullptr;
    }

    auto result = std::make_unique<SocketAddress>(*list->ai_addr);
    freeaddrinfo(list_head);
    return result;
  }

  static UDPSocketPtr createUDPSocket(SocketAddressFamily inFamily) {
    const SOCKET s = socket(inFamily, SOCK_DGRAM, IPPROTO_UDP);

    if (s != INVALID_SOCKET) {
      return UDPSocketPtr(std::make_unique<UDPSocket>(s));
    }

    std::cout << "Error Creating UDP Socket" << std::endl;
    return nullptr;
  }

  static TCPSocketPtr createTCPSocket(SocketAddressFamily inFamily) {
    const SOCKET s = socket(inFamily, SOCK_STREAM, IPPROTO_TCP);

    if (s != INVALID_SOCKET) {
      return TCPSocketPtr(std::make_unique<TCPSocket>(s));
    }

    std::cout << "Error Creating TCP Socket" << std::endl;
    return nullptr;
  }
};
#endif
