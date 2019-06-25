#pragma once

// Client device ID must be unique within network.
// Make sure to change it when adding a client.
// IDs should be continuous.
#define ClientOnly
#define CLIENT_DEVICE_ID 2

#include "IArduinoTarget.h"
#include "RFManager.h"

struct ArduinoClient : public IArduinoTarget<ArduinoClient> {
    static void setup(void) {
    
    }

    static void loop(void) {
        RF.HandleClientRequest();
    }
};