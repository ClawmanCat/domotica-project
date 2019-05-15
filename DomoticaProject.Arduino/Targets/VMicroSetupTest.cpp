/*
    Arduino Testfile

    Use this file to test if Visual Micro is installed and set up correctly.
    Insert a 6-bit LED into ports 8-13. Uploading this file will make them count.
*/

void setup() {
    DDRB = B00111111;
}

void loop() {
    PORTB = 0xFF;

    for (byte i = (1 << 6); i >= 0; --i) {
        PORTB = i;
        delay(250);
    }
}
