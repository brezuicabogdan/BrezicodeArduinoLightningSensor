# BreziCode Lighting Sensor

Turn and 16Mhz arduino int a lightining sensor. This is based on the great article from Hackster.io - <a href="https://www.hackster.io/runtimeprojects/a-lightning-detector-for-arduino-9f679c" targer="_blank">HERE</a> 

Please read the above instruction for the basic functionality and background.

The article abve describes how to use an Arduino, 3 resistors and a piece of wire to detect and graph lightning strikes. 

What this code does is basically turn an arduino into a I2C Lightining sensor to be used in larger applications.


#### Why not an Arduino Library?

Well because there is not much you can do beside this on the same micro-controller. Because detecting lightining events requires very fast analog readings any other processing will impair the functionality of the detector. Again this is meant to use the Arduino as a sensor and not as a processor (micro-controller).


#### Basic theory 

See <a href="https://www.hackster.io/runtimeprojects/a-lightning-detector-for-arduino-9f679c" target="_blank">https://www.hackster.io/runtimeprojects/a-lightning-detector-for-arduino-9f679c</a>

All credits to the ideea and implementation of using an Arduino as a lightinng detector goe to <a href="https://www.hackster.io/runtimeprojects" target="_balnk">runtimeprojects</a>

#### How does the code work

The code reads an analog pin continuously and regords the readings for the past 60 seconds. After that it can calculate the number of strikes per minute and the average strike intensity for the last minute as a percent.

##### Settings:

At the begining of this sketch you canfind some constans you can customize to make this sensor work as want it to:

**#define PIN A7** - the Analog Arduino pin you have the antenan atatched to

**#define I2C_ADDR 1** - The I2C(Wire) slave address you will communicate with the sensor on

**#define ATTN_MARGIN 10** - Treshold value to consider a strike. Any deviance more than this value from the baseline will be considered a strike.

**#define CALIBRATION_TIME 5000** - How many seconds will the sensor calibrate for. At the beggining of the sketch we go through a calibration process which basically determines the normal base reading - the baseline. Thi is used as  a reference forfuture readings. Dynamical baseline is used because every arduino is different and although any Arduino analog reading should be between 0 and 1023 in reality because of power fluctuations in the Arduino supply and other factors and because of the voltage divider to set the base voltage at the middle of the scale this may not be perfect.

**#define NUM_READINGS 60** - How many readings do you want to record. This is normally set to 60 which means 60 seconds and this will give you readings like strikes per minute and average strike intensity for a minute You can increase this number to for example 600 if you want strikes per 10 minutes.


#### Notes:

1. As the above article states only 16Mhz Arduinos will work. I use this code with an Arduino Nano 5v 16Mhz
2. From my experience I got the best results with a 20cm single core copper wire as an antenna. (I used one wire from a Cat5 UTP Ethernet cable) and when outdoors. Indoor the sensor will work but the sensitivity will suffer a lot. To increase the sensitivity you can play around with the 3.3 MOhm resistor but I DO NOT recommend  this as it can lead to the distruction of your analog pin or the whole arduino.

#### License and copyright

I take no responsability for any damage caused to you or your adrionos or other tools/electronics if you attempt to implement this. The code is provided as-is without any warranty.

As above stated the copyright on the initial implementation goes to <a href="https://www.hackster.io/runtimeprojects/a-lightning-detector-for-arduino-9f679c" targer="_blank">HERE</a> 