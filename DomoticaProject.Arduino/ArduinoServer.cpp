#include "ArduinoServer.h"

const byte  ArduinoServer::MAC_ADDRESS[] = { 0xFA, 0x32, 0xA5, 0x50, 0xF3, 0x3B };
const byte  ArduinoServer::IP_ADDRESS[]  = { 192, 168, 1, 2 };
const short ArduinoServer::PORT          = 5300;

EthernetServer ArduinoServer::server = EthernetServer(ArduinoServer::PORT);