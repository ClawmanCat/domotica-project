#pragma once

#include <RH_ASK.h>
#include "../Shared/GlobalSerial.h"
#include "../Server/Message.h"

#define RFManager RFTransmission::instance()

class RFTransmission {
public:
    const static byte TRNS_PIN = 2;
    const static byte RECV_PIN = 3;

    static RFTransmission instance(void) {
        static RFTransmission i = RFTransmission();
        return i;
    }

    // Send a message and wait for a response
    Message SendAndWait(Message& msg, int timeoutms = 1000) {
        if (!initialized) init();

        UniqueArray<byte> message = new byte[strlen(msg.data.raw_ptr()) + 7];
        memcpy(message.raw_ptr() + 0, &msg.MessageID,     4);
        memcpy(message.raw_ptr() + 4, &msg.device,        1);
        memcpy(message.raw_ptr() + 5, &msg.port,          1);
        memcpy(message.raw_ptr() + 6, msg.data.raw_ptr(), strlen(msg.data.raw_ptr()) + 1);

        // Send message.
        driver.send(message.raw_ptr(), strlen(msg.data.raw_ptr()) + 7);
        
        
        // Get response.
        byte buffer[256];
        unsigned long start = millis();


    }
private:
    RFTransmission(void) : initialized(false), driver(2000, RECV_PIN, TRNS_PIN) {}

    bool initialized;
    RH_ASK driver;

    void init(void) {
        if (!driver.init()) GSerial.println(F("Failed to initialize RF driver. Problems may arise later."));
        initialized = true;
    }
};