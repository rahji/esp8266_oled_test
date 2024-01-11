# ESP8266 OLED Test

This is an example that works with the NodeMCU board found here: <https://www.aliexpress.us/item/3256805824888360.html>

The OLED screen is 128x64 pixels and is connected via I2C. The I2C pins are 14 (SDA) and 12 (SCL)

After installing the OSC library via PIO's "Libraries" page, I needed to delete the two bluetooth header and source files from `.pio/libdeps/nodemcuv2/OSC`. This doesn't seem like a great workaround since the `.pio` directory isn't under source control and I assume those files would return if I reinstalled or updated the OSC library. Not sure. In any case, this was the only way I could see to keep it from including those files and causing a compile error. The ESP8266 board has no bluetooth capabilities.

In a YouTube video, I walk through the code: <https://www.youtube.com/watch?v=EqqDQa8BTsw>

![Animated demo GIF](esp8266.gif)