#include "SocketServer.h"

const byte SocketServer::MAC_ADDRESS[] = { 0xFA, 0x32, 0xA5, 0x50, 0xF3, 0x3B };
const byte SocketServer::IP_ADDRESS[]  = { 192, 168, 1, 2 };
const short SocketServer::PORT = 5300;

EthernetServer SocketServer::server = EthernetServer(SocketServer::PORT);