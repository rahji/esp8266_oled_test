/*

This is an example that works with the NodeMCU board found here:
https://www.aliexpress.us/item/3256805824888360.html

OLED screen is 128x64 pixels and is connected via I2C
The I2C pins are 14 (SDA) and 12 (SCL)

More information about the graphics library can be found here:
https://learn.adafruit.com/adafruit-gfx-graphics-library/graphics-primitives

A bunch of examples showing how the OSC library works can be found here:
https://github.com/CNMAT/OSC/tree/master/examples

*/

#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif

#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define WIRE Wire
#define OLED_RESET -1

Adafruit_SSD1306 display;

char ssid[] = "FalafelHaus";  // wifi network SSID (name)
char pass[] = "GoodDogRonin"; // wifi network password

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;
const IPAddress outIp(10, 40, 10, 105); // remote IP (not needed for receive)
const unsigned int outPort = 9999;      // remote port (not needed for receive)
const unsigned int localPort = 8888;    // local port to listen for UDP packets (here's where we send the packets)

OSCErrorCode error;

int counter = 0;

void setup()
{
    Wire.begin(14, 12);
    display = Adafruit_SSD1306(128, 64, &WIRE);

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32

    // Clear the buffer, so it doesn't show the default image (an Adafruit logo by default)
    display.clearDisplay();
    display.display();

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE, SSD1306_BLACK); // black background

    // Display network connection info as we connect to wifi
    display.setCursor(0, 0);
    display.print("Connecting to ");
    display.println(ssid);
    display.display();
    WiFi.begin(ssid, pass);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print("."); // working on it...
        display.display();
    }

    display.println("WiFi connected");
    display.print("IP: ");
    display.println(WiFi.localIP());
    display.display();

    display.println("Starting UDP");
    display.display();
    Udp.begin(localPort);
    display.print("Local port: ");
    display.println(Udp.localPort());
    display.display();

    // wait a few seconds so someone can see the IP address if they're watching
    delay(3000);

    // clear the buffer and draw the empty bar graphs and checkbox
    display.clearDisplay();
    display.drawRect(38, 0, 90, 10, SSD1306_WHITE); // top bar graph
    display.drawRect(38, 20, 90, 10, SSD1306_WHITE); // second bar graph
    display.drawRect(0, 44, 20, 20, SSD1306_WHITE);  // checkbox
    display.display();
}

// helper function for drawing rectangles
long mapValue(float val, long out_min, long out_max) {
    return int(val * (out_max - out_min) / 1 + out_min);
}

// faderA has a float value, draw a bar graph of the value
void faderA(OSCMessage &msg) {
    float val = msg.getFloat(0);
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.printf("%.4f", val);
    display.fillRect(38, 0, 90, 10, SSD1306_BLACK); // clear
    display.drawRect(38, 0, 90, 10, SSD1306_WHITE); // outline
    display.fillRect(38, 0, mapValue(val,0,90), 10, SSD1306_WHITE); // fill
}

// rotaryA has a float value, draw a bar graph of the value
void rotaryA(OSCMessage &msg) {
    float val = msg.getFloat(0);
    display.setCursor(0, 20);
    display.setTextSize(1);
    display.printf("%.4f", val);
    display.fillRect(38, 20, 90, 10, SSD1306_BLACK); // clear
    display.drawRect(38, 20, 90, 10, SSD1306_WHITE); // outline
    display.fillRect(38, 20, mapValue(val,0,90), 10, SSD1306_WHITE); // fill
}

// encoderM has a float value of either 0 or 1, increment or decrement a counter
// and show it in big letters
void encoderM(OSCMessage &msg) {
    float val = msg.getFloat(0);
    display.setCursor(38, 44);
    display.setTextSize(3);
    if (val == 0) display.print(--counter);
    else display.print(++counter);
}

// toggleA has a float value of either 0 or 1, draw a filled rectangle if 1
void toggleA(OSCMessage &msg) {
    float val = msg.getFloat(0);
    display.fillRect(0, 44, 20, 20, SSD1306_BLACK); // clear
    display.drawRect(0, 44, 20, 20, SSD1306_WHITE); // outline
    if (val == 1) display.fillRect(4, 48, 12, 12, SSD1306_WHITE); // fill
}

void loop() {

    OSCMessage msg;
    int size = Udp.parsePacket();

    if (size > 0) {
        while (size--) {
            msg.fill(Udp.read());
        }
        if (!msg.hasError()) {
            msg.dispatch("/1/faderA", faderA);
            msg.dispatch("/1/rotaryA", rotaryA);
            msg.dispatch("/encoderM", encoderM);
            msg.dispatch("/toggleA_1", toggleA);
        } else {
            error = msg.getError();
            display.setCursor(0, 0);
            display.print("Error: ");
            display.println(error);
        }
        display.display();
    }
}