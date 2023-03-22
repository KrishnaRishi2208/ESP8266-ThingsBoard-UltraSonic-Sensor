# ESP8266-ThingsBoard-UltraSonic-Sensor
View Ultrasonic Sensor data using ThingsBoard Dashboard

<h3>Requirements:</h3>
1 esp8266
1 UltraSonic Sensor
Jumper wires

<h3>Procedure</h3>

1)First,connect the board with the following schema:

      esp8266 GND  ----> Ultrasonic GND
      
      esp8266 D2   ----> Ultrasonic Echo
      
      esp8266 D1   ----> Ultrasonic Trig
      
      esp8266 Vin  ----> Ultrasonic Vcc

2)Then,download and run the UltrasonicDataSend.ino file. In the file, change your ssid,password.


3)Then, go to https://demo.thingsboard.io/ and sign in to your account.

      Go to devices and create a new device with any name and click add.
      
      Here, you need to configure the device to your application by clicking on manage credentials and copying the access token and pasting it into the  ACCESS_TOKEN variable defined in the UltrasonicDataSend.ino file.
      
      Go to dashboards and import ultrasonic_test.json file.
      
      In the dashboard, change the alias to the device you just created.
      
      Here, you may need to add the data key as "distance"(use the same spelling).

4)Now, upload your code to esp8266 and view the data on your ThingsBoard dashboard.

