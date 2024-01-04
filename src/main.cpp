/*

This is an example that works with the NodeMCU board found here:
https://www.aliexpress.us/item/3256805824888360.html

OLED screen is 128x64 pixels and is connected via I2C
The I2C pins are 14 (SDA) and 12 (SCL)

*/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define WIRE Wire
#define OLED_RESET -1

Adafruit_SSD1306 display;

void setup()
{
    Wire.begin(14, 12);
    display = Adafruit_SSD1306(128, 64, &WIRE);
    Serial.begin(9600);

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32

    Serial.println("OLED init done");

    // Clear the buffer, so it doesn't show the default image
    display.clearDisplay();
    display.display();

    // text display tests
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("yellow 1"); // print() also works
    display.println("yellow 2");
    display.println("blue 1");
    display.println("blue 2");
    display.println("blue 3");
    display.println("blue 4");
    display.println("blue 5");
    display.println("blue 6");
    display.setCursor(0, 0);
    // display.println("overwrote");
    display.display(); // actually display all of the above
}

void loop()
{
    // display.print("overwrote");
    // delay(10);
    // yield();
    // display.display();
}