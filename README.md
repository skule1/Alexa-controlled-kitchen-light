# Kitchen-lamp controlled by Alexa

My wife demanded that our kitchen lamp should be changed. She thought that the lamp now is old and not fancy any more. So we went to a lamp store to look for a new lamp for the kitchen. There were several to choose among, but my wife and I were not agreed on what to choose. At last, when the price tag was high enough, we found a led device we thought was suitable for our home. So be bought it and carried it home.

After some efforts we succeeded in mounting the lamp in the ceiling. But when we turned it on, the light emitted was too high. So we concluded that the renewal project of the kitchen lamp was not a great success.

But then the Alexa system that we have implemented in our home came to our mind. In fact, Alexa controlled the lamp in the previous lamp by mean of at Philips Hue light bulb, controlled by the Philips HUE app and Alexa. Why not make a similar device for the new lamp also?

Since the new lamp is a LED device, I had to make a pulse width modulation unit controlled by some smart microcontroller device.  We studied the controller initially installed in the lamp and found out that the LEDs were supported by a 40 volt DC power supply at the current rate up to 700 mA.

At eBay I found at LED pulse width controller able to control a current up to 700mA with an input power voltage of 35 volt. In order to get the 35 volts from the 45 volt DC power unit I found a regulator that could to the task. The PWD-pulses could be delivered from a ESP8266 D1 mini device with a WiFi connection. That device demand 5 volts input, so I had to use another regulator to get that.

The software in the microcontroller was accomplished from example Arduino sketches form the SPI.h, WiFi.h, PubSubClient.h, Adafruit_GFX.h, Adafruit_SSD1306.h, EEPROM.h, ESPmDNS.h, WiFiUdp.h, and ArduinoOTA.h. After the sketch has been loaded, Alexa app was added the device by scanning the WiFi where the controller was found with the id  'kitchenlamp'. That made the connection to the Alexa.

<img width=170 height=170
src="README-filer/image002.jpg" align=left hspace=12 v:shapes="Bilde_x0020_4"><img width=170 height=170
src="README-filer/image004.jpg" align=left hspace=12 v:shapes="Bilde_x0020_4"><img width=170 height=170
src="README-filer/image006.jpg" align=left hspace=12 v:shapes="Bilde_x0020_4"><img width=170 height=170
src="README-filer/image008.jpg" align=left hspace=12 v:shapes="Bilde_x0020_4">
