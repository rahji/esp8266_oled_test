/*

This is an example that works with the NodeMCU board found here:
https://www.aliexpress.us/item/3256805824888360.html

OLED screen is 128x64 pixels and is connected via I2C
The I2C pins are 14 (SDA) and 12 (SCL)

*/

#include <ESP8266WiFi.h>
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

void setup()
{
    Wire.begin(14, 12);
    display = Adafruit_SSD1306(128, 64, &WIRE);

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32

    // Clear the buffer, so it doesn't show the default image
    display.clearDisplay();
    display.display();

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    // write network connection info
    display.setCursor(0, 0);
    display.print("Connecting to ");
    display.println(ssid);
    display.display();
    WiFi.begin(ssid, pass);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
        display.display();
    }

    display.println("WiFi connected");
    display.print("IP address: ");
    display.println(WiFi.localIP());
    display.display();

    display.println("Starting UDP");
    display.display();
    Udp.begin(localPort);
    display.print("Local port: ");
    display.println(Udp.localPort());
    display.display();

    // display.setCursor(0, 0);
    // display.display();
}

void loop()
{
    // display.print("overwrote");
    // delay(10);
    // yield();
    // display.display();

    OSCMessage msg;
    int size = Udp.parsePacket();

    if (size > 0)
    {
        while (size--)
        {
            msg.fill(Udp.read());
        }
        if (!msg.hasError())
        {
            display.println("Received UDP message!");
        }
        else
        {
            error = msg.getError();
            display.print("Error: ");
            display.println(error);
        }
        display.display();
    }
}