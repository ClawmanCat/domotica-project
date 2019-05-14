# Domotica Project
Arduino/Domotica project controlled over TCP via a cross-platform Xamarin Forms application.

## Getting Started
Before downloading this repository, please install the following Visual Studio components/extensions:
- Desktop development with C++
- Visual Micro Extension
- Mobile development with .NET

Visual Micro may take some manual tweaking to be set up correctly. Please ensure that:
- Your Controller IDE is set to "Arduino/Genuino Uno"
- You have installed the arduino.exe executable from [arduino.cc](https://arduino.cc)
- Your Microcontroller Application is set to "Arduino 1.6/1.8" and points to the aforementioned arduino.exe executable.
- Your Arduino is plugged in, and selected by Visual Micro. (It will appear as "COM\<Some number\> - USB-SERIAL \<More letters/numbers\>",
  assuming it's plugged in as a USB device.)

The DomoticaProject.Arduino project comes with the DomoticaProject.Arduino.ino file to test if everything is set up correctly.
If everything works correctly, this program will count from 0 to 63 using a 6-bit LED, to be plugged into ports 8 - 13.
