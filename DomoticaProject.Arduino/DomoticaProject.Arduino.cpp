/*
    This file is used to load other files.
    Please insert the name of the header to be loaded in the SRC macro below.
    Class name should match file name, and implement IArduinoTarget.
*/

#define SRC VMTest


// IntelliSense may complain the file doesn't exist.
// IntelliSense is wrong.
#define STRFY(x) #x
#define INC_F(x) STRFY(x.h)
#include INC_F(SRC)

void setup() {
    SRC::setup();
}

void loop() {
    SRC::loop();
}
