/*
    This file is used to load other files.
    Please insert the name of the header to be loaded in the SRC macro below.
    Class name should match file name, and implement IArduinoTarget.
*/

// Insert the name of the target here:
#define SRC SocketServer


#define STRFY(x) #x
#define INC_F(x) STRFY(Targets/x.h)
#include INC_F(SRC)

void setup() {
    SRC::setup();
}

void loop() {
    SRC::loop();
}
