/*
    Arduino Testfile

    Use this file to test if Visual Micro is installed and set up correctly.
    Insert a 6-bit LED into ports 8-13. Uploading this file will make them count.
*/

void setup() {
    DDRB = B00111111;
}

void loop() {
    PORTB = 0x00;

    for (byte i = 0; i < (1 << 6); i++) {
        PORTB = i;
        delay(250);
    }
}
