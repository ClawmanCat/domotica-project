# Domotica Project
Cross platform LUA controller for Arduino based domotica systems.

## Getting Started
Before downloading this repository, please install the following Visual Studio components/extensions:
- Desktop development with C++
- Visual Micro Extension
- Mobile development with .NET

Visual Micro may take some manual tweaking to be set up correctly. Please ensure that:
- Your Controller IDE is set to "Arduino/Genuino Uno" (Or to "Arduino/Genuino Mega w/ ATMega2560 (Mega 2560)" when using the Arduino Mega)
- You have installed the arduino.exe executable from [arduino.cc](https://arduino.cc)
- Your Microcontroller Application is set to "Arduino 1.6/1.8" and points to the aforementioned arduino.exe executable.
- Your Arduino is plugged in, and selected by Visual Micro. (It will appear as "COM\<Some number\> - USB-SERIAL \<More letters/numbers\>",
  assuming it's plugged in as a USB device.)

The DomoticaProject.Arduino project comes with the ArduinoTest.h file to test if everything is set up correctly.
If everything works correctly, this program will count from 0 to 63 using a 6-bit LED, to be plugged into ports 8 - 13.

The ArduinoTarget.zip file contains a Visual Studio template for generating Arduino Targets. (Sketches)
Installing this template is optional, but highly recommended. Install it by moving it to the Visual Studio templates folder.
(Usually My Documents/Visual Studio \<YEAR\>/Templates)

## Server / Client Setup
To setup the server:
- Go to DomoticaProject.Arduino.cpp and change the SRC macro to have the value ArduinoServer.
The macro definition should look as follows: #define SRC ArduinoServer
- Connect an ethernet shield to the Arduino.
- Connect an RF receiver and transmitter to the Arduino. By default, the server will use pin 2 as the transmitter and pin 3 as the receiver.
You can change this by changing TRNS_PIN and RECV_PIN in RFManager.h
- Upload the project to the Arduino.
- The server will print its IP address through Serial. By default, it will use DHCP. if you prefer a static IP address, 

Please note that the server uses approximately 4 KB of RAM, and thus cannot run from an Arduino Uno.
The server will only look for clients when first starting, so make sure that all clients are online before starting the server.
(or just restart the server when they are.)


To setup the client:
- Create a new header for your client, preferably in Targets/Client. Use the ArduinoTarget Visual Studio template, provided in ArduinoTarget.zip.
- Go to DomoticaProject.Arduino.cpp and change the SRC macro to have the name of your client file.
For example, if you created a file called MyClient.h, your macro should look like #define SRC MyClient
- Connect any devices you want to use to the Arduino, and create an Attachable object in the AttachableRegistry for it.
For example, if you had an array of LEDs on PORTB, you could add the following method call to the setup method of your client:

```cpp
AttRegistry.RegisterAttachable(
	new Attachable(
		// Callback when initializing.
		[](Attachable* thisptr) {
			DDRB = 0xFF;
		},
		// Callback when a request to this attachable is made from the app.
		[](Attachable* thisptr, SafeCString&& request_data) {
			// This attachable requires a number.
			if (!IsNumber(data)) return SafeCString::FromCopy("NO", 3);
			
			// Convert the string to a number.
			byte new_value = (byte) strtol(data.raw_ptr(), nullptr, 10);
			
			// Write the value to the LED array.
			PORTB = new_value;
			
			// Send message back to app. (Required!)
			return SafeCString::FromCopy("OK", 3);
		},
		// The name to be shown in the app for this attachable.
		SafeCString::FromCopy("Client LED Array", 17),
		// The unique ID for this attachable. Don't use 0.
		1
	);
);
```

- In your client file create the following macros, above any #include directives:

```cpp
#define ClientOnly
#define CLIENT_DEVICE_ID <Number>
```

where <Number> is the ID of your client in the network. Client IDs should be:
   - Unique. (No two clients may have the same ID)
   - Continuous. (Client IDs should follow each other, e.g. 2, 3, 4, 5 but not 2, 3, 11, 45)
   - Start at 2.
   - Be smaller than 127.
- Upload the project to the Arduino.

If everything is done correctly, you should now be able to see the attachable on the aliasing page in the app,
and be able to call it by its alias in the Lua editor.